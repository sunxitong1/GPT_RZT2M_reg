
/*********************************************************************************************************************
	**
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *------16-01-2023
 *------Winter Sun
 *********************************************************************************************************************
	*/
#include "GPT_int.h"
#include "hal_data.h"


void R_GPT123_Create_Complementary_mode(void)
{
	__asm volatile("cpsid i");					/* Disable IRQ interrupt (Set CPSR.I bit to 1)	*/
	__asm volatile("isb");

	R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
	R_BSP_MODULE_START(FSP_IP_GPT,1);
	R_BSP_MODULE_START(FSP_IP_GPT,2);
	R_BSP_MODULE_START(FSP_IP_GPT,3);
	R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

	/* Initialize all registers that may affect operation of this driver to reset values.  Skip these since they
	* affect all channels, and are initialized in GTCR and GTCNT: GTSTR, GTSTP, GTCLR. GTCR is set immediately after
	* clearing the module stop bit to ensure the timer is stopped before proceeding with configuration. */
	//GPT1 congiguratin for phase U
	R_GPT1->GTWP = 0xA500;
	R_GPT1->GTCR = 0U;
	R_GPT1->GTST = 0U;
	R_GPT1->GTCNT = 0U;

	R_GPT1->GTSSR = 0x80000000U;				//Enables count start by the GTSTR register
	R_GPT1->GTPSR = 0x80000000U;				//Enables count stop by the GTSTP register
	R_GPT1->GTCSR = 0x80000000U;				//Enables counter clear by the GTCLR registe
	R_GPT1->GTICASR = 0;
	R_GPT1->GTICBSR = 0;

	R_GPT1->GTCR_b.CST = 0; 					//Count operation is stopped

	R_GPT1->GTCR_b.MD = 0x05;					//triangle-wave PWM mode 2
	R_GPT1->GTCR_b.TPCS = 0x00; 				//Core Clock/1
	R_GPT1->GTPR = 0x3000;						//period
	R_GPT1->GTPBR = 0x3000; 					//period buffer

	R_GPT1->GTCNT = 0x00;						//CNT Clear;
	//IO function configuration
	R_GPT1->GTIOR_b.GTIOA = 0x03;				//initial low,	Output toggled at GTCCRA/GTCCRB compare match
	R_GPT1->GTIOR_b.GTIOB = 0x03;				//initial low,	Output toggled at GTCCRA/GTCCRB compare match
	R_GPT1->GTIOR_b.OAE = 1;					//GPTIOC0A output enable
	R_GPT1->GTIOR_b.OBE = 1;					//GPTIOC0B output enable

	//Buffer operation
	R_GPT1->GTBER_b.CCRA = 1;					//single buffer operation(GTCCRA <--> GTCCRC)
	R_GPT1->GTBER_b.CCRB = 1;					//single buffer operation(GTCCRB <--> GTCCRE)

	//Compare match value
	R_GPT1->GTCCRA = 0x2fff;
	R_GPT1->GTCCRC = 0x2fff;
	R_GPT1->GTCCRB = 0x2fff;
	R_GPT1->GTCCRE = 0x2fff;

	//Start timer
	R_GPT1->GTCR_b.CST = 1;


	__asm volatile("cpsie i");
	__asm volatile("isb");
}


void R_GPT1_Create_Sawtooth_mode(void)
{
	__asm volatile("cpsid i");					/* Disable IRQ interrupt (Set CPSR.I bit to 1)	*/
	__asm volatile("isb");

	R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
	R_BSP_MODULE_START(FSP_IP_GPT,1);
	R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

	/* Initialize all registers that may affect operation of this driver to reset values.  Skip these since they
	* affect all channels, and are initialized in GTCR and GTCNT: GTSTR, GTSTP, GTCLR. GTCR is set immediately after
	* clearing the module stop bit to ensure the timer is stopped before proceeding with configuration. */
	//GPT1 congiguratin for phase U
	R_GPT1->GTWP = 0xA500;
	R_GPT1->GTCR = 0U;
	R_GPT1->GTST = 0U;
	R_GPT1->GTCNT = 0U;

	R_GPT1->GTSSR = 0x80000000U;				//Enables count start by the GTSTR register
	R_GPT1->GTPSR = 0x80000000U;				//Enables count stop by the GTSTP register
	R_GPT1->GTCSR = 0x80000000U;				//Enables counter clear by the GTCLR registe
	R_GPT1->GTICASR = 0;
	R_GPT1->GTICBSR = 0;

	R_GPT1->GTCR_b.CST = 0; 					//Count operation is stopped

	R_GPT1->GTCR_b.MD = 0x00;					//sawtooth mode
	R_GPT1->GTCR_b.TPCS = 0x0A; 				//Core Clock/1024
	R_GPT1->GTPR = 9999;						//period
	R_GPT1->GTPBR = 9999;						//period buffer

	R_GPT1->GTCNT = 0x00;						//CNT Clear;
	//IO function configuration
	//	R_GPT1->GTIOR_b.GTIOA = 0x03;//initial low,  Output toggled at GTCCRA/GTCCRB compare match
	//initial high, 
	//high outpt at the end of a cycle, 
	//low output at compare match
	R_GPT1->GTIOR_b.GTIOB = 0x11;

	//	R_GPT1->GTIOR_b.OAE = 1;					//GPTIOC0A output enable
	R_GPT1->GTIOR_b.OBE = 1;					//GPTIOC0B output enable

	//Buffer operation
	//	R_GPT1->GTBER_b.CCRA = 1;					//single buffer operation(GTCCRA <--> GTCCRC)
	R_GPT1->GTBER_b.CCRB = 1;					//single buffer operation(GTCCRB <--> GTCCRE)
	R_GPT1->GTBER_b.PR = 1; 					// Single buffer operation (GTPBR register → GTPR register)

	//Compare match value

	R_GPT1->GTCCRB = 9999;
	R_GPT1->GTCCRE = 9999;

	//Start timer
	R_GPT1->GTCR_b.CST = 1;


	__asm volatile("cpsie i");
	__asm volatile("isb");
}


void R_GPT123_IO_int(void)
{
	R_BSP_PinAccessEnable();					// Unlock Register Write Protection

	//NONE SAFE 
	//	R_PTADR->RSELP_b[18].RS1 = 0;
	//	R_PTADR->RSELP_b[17].RS6 = 0;
	R_PORT_SR->PMC_b[17].PMC6 = 1;				//GPIOC1A<-->P17-6
	R_PORT_SR->PFC_b[17].PFC6 = 0x01;			//GPIOC1A<-->P17-6
	R_PORT_SR->PMC_b[18].PMC1 = 1;				//GPIOC1B<-->P18-1
	R_PORT_SR->PFC_b[18].PFC1 = 0x02;			//GPIOC1B<-->P18-1

	R_BSP_PinAccessDisable();					// Lock Register Write Protection


}


