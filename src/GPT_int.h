/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *------16-01-2023
 *------Winter Sun
 **********************************************************************************************************************/

#ifndef GPT_INT_H_
#define GPT_INT_H_

#define MAIN_ISR_PERIOD 0x30D4 //period---16K ...GPT clock 400M

void R_GPT123_Create(void);
void R_GPT123_IO_int(void);
void gpt_counter_overflow_isr(void);
void gpt_counter_underflow_isr(void);

#define PWM_CMP_UPDATE_MACRO(t1_cmp,t2_cmp,t3_cmp)	\
		R_GPT1->GTCCRC = t1_cmp;					\
		R_GPT1->GTCCRE = t1_cmp;					\
		R_GPT2->GTCCRC = t2_cmp;					\
		R_GPT2->GTCCRE = t2_cmp;					\
		R_GPT3->GTCCRC = t3_cmp;					\
		R_GPT3->GTCCRE = t3_cmp;						

#endif

