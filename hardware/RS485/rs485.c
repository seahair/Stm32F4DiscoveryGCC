#include "rs485.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"


u8 RS485_RX_BUF[64]; //接收缓冲,最大 64 个字节.
u8 RS485_RX_CNT=0;//接收到的数据长度

static RS485_ATTR rs485_attr;

static void Rs485_Enable( u8 cmd )
{
	if( RS485TXEN == cmd )
		GPIO_SetBits( GPIOG, rs485_attr.PinEn );
	if( RS485RXEN == cmd )
		GPIO_ResetBits( GPIOG, rs485_attr.PinEn );
}

static void Rs485_gpioinit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能 PA 时钟

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2 复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);//PA3 复用为 USART2

	GPIO_InitStructure.GPIO_Pin = rs485_attr.PinA | rs485_attr.PinB; //GPIOA2 与 GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化 PA2，PA3

	//RS485输出使能
	GPIO_InitStructure.GPIO_Pin = rs485_attr.PinEn; //GPIOG8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化 PG8
}

static void Rs485_uartinit( void )
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能 USART2 时钟

	USART_InitStructure.USART_BaudRate = rs485_attr.boundrate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为 8 位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发
	USART_Init(USART2, &USART_InitStructure); //初始化串口 2
	USART_Cmd(USART2, ENABLE); //使能串口 2
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

static void Rs485_intruptinit( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 VIC 寄存器

	Rs485_Enable( RS485RXEN );
}


s8 Rs485Init( RS485_ATTR *prs485attr )
{
	rs485_attr.PinA = prs485attr->PinA;
	rs485_attr.PinB = prs485attr->PinB;
	rs485_attr.PinEn = prs485attr->PinEn;
	rs485_attr.boundrate = prs485attr->boundrate;

	Rs485_gpioinit( );
	Rs485_uartinit( );
	Rs485_intruptinit( );

	return 0;
}

void Rs485Send( u8* addr, u16 len )
{
	u8 t;

	Rs485_Enable( RS485TXEN );
	for( t=0; t<len; t++ )
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束
		USART_SendData(USART2, *(addr+t) ); //发送数据
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束

	Rs485_Enable( RS485RXEN );
}

u16 Rs485Recv( u8* addr, u16 len )
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	len=0; //默认为 0

	delay_ms(10); //等待 10ms,连续超过 10ms 没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			*(addr+i)=RS485_RX_BUF[i];
		}
		len=RS485_RX_CNT; //记录本次数据长度
		RS485_RX_CNT=0; //清零
	}
	
	return len;
}

void USART2_IRQHandler(void)
{
	u8 res; 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{
		res =USART_ReceiveData(USART2);//;读取接收到的数据 USART2->DR
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res; //记录接收到的值
			RS485_RX_CNT++; //接收数据增加 1 
		} 
	} 
} 

