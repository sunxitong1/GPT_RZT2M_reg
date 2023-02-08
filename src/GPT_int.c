

/*********************************************************************************************************************
	**
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *------16-01-2023
 *------Winter Sun
 *********************************************************************************************************************
	*/
//#include "bsp_register_protection.h"
#include "GPT_int.h"
#include "hal_data.h"
#include "vector_data.h"
#include "Foc.h"

#define GPT1_OVERFLOW_PRIORITY_LEVEL (10) /* The lower the value, the higher the priority of the corresponding interrupt(0-31) */
#define GPT1_UNDERFLOW_PRIORITY_LEVEL (11)


void R_GPT123_Create(void)
{
	__asm volatile("cpsid i");						/* Disable IRQ interrupt (Set CPSR.I bit to 1)	*/
	__asm volatile("isb");

	R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
	R_BSP_MODULE_START(FSP_IP_GPT, 1);
	R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

	/* Initialize all registers that may affect operation of this driver to reset values.  Skip these since they
	* affect all channels, and are initialized in GTCR and GTCNT: GTSTR, GTSTP, GTCLR. GTCR is set immediately after
	* clearing the module stop bit to ensure the timer is stopped before proceeding with configuration. */
	//GPT1 congiguratin for phase U
	R_GPT1->GTWP = 0xA500;
	R_GPT1->GTCR = 0U;
	R_GPT1->GTST = 0U;
	R_GPT1->GTCNT = 0U;

	R_GPT1->GTSSR = 0x80000000U;					//Enables count start by the GTSTR register
	R_GPT1->GTPSR = 0x80000000U;					//Enables count stop by the GTSTP register
	R_GPT1->GTCSR = 0x80000000U;					//Enables counter clear by the GTCLR registe
	R_GPT1->GTICASR = 0;
	R_GPT1->GTICBSR = 0;

	R_GPT1->GTCR_b.CST = 0; 						//Count operation is stopped

	R_GPT1->GTCR_b.MD = 0x05;						//triangle-wave PWM mode 2
	R_GPT1->GTCR_b.TPCS = 0x00; 					//Core Clock/1---GPT clock 400M
	R_GPT1->GTPR = MAIN_ISR_PERIOD;					//period---16K
	R_GPT1->GTPBR = MAIN_ISR_PERIOD; 				//period buffer

	R_GPT1->GTCNT = 0x00;							//CNT Clear;

	//IO function configuration
	R_GPT1->GTIOR_b.GTIOA = 0x03;					//initial low,	Output toggled at GTCCRA/GTCCRB compare match
	R_GPT1->GTIOR_b.GTIOB = 0x13;					//initial high,  Output toggled at GTCCRA/GTCCRB compare match
	R_GPT1->GTIOR_b.OAE = 1;						//GPTIOC0A output enable
	R_GPT1->GTIOR_b.OBE = 1;						//GPTIOC0B output enable

	//Buffer operation
	R_GPT1->GTBER_b.CCRA = 1;						//single buffer operation(GTCCRA <--> GTCCRC)
	R_GPT1->GTBER_b.CCRB = 1;						//single buffer operation(GTCCRB <--> GTCCRE)

	//Compare match value
	R_GPT1->GTCCRA = 0x500;
	R_GPT1->GTCCRC = 0x500;
	R_GPT1->GTCCRB = 0x500;
	R_GPT1->GTCCRE = 0x500;

	//Dead time setting
	R_GPT1->GTDTCR_b.TDE = 1;						// GTDVU and GTDVD registers are used to set the compare match value for

	//negative-phase waveform with dead time automatically in GTCCRB registe
	R_GPT1->GTDTCR_b.TDBUE = 0; 					// GTDVU register buffer operation is disabled
	R_GPT1->GTDTCR_b.TDBDE = 0; 					//GTDVD register buffer operation is disabled
	R_GPT1->GTDTCR_b.TDFER = 1; 					//The value written to GTDVU register is automatically set to GTDVD register
	R_GPT1->GTDVU = 0x320;							//dead time 2us

	//GPT2 congiguratin for phase V
	R_GPT2->GTWP = 0xA500;
	R_GPT2->GTCR = 0U;
	R_GPT2->GTST = 0U;
	R_GPT2->GTCNT = 0U;

	R_GPT2->GTSSR = 0x80000000U;					//Enables count start by the GTSTR register
	R_GPT2->GTPSR = 0x80000000U;					//Enables count stop by the GTSTP register
	R_GPT2->GTCSR = 0x80000000U;					//Enables counter clear by the GTCLR registe
	R_GPT2->GTICASR = 0;
	R_GPT2->GTICBSR = 0;

	R_GPT2->GTCR_b.CST = 0; 						//Count operation is stopped

	R_GPT2->GTCR_b.MD = 0x05;						//triangle-wave PWM mode 2
	R_GPT2->GTCR_b.TPCS = 0x00; 					//Core Clock/1//// GPT clock 400M
	R_GPT2->GTPR = 0x30D4;							//period--16K
	R_GPT2->GTPBR = 0x30D4; 						//period buffer

	R_GPT2->GTCNT = 0x00;							//CNT Clear;

	//IO function configuration
	R_GPT2->GTIOR_b.GTIOA = 0x03;					//initial low,	Output toggled at GTCCRA/GTCCRB compare match
	R_GPT2->GTIOR_b.GTIOB = 0x13;					//initial high,  Output toggled at GTCCRA/GTCCRB compare match
	R_GPT2->GTIOR_b.OAE = 1;						//GPTIOC0A output enable
	R_GPT2->GTIOR_b.OBE = 1;						//GPTIOC0B output enable

	//Buffer operation
	R_GPT2->GTBER_b.CCRA = 1;						//single buffer operation(GTCCRA <--> GTCCRC)
	R_GPT2->GTBER_b.CCRB = 1;						//single buffer operation(GTCCRB <--> GTCCRE)

	//Compare match value
	R_GPT2->GTCCRA = 0x1000;
	R_GPT2->GTCCRC = 0x1000;
	R_GPT2->GTCCRB = 0x1000;
	R_GPT2->GTCCRE = 0x1000;

	//Dead time setting
	R_GPT2->GTDTCR_b.TDE = 1;						// GTDVU and GTDVD registers are used to set the compare match value for

	//negative-phase waveform with dead time automatically in GTCCRB registe
	R_GPT2->GTDTCR_b.TDBUE = 0; 					// GTDVU register buffer operation is disabled
	R_GPT2->GTDTCR_b.TDBDE = 0; 					//GTDVD register buffer operation is disabled
	R_GPT2->GTDTCR_b.TDFER = 1; 					//The value written to GTDVU register is automatically set to GTDVD register
	R_GPT2->GTDVU = 0x320;							//dead time 2us

	//GPT3 congiguratin for phase W
	R_GPT3->GTWP = 0xA500;
	R_GPT3->GTCR = 0U;
	R_GPT3->GTST = 0U;
	R_GPT3->GTCNT = 0U;

	R_GPT3->GTSSR = 0x80000000U;					//Enables count start by the GTSTR register
	R_GPT3->GTPSR = 0x80000000U;					//Enables count stop by the GTSTP register
	R_GPT3->GTCSR = 0x80000000U;					//Enables counter clear by the GTCLR registe
	R_GPT3->GTICASR = 0;
	R_GPT3->GTICBSR = 0;

	R_GPT3->GTCR_b.CST = 0; 						//Count operation is stopped

	R_GPT3->GTCR_b.MD = 0x05;						//triangle-wave PWM mode 2
	R_GPT3->GTCR_b.TPCS = 0x00; 					//Core Clock/1
	R_GPT3->GTPR = 0x30D4;							//period
	R_GPT3->GTPBR = 0x30D4; 						//period buffer

	R_GPT3->GTCNT = 0x00;							//CNT Clear;

	//IO function configuration
	R_GPT3->GTIOR_b.GTIOA = 0x03;					//initial low,	Output toggled at GTCCRA/GTCCRB compare match
	R_GPT3->GTIOR_b.GTIOB = 0x13;					//initial high,  Output toggled at GTCCRA/GTCCRB compare match
	R_GPT3->GTIOR_b.OAE = 1;						//GPTIOC0A output enable
	R_GPT3->GTIOR_b.OBE = 1;						//GPTIOC0B output enable

	//Buffer operation
	R_GPT3->GTBER_b.CCRA = 1;						//single buffer operation(GTCCRA <--> GTCCRC)
	R_GPT3->GTBER_b.CCRB = 1;						//single buffer operation(GTCCRB <--> GTCCRE)

	//Compare match value
	R_GPT3->GTCCRA = 0x1500;
	R_GPT3->GTCCRC = 0x1500;
	R_GPT3->GTCCRB = 0x1500;
	R_GPT3->GTCCRE = 0x1500;

	//Dead time setting
	R_GPT3->GTDTCR_b.TDE = 1;						// GTDVU and GTDVD registers are used to set the compare match value for

	//negative-phase waveform with dead time automatically in GTCCRB registe
	R_GPT3->GTDTCR_b.TDBUE = 0; 					// GTDVU register buffer operation is disabled
	R_GPT3->GTDTCR_b.TDBDE = 0; 					//GTDVD register buffer operation is disabled
	R_GPT3->GTDTCR_b.TDFER = 1; 					//The value written to GTDVU register is automatically set to GTDVD register
	R_GPT3->GTDVU = 0x320;							//dead time 2us

	//interrupt enable
	R_GPT1->GTINTAD_b.GTINTPR = 1;
	/* Configure the interrupts */
	R_BSP_IrqDisable(VECTOR_NUMBER_GPT1_OVF);
	R_BSP_IrqCfg(VECTOR_NUMBER_GPT1_OVF, GPT1_OVERFLOW_PRIORITY_LEVEL, (NULL));
	R_BSP_IrqEnable(VECTOR_NUMBER_GPT1_OVF);

	//Start timer
	R_GPT1->GTCR_b.CST = 1;
	R_GPT2->GTCR_b.CST = 1;
	R_GPT3->GTCR_b.CST = 1;

	__asm volatile("cpsie i");
	__asm volatile("isb");
}


