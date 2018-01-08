#include "sys.h"  

#if 0
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
#endif


#define ATTR_NO_INSTRUMENT_FUNCTION __attribute__( ( no_instrument_function ) )

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
inline ATTR_NO_INSTRUMENT_FUNCTION void WFI_SET(void)
{
	__asm__ __volatile__ ("WFI");		  
}

#if 1
//�ر������ж�(���ǲ�����fault��NMI�ж�)
inline ATTR_NO_INSTRUMENT_FUNCTION void INTX_DISABLE(void)
{
	__asm__ __volatile__ ("CPSID   I");
	//__asm__ __volatile__ ("CPSID   I\n\t"
	//"BX  LR");	  
	//__asm__ __volatile__ ("BX  LR");	  
	
}
#endif

//���������ж�
inline ATTR_NO_INSTRUMENT_FUNCTION void INTX_ENABLE(void)
{
	__asm__ __volatile__ ("CPSIE   I"); 
	//__asm__ __volatile__ ("BX LR");  
	
}
//����ջ����ַ
//addr:ջ����ַ
inline ATTR_NO_INSTRUMENT_FUNCTION void MSR_MSP(u32 addr) 
{
	__asm__ __volatile__ ("MSR MSP, r0");	 
	__asm__ __volatile__ ("BX r14");
	
}

#if 0
inline ATTR_NO_INSTRUMENT_FUNCTION void INTX_DISABLE(void)
{
    unsigned long old,temp;
    __asm__ __volatile__("mrs %0, cpsr\n"     "orr %1, %0, #0x80\n"
    "msr cpsr_c, %1" 
    : "=r" (old), "=r" (temp)
    :
    : "memory");
    //return (old & 0x80) == 0;
}
#endif









