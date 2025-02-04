 /******************************************************************************
 * Module: ADC
 * File Name: adc.h
 * Description: Source file for the ADC driver
 * Author: Yousif Adel
 *******************************************************************************/
#include "adc.h"
#include "common_macros.h"
#include "avr/io.h" /* To use the ADC Registers */


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* OLD way */
	//void ADC_init(void)
	//{
	//
	//	/* 1. activate AREF -> REFS1 = 0   ,  REFS0 = 0 [default] */
	//	/* 2. activate Channel 0   [do nothing cause the default is channel 0]
	//	 * ADMUX = 0 [default]*/
	//	ADC_ADMUX_REG.Byte = 0;
	//	/* 3. enable The ADC */
	//	ADC_ADCSRA_REG.Bits.ADEN_Bit = 1;
	//	/* 4. Disable ADC interrupt [do nothing cause the default is disabled]*/
	//	/* 5. Choose the Pre-Scalar
	//	 * choose N = 8  =>		ADPS1 = 1  ,ADPS0 = 1	*/
	//	ADC_ADCSRA_REG.Bits.ADPS1_Bit = 1;
	//	ADC_ADCSRA_REG.Bits.ADPS0_Bit = 1;
	//
	//}

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(const ADC_ConfigType *Config_Ptr)
{
	/* 1. activate Internal -> REFS1 = 1   ,  REFS0 = 1 */
	ADC_ADMUX_REG.Byte = (ADC_ADMUX_REG.Byte & 0x3F) | ((Config_Ptr->ref_volt)<<6);


	/* 2. activate Channel 0   [do nothing cause the default is channel 0]
	 * ADMUX = 0 [default]*/

	/* 3. enable The ADC */
	ADC_ADCSRA_REG.Bits.ADEN_Bit = 1;

	/* 4. Disable ADC interrupt [do nothing cause the default is disabled]*/

	/* 5. Choose the Pre-Scalar
	 * choose N = 8  =>		ADPS1 = 1  ,ADPS0 = 1	*/
	ADC_ADCSRA_REG.Byte = (ADC_ADCSRA_REG.Byte & 0xF8) | ((Config_Ptr->prescaler)&0x07);
}

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */

uint16 ADC_readChannel(uint8 channel_num)
{
	/* 1. Insert the channel number in ADMUX (channel number is only 3bits cause the maximum value is 111) */

	 	 	 /* First Way*/
			ADC_ADMUX_REG.Bits.MUX0_Bit = GET_BIT(channel_num,0);
			ADC_ADMUX_REG.Bits.MUX1_Bit = GET_BIT(channel_num,1);
			ADC_ADMUX_REG.Bits.MUX2_Bit = GET_BIT(channel_num,2);
			/*
		  	 // Second Way
		// ADMUX = (ADMUX & 11100000) | (ch_num & 0b00000111);
			 // Third way using Mask
			ADC_ADMUX_REG.Byte = (ADC_ADMUX_REG.Byte & 0xE0) | (channel_num & 0x07);
	(OR)	ADC_ADMUX_REG.Byte = (ADC_ADMUX_REG.Byte & 0xE0) | channel_num;

		 */

	/* 2. start conversion [set ADSC = 1]*/
	ADC_ADCSRA_REG.Bits.ADSC_Bit = 1;

	/* 3. Polling until the ADIF = 1
	 * [waits until ADIF = 1]*/
	while(ADC_ADCSRA_REG.Bits.ADIF_Bit == 0);

	/* 4. Clear ADIF Flag by set its value = 1 */
	ADC_ADCSRA_REG.Bits.ADIF_Bit = 1;

	/* 5. Read the data [return ADC]*/
	return ADC_REG;
}


