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



//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__asm__("WFI");		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{
	__asm__("CPSID   I"); 
	__asm__("BX  LR");	  
	
}
//���������ж�
void INTX_ENABLE(void)
{
	__asm__("CPSIE   I"); 
	__asm__("BX LR");  
	
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
	__asm__("MSR MSP, r0");	 
	__asm__("BX r14");
	
}












