#include "touch.h"
#include "stm32f4xx_gpio.h"
#include "gt9147.h"
#include "i2c.h"
#include "delay.h"
#include "ctiic.h"


#define  GT9147VERSION    0x60

const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

const static I2C_ATTR i2c_gt9147attr = {
	GPIO_Pin_0,
	GPIO_Pin_11,
	GPIOB,
	GPIOF,
	0,
	0
};

#if 0
const static TOUCH_ATTR touch_gt9147attr = {
	0,
	0,
	0
};	
#endif

const u8 ID_gt9147[4] = "9147";

static GT9147_ATTR mygt9147_attr;


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

#if 0
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
#endif 



//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
	CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
		CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
	CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}

//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
	CT_IIC_Start();	
	CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
		buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
	CT_IIC_Stop();//产生一个停止条件    
} 

//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
u8 GT9147_Send_Cfg(u8 mode)
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

static void gt9147_hdresetinit( void )
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

static void gt9147_hdreset( void )
{
	GPIO_ResetBits( GPIOC, GPIO_Pin_13 );
	delay_ms( 10 );
	GPIO_SetBits( GPIOC, GPIO_Pin_13 );
	delay_ms( 10 );
}

static void gt9147_update( void )
{
	u8 temp;

	GT9147_RD_Reg(GT_CFGS_REG,&temp,1);//读取GT_CFGS_REG寄存器
	if(temp < mygt9147_attr.version)//默认版本比较低,需要更新flash配置
	{
		printf("Default Ver:%d\r\n",temp);
		GT9147_Send_Cfg(mygt9147_attr.saveupdate);//更新并保存配置
	}
	delay_ms(10);
	
}

static void gt9147_sftreset( void )
{
	u8 temp;

	temp = GT_SFTRESET;			
	GT9147_WR_Reg(GT_CTRL_REG,&temp,1);//软复位GT9147

	gt9147_update( );

	temp = GT_SFTUNRESET;	 
	GT9147_WR_Reg(GT_CTRL_REG,&temp,1);//结束复位   

}

static s8 gt9147_init( TOUCH_ATTR *ptouchattr )
{
	mygt9147_attr.i2c_gt9147 = &i2c_gt9147attr;
	//mygt9147_attr.touch_gt9147 = &touch_gt9147attr;
	mygt9147_attr.ID = &ID_gt9147;
	mygt9147_attr.version = GT9147_CFG_TBL[0];
	mygt9147_attr.saveupdate = GT_SAVEUPDATE;

	gt9147_sftreset( );

	return 0;
}

static s8 gt9147_match( void )
{
	u8 temp[5]; 

	//I2cInit( &gt9147_attr );
	CT_IIC_Init( );
	gt9147_hdresetinit(  );
	gt9147_hdreset(  );
	//delay_ms( 100 );

	GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//打印ID

	if(strcmp((char*)temp, mygt9147_attr.ID)==0)//ID==9147
	{
		return 0;
	} 
	return -1;
}

static void gt9147_getxy( TOUCH_ATTR *ptouchattr )
{
	u8 mode;
	u8 temp;
	u8 i;
	u8 buf[4];

	GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//读取触摸点的状态  

	if( mode&0X80 )	//判断有无触点按下，以及按下了几个
	{
		temp=0;
		GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志		
		if( (mode&0XF)<6 )
		{
			ptouchattr->touchnum = (mode&0X0F);
			for( i=0; i<ptouchattr->touchnum; i++ )
			{
				GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
				if( ptouchattr->dir == TOUCHDIVV )
				{
					ptouchattr->OriginalXY.x[i] = ((u16)buf[1]<<8)+buf[0];
					ptouchattr->OriginalXY.y[i] = ((u16)buf[3]<<8)+buf[2];
				}
				else if( ptouchattr->dir == TOUCHDIVH )
				{
					ptouchattr->OriginalXY.y[i] = ((u16)buf[1]<<8)+buf[0];
					ptouchattr->OriginalXY.x[i] = 800 - ((u16)buf[3]<<8)+buf[2];
				}
			}
			ptouchattr->sta = TOUCHSTAYES;
		}
	}
	else //无触点按下
	{
		ptouchattr->sta	= TOUCHSTANO; 
	}
}





const TOUCH_DIR gt9147_module = {
	gt9147_init,
	gt9147_match,
	gt9147_getxy
};
