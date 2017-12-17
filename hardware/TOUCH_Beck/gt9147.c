#include "touch.h"
#include "stm32f4xx_gpio.h"
#include "gt9147.h"
#include "delay.h"



const TOUCH_ATTR gt9147_attr = {
		GPIO_Pin_0,
		GPIO_Pin_11,
		GPIOB,
		GPIOF,
		0,
		0
};

//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const u8 GT9147_CFG_TBL[]=
{ 
		0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
		0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
		0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
		0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
		0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
		0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
		0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
		0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
		0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
		0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
		0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,
}; 

//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
static u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
		u8 i;
		u8 ret=0;
		I2cStart();	
		I2cSendByte(GT_CMD_WR);	//发送写命令	 
		I2cWaitAck();
		I2cSendByte(reg>>8);		//发送高8位地址
		I2cWaitAck();													   
		I2cSendByte(reg&0XFF);		//发送低8位地址
		I2cWaitAck();  
		for(i=0;i<len;i++)
		{	   
				I2cSendByte(buf[i]);	//发数据
				ret=I2cWaitAck();
				if(ret)break;  
		}
		I2cStop();					//产生一个停止条件	    
		return ret; 
}

//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
static void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
		u8 i; 
		I2cStart();	
		I2cSendByte(GT_CMD_WR);   //发送写命令 	 
		I2cWaitAck();
		I2cSendByte(reg>>8);   	//发送高8位地址
		I2cWaitAck(); 	 										  		   
		I2cSendByte(reg&0XFF);   	//发送低8位地址
		I2cWaitAck();  
		I2cStart();  	 	   
		I2cSendByte(GT_CMD_RD);   //发送读命令		   
		I2cWaitAck();	   
		for(i=0;i<len;i++)
		{	   
				buf[i]=I2cRecvByte(i==(len-1)?0:1); //发数据	  
		} 
		I2cStop();//产生一个停止条件    
} 

//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
static u8 GT9147_Send_Cfg(u8 mode)
{
		u8 buf[2];
		u8 i=0;
		buf[0]=0;
		buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
		for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//计算校验和
		buf[0]=(~buf[0])+1;
		GT9147_WR_Reg(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
		GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
		return 0;
}

static void gt9147_resetinit( void )
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;//PB1设置为上拉输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输入模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void gt9147_reset( void )
{
		GPIO_ResetBits( GPIOC, GPIO_Pin_13 );
		delay_ms( 10 );
		GPIO_SetBits( GPIOC, GPIO_Pin_13 );
		delay_ms( 10 );
}

static s8 gt9147_init( TOUCH_ATTR *ptouchattr )
{

}

static s8 gt9147_match( void )
{
		u8 temp[5]; 

		I2cInit( &gt9147_attr );
		gt9147_resetinit(  );
		gt9147_reset(  );
		delay_ms( 100 );

		GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
		temp[4]=0;
		printf("CTP ID:%s\r\n",temp);	//打印ID

		if(strcmp((char*)temp,"9147")==0)//ID==9147
		{
				temp[0]=0X02;			
				GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
				GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
				if(temp[0]<0X60)//默认版本比较低,需要更新flash配置
				{
						printf("Default Ver:%d\r\n",temp[0]);
						GT9147_Send_Cfg(1);//更新并保存配置
				}
				delay_ms(10);
				temp[0]=0X00;	 
				GT9147_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
				return 0;
		} 
		return -1;
}

static void gt9147_getxy( u16 *x, u16 *y )
{

}





const TOUCH_DIR gt9147_module = {
		gt9147_init,
		gt9147_match,
		gt9147_getxy
};
