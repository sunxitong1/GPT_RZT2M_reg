
/*********************************************************************************************************************
	**
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *------13-02-2023
 *------Winter Sun
 *********************************************************************************************************************
	*/
#include "POEG_reg.h"
#include "hal_data.h"

void R_POEG_Create(void)
{
	__asm volatile("cpsid i");						/* Disable IRQ interrupt (Set CPSR.I bit to 1)	*/
	__asm volatile("isb");

//	R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
//	R_BSP_MODULE_START(FSP_IP_POEG, 1);
//	R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

	R_POEG0->POEG0GA_b.PIDE = 1;		///Detection of input levels on the corresponding GTETRGA pin is enabled
	R_POEG0->POEG0GA_b.INV = 0; 		//Input on the GTETRGn pin is inverted
	
	R_POEG0->POEG0GA_b.IOCE = 1;		// Detection of stopping of output from the GPT is enabled
	__asm volatile("cpsie i");
	__asm volatile("isb");
}


void R_POEG_IO_int(void)
{
	R_PORT_SR->PMC_b[17].PMC3 = 1;					//GTETRGA<-->P17-3
	R_PORT_SR->PFC_b[17].PFC3 = 0x01;				//GTETRGA<-->P17-3

}


