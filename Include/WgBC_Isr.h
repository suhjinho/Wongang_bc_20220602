//#ifndef GLOBAL__VARIABLE_FUNCTION
//#define	GLOBAL__VARIABLE_FUNCTION
#ifndef __NTHS_ISR_H
#define __NTHS_ISR_H
//============================================================================
// Interrupt declaration
//============================================================================
// Á¦°øµÈ ¿¹Á¦¿¡¼­´Â ÀÎÅÍ·´Æ® ¼­ºñ½º ·çÆ¾À» "ramfuncs" ÀÌ¶ó´Â ¼½¼Ç¿¡ ÇÒ´çÇÏ°í
// "ramfuncs" ¼½¼ÇÀº ºü¸¥ ÀÀ´ä Ã³¸®¸¦ §ÇØ ³»ºÎ RAM¿¡¼­ µ¿ÀÛÇÏµµ·Ï ÇÔ

#pragma CODE_SECTION(epwm1_timer_isr, "ramfuncs");
interrupt void epwm1_timer_isr(void);

#pragma CODE_SECTION(epwm5_timer_isr, "ramfuncs");
interrupt void epwm5_timer_isr(void);

// CPU Timer0/1/2ÀÇ Interrupt Service Function ±¾ð
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
interrupt void cpu_timer0_isr(void);

#pragma CODE_SECTION(cpu_timer1_isr, "ramfuncs");
interrupt void cpu_timer1_isr(void);

// ADC Interrupt Service Function ¼±¾ð
#pragma CODE_SECTION(adc_isr, "ramfuncs");
interrupt void adc_isr(void);
#pragma CODE_SECTION(seq1adc_isr, "ramfuncs"); //Run from RAM
interrupt void seq1adc_isr(void);
#pragma CODE_SECTION(seq2adc_isr, "ramfuncs"); //Run from RAM
interrupt void seq2adc_isr(void);
#pragma CODE_SECTION(xint1_isr, "ramfuncs");
interrupt void xint1_isr(void);
//----- ÀÌ¼±Èñ, XINT2 Ãß°¡
#pragma CODE_SECTION(xint2_isr, "ramfuncs");
interrupt void xint2_isr(void);

#pragma CODE_SECTION(epwm1_tzint_isr, "ramfuncs");
interrupt void epwm1_tzint_isr(void);
void siv_test();

#endif



