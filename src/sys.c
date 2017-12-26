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


#define ATTR_NO_INSTRUMENT_FUNCTION __attribute__( ( no_instrument_function ) )

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
inline ATTR_NO_INSTRUMENT_FUNCTION void WFI_SET(void)
{
	__asm__ __volatile__ ("WFI");		  
}
//关闭所有中断(但是不包括fault和NMI中断)
inline ATTR_NO_INSTRUMENT_FUNCTION void INTX_DISABLE(void)
{
	__asm__ __volatile__ ("CPSID   I"); 
	__asm__ __volatile__ ("BX  LR");	  
	
}
//开启所有中断
inline ATTR_NO_INSTRUMENT_FUNCTION void INTX_ENABLE(void)
{
	__asm__ __volatile__ ("CPSIE   I"); 
	__asm__ __volatile__ ("BX LR");  
	
}
//设置栈顶地址
//addr:栈顶地址
inline ATTR_NO_INSTRUMENT_FUNCTION void MSR_MSP(u32 addr) 
{
	__asm__ __volatile__ ("MSR MSP, r0");	 
	__asm__ __volatile__ ("BX r14");
	
}












