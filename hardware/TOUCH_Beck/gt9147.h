#ifndef  __GT9147_H__
#define  __GT9147_H__

#include "touch.h"
#include "stm32f4xx.h"

//I2C读写命令	
#define GT_CMD_WR		0X28		//写命令
#define GT_CMD_RD		0X29		//读命令


//GT9147 部分寄存器定义 
#define GT_CTRL_REG	0X8040		//GT9147控制寄存器
#define GT_CFGS_REG	0X8047		//GT9147配置起始地址寄存器
#define GT_CHECK_REG	0X80FF		//GT9147校验和寄存器
#define GT_PID_REG		0X8140		//GT9147产品ID寄存器

#define GT_GSTID_REG	0X814E		//GT9147当前检测到的触摸情况
#define GT_TP1_REG		0X8150		//第一个触摸点数据地址
#define GT_TP2_REG		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG		0X8170		//第五个触摸点数据地址  



extern const TOUCH_DIR gt9147_module;

#endif