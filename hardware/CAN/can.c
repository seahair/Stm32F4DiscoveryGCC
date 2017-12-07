#include "can.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"



static CAN_ATTR can_attr;



static void Can_gpioinit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 PORTA 时钟

	GPIO_InitStructure.GPIO_Pin = can_attr.CanPinH| can_attr.CanPinL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 PA11,PA12

	GPIO_PinAFConfig(GPIOA,can_attr.CanPinH,GPIO_AF_CAN1);//PA11 复用为 CAN1
	GPIO_PinAFConfig(GPIOA,can_attr.CanPinL,GPIO_AF_CAN1); //PA12 复用为 CAN1
}

static void Can_structinit( void )
{
	CAN_InitTypeDef CAN_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能 CAN1 时钟

	CAN_InitStructure.CAN_TTCM=DISABLE; //非时间触发通信模式 
	CAN_InitStructure.CAN_ABOM=DISABLE; //软件自动离线管理 
	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒
	CAN_InitStructure.CAN_NART=ENABLE;//禁止报文自动传送
	CAN_InitStructure.CAN_RFLM=DISABLE; //报文不锁定,新的覆盖旧的 
	CAN_InitStructure.CAN_TXFP=DISABLE; //优先级由报文标识符决定
	CAN_InitStructure.CAN_Mode= can_attr.mode; //模式设置
	CAN_InitStructure.CAN_SJW= can_attr.sjwtime; //重新同步跳跃宽度
	CAN_InitStructure.CAN_BS1= can_attr.bs1time; //Tbs1 范围 CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2= can_attr.bs2time;//Tbs2 范围 CAN_BS2_1tq ~ CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler= can_attr.prescaler; //分频系数(Fdiv)为 brp+1
	CAN_Init(CAN1, &CAN_InitStructure); // 初始化 CAN1 

}

static void Can_filterinit( void )
{
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	CAN_FilterInitStructure.CAN_FilterNumber=0; //过滤器 0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32 位
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32 位 ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32 位 MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器 0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
}

static void Can_intruptinit( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0 消息挂号中断允许. 
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 主优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 次优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

s8 CanInit( CAN_ATTR *pcanattr )
{
	can_attr.CanPinH = pcanattr->CanPinH;
	can_attr.CanPinL = pcanattr->CanPinL;
	can_attr.mode = pcanattr->mode; 
	can_attr.sjwtime = pcanattr->sjwtime; 
	can_attr.bs1time = pcanattr->bs1time; 
	can_attr.bs2time = pcanattr->bs2time; 
	can_attr.prescaler = pcanattr->prescaler; 

	Can_gpioinit( );
	Can_structinit( );
	Can_filterinit( );
	Can_intruptinit( );

	return 0;
}

void CanIoctrl( u8 cmd, CAN_ATTR *pcanattr )
{
	switch ( cmd )
	{
		case CANENABLE:
			break;
		case CANDISABLE:
			break;
		case CANSETMODE:
			can_attr.mode = pcanattr->mode;
			break;
		default:
			break;
	}
	Can_structinit( );
}

s8 CanSend( u8 *buf, u16 len )
{
	u8 mbox;

	u16 i=0;

	CanTxMsg TxMessage;
	TxMessage.StdId=0x12; // 标准标识符为 0
	TxMessage.ExtId=0x12; // 设置扩展标示符（29 位）
	TxMessage.IDE=0; // 使用扩展标识符
	TxMessage.RTR=0; // 消息类型为数据帧，一帧 8 位
	TxMessage.DLC=len; // 发送两帧信息

	for(i=0;i<len;i++)
		TxMessage.Data[i]=buf[i]; // 第一帧信息 

	mbox= CAN_Transmit(CAN1, &TxMessage); 
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;
	if(i>=0XFFF)return 1;
	return 0;
}

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	int i=0;
	CAN_Receive(CAN1, 0, &RxMessage);
	//for(i=0;i<8;i++)
	//	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}

u8 CanRecv( u8 *buf )
{
	u32 i;

	CanRxMsg RxMessage;

	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;//没有接收到数据,直接退出
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据
	for(i=0;i<RxMessage.DLC;i++)
		buf[i]=RxMessage.Data[i]; 

	return RxMessage.DLC; //返回接受到的数据长度
}