void R_GPT123_IO_int(void)
{
	R_BSP_PinAccessEnable();						// Unlock Register Write Protection

	//NONE SAFE 
	//	R_PTADR->RSELP_b[18].RS1 = 0;
	//	R_PTADR->RSELP_b[17].RS6 = 0;
	//phase U 
	R_PORT_SR->PMC_b[17].PMC6 = 1;					//GPIOC1A<-->P17-6
	R_PORT_SR->PFC_b[17].PFC6 = 0x01;				//GPIOC1A<-->P17-6
	R_PORT_SR->PMC_b[18].PMC1 = 1;					//GPIOC1B<-->P18-1
	R_PORT_SR->PFC_b[18].PFC1 = 0x02;				//GPIOC1B<-->P18-1

	//phase V 
	R_PORT_SR->PMC_b[17].PMC7 = 1;					//GPIOC2A<-->P17-7
	R_PORT_SR->PFC_b[17].PFC7 = 0x02;				//GPIOC2A<-->P17-7
	R_PORT_SR->PMC_b[18].PMC2 = 1;					//GPIOC2B<-->P18-2
	R_PORT_SR->PFC_b[18].PFC2 = 0x02;				//GPIOC2B<-->P18-2

	//phase W 
	R_PORT_SR->PMC_b[18].PMC0 = 1;					//GPIOC3A<-->P18-0
	R_PORT_SR->PFC_b[18].PFC0 = 0x02;				//GPIOC3A<-->P18-0
	R_PORT_SR->PMC_b[18].PMC3 = 1;					//GPIOC3B<-->P18-3
	R_PORT_SR->PFC_b[18].PFC3 = 0x03;				//GPIOC3B<-->P18-3


	R_BSP_PinAccessDisable();						// Lock Register Write Protection


}


void gpt_counter_overflow_isr(void)
{
//	R_PORT_SR->P[19] = (uint8_t) ((R_PORT_SR->P[19]) | (0x40));
	commutate_foc(&motor_1);
//	__NOP();
}






