#include "remote.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

static REMOTE_ATTR reattr;

static void Remote_gpioinit( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOA8复用为TIM1
}

static u8 GetRemotePinStatus( void )
{
	return GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_8 );
}

static void Remote_timeinit( void )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//TIM1时钟使能 

	TIM_TimeBaseStructure.TIM_Prescaler=167;  ////预分频器,1M的计数频率,1us加1.	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=10000;   //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); 
}

static void Remote_captureinit( void )
{
	TIM_ICInitTypeDef  TIM1_ICInitStructure;

	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01		选择输入端 IC1映射到TI1上
	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM1_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0003 8个定时器时钟周期滤波
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);//初始化定时器1输入捕获通道
}

static void Remote_intruptinit( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;	//捕获中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//初始化NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;  //溢出中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//初始化NVIC寄存器

	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
}

void RemoteInit( void )
{
	reattr.value = 0;
	reattr.addr = 0;
	reattr.cmd  = 0;
	reattr.id  = 0;
	reattr.logic0time = 560;
	reattr.logic1time = 1680;
	reattr.starttime = 4500;
	reattr.stoptime = 14;	//10ms
	reattr.repeattime = 2500;
	reattr.timerank = 240;
	reattr.state = REMOTESTAWAIT;
	reattr.repeatcount = 0;

#if 1
	Remote_gpioinit( );
	Remote_timeinit( );
	Remote_captureinit( );
	Remote_intruptinit( );
#endif

	TIM_Cmd(TIM1,ENABLE );		//使能定时器1
}

u32 overcount;

void TIM1_UP_TIM10_IRQHandler(void)//定时器1溢出中断
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
	{
		switch ( reattr.state ) 
		{
			case REMOTESTAWAIT:
				break;
			case REMOTESTASTART:
				overcount++;
				if( overcount>reattr.stoptime )
				{
					reattr.state = REMOTESTASTOP;
					overcount = 0;
				}
				break;
			case REMOTESTAREPEAT:
				overcount++;
				if( overcount>reattr.stoptime )
				{
					reattr.state = REMOTESTASTOP;
					overcount = 0;
				}

				break;
			case REMOTESTASTOP:
				overcount = 0;
				break;
		}
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}

void TIM1_CC_IRQHandler(void)//定时器1输入捕获中断服务程序	 
{
	u16 dval;

	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)==SET) //处理捕获(CC1IE)中断
	{
		if( REMOTEPINUP == GetRemotePinStatus() )
		{
			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			TIM_SetCounter(TIM1,0);		//清空定时器值
		}
		else
		{
			dval=TIM_GetCapture1(TIM1);//读取CCR1也可以清CC1IF标志位
			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0	设置为上升沿捕获

			switch ( reattr.state ) 
			{
				case REMOTESTAWAIT:
					if( dval>(reattr.starttime-reattr.timerank) && dval<(reattr.starttime+reattr.timerank) )
						reattr.state = REMOTESTASTART;
					break;
				case REMOTESTASTART:
					if( dval>(reattr.logic0time-reattr.timerank) && dval<(reattr.logic0time+reattr.timerank) )
					{
						reattr.value<<=1;
						reattr.value|=0;
					}
					if( dval>(reattr.logic1time-reattr.timerank) && dval<(reattr.logic1time+reattr.timerank) )
					{
						reattr.value<<=1;
						reattr.value|=1;
					}
					if( dval>(reattr.repeattime-reattr.timerank) && dval<(reattr.repeattime+reattr.timerank) )
					{
						reattr.state = REMOTESTAREPEAT;
						reattr.repeatcount++;
					}
					break;
				case REMOTESTAREPEAT:
					if( dval>(reattr.repeattime-reattr.timerank) && dval<(reattr.repeattime+reattr.timerank) )
					{
						reattr.repeatcount++;
					}
					break;
				case REMOTESTASTOP:
					break;
			}
			TIM_SetCounter(TIM1,0);		//清空定时器值
			
		}

	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);  //清除中断标志位
}

static void RemoteDecode( void )
{
	u8 t1, t2;

	t1 = (reattr.value>>24) & (0XFF);
	t2 = (reattr.value>>16) & (0XFF);

	if( t1==(u8)~t2 && t1==reattr.id )
	{
		reattr.addr = t1;

		t1 = (reattr.value>>8) & (0XFF);
		t2 = (reattr.value) & (0XFF);
		if( t1==(u8)~t2 )
			reattr.cmd = t1;
		reattr.state = REMOTESTAWAIT;
	}
	
	reattr.value = 0;
}

u8   RemoteRead( void )
{
	if( reattr.state == REMOTESTASTOP )
	{
		RemoteDecode( );
		reattr.state = REMOTESTAWAIT;
		reattr.value = 0;
		reattr.repeatcount = 0;
	}

	return reattr.cmd;
}
