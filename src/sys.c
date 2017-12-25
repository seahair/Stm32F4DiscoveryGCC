#include "sys.h"  

#if 0
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
#endif



//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__asm__("WFI");		  
}
//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__asm__("CPSID   I"); 
	__asm__("BX  LR");	  
	
}
//开启所有中断
void INTX_ENABLE(void)
{
	__asm__("CPSIE   I"); 
	__asm__("BX LR");  
	
}
//设置栈顶地址
//addr:栈顶地址
void MSR_MSP(u32 addr) 
{
	__asm__("MSR MSP, r0");	 
	__asm__("BX r14");
	
}












