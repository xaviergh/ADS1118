#include "ADS1118.h"

/*
	Max Speed: 4MHz
	Holding SCLK low longer than 28ms resets the SPI interface
	SCLK Idle LOW
	Data valid on the SCLK falling Edge
	SPI Mode 1
*/
void ads1118Init(uint16_t configRegister)
{
	ADS1118_CS_HIGH

	#ifndef __XC8
    setup_spi(SPI_MASTER|SPI_SCK_IDLE_LOW|SPI_XMIT_H_TO_L|SPI_CLK_DIV_16);
    #endif

	ads1118Config(configRegister);
}

/*
	Config Register

	B15: SS - 1 Start a single conversion
	B14-B12: Muxer config
	B11-B9:  PGA config
	B8: Conversion Mode 0: Continuous - 1: Single-shot
	B7-B5: Data Rate
	B4: TS_MODE 0: ADC Mode (default) 1: Temp sensor mode
	B3: Pull-up enable 0: Pull-up on DOUT disabled 1: Pull-up enabled (default)
	B2-1: No Operation, to write the config register write "01"
	B0: Not used
*/
void ads1118Config(uint16_t configRegister)
{
	uint8_t dummy;
    ADS1118_CS_LOW
	dummy = spi_write(configRegister >> 8);
	dummy = spi_write(configRegister);
//	dummy = spi_write(0xC2);
//	dummy = spi_write(0x8A);
	ADS1118_CS_HIGH
}

uint16_t ads1118ReadConversionRaw(uint16_t nextConfReg)
{
	
	// int16_t adcValue;

	// ADS1118_CS_LOW
	// while(ADS1118_GET_SDO);
	// adcValue = spi_read(0) << 8;
	// adcValue |= spi_read(0);
	// ADS1118_CS_HIGH

	// return adcValue;

	int16_t adcValue;
	uint8_t adcValueMSB, adcValueLSB;

	ADS1118_CS_LOW
	while(ADS1118_GET_SDO);
	adcValueMSB = spi_read(nextConfReg >> 8);
	adcValueLSB = spi_read(nextConfReg);
	ADS1118_CS_HIGH

	//adcValue = make16(adcValueMSB,adcValueLSB);
	adcValue = (adcValueMSB<<8) | adcValueLSB;

	return adcValue;
}

int16_t ads1118ReadConversionRawSS(uint16_t nextConfReg)
{
	
	int16_t adcValue;
	uint8_t adcValueMSB, adcValueLSB;

	ADS1118_CS_LOW
	while(ADS1118_GET_SDO);
	// adcValue = spi_read(nextConfReg >> 8) << 8;
	// adcValue |= spi_read(nextConfReg);
	//0xC28A
	// adcValueMSB = spi_read(nextConfReg >> 8);
	// adcValueLSB = spi_read(nextConfReg);
	adcValueMSB = spi_read(0xC3);
	adcValueLSB = spi_read(0x8B);
	ADS1118_CS_HIGH

	adcValue = (adcValueMSB <<8 ) | adcValueLSB;

	return adcValue;
}

float ads1118ReadConversionFloat(uint8_t pgaGain, uint16_t nextConfReg)
{
	
	//TODO: Add Twos Complement conversion
	//for negative values.

	int16_t adcValue;
	uint8_t adcValueMSB, adcValueLSB;

	ADS1118_CS_LOW
	while(ADS1118_GET_SDO);
	adcValueMSB = spi_read(nextConfReg >> 8);
	adcValueLSB = spi_read(nextConfReg);
	ADS1118_CS_HIGH

	//adcValue = make16(adcValueMSB,adcValueLSB);
	adcValue = (adcValueMSB<<8) | adcValueLSB;
	// adcValue = adcValue >> 4; //reduce precision

	//printf("ADC Raw: %LX\n",adcValue);
	// printf("ADC Twos: %LX\n",(~adcValue)+1);

	// if (bit_test(adcValue,16)){
	// 	adcValue = (~adcValue)+1;
	// 	adcValue >>= 2; //turn right justified
	// 	return adcValue*ADC_TEMP_CONV*(-1);
	// }
	// else{
	// 	adcValue >>= 2; //turn right justified
	// 	return adcValue*ADC_TEMP_CONV;
	// }

	switch(pgaGain)
	{
		case ADS1118_PGA_23:
			return (float)adcValue*(ADS1118_PGA23_FS/ADS1118_ADC_CONV);
		break;

		case ADS1118_PGA_1:
			// return (float)adcValue*(ADS1118_PGA1_FS/ADS1118_ADC_CONV);
			return (float)(adcValue*ADS1118_PGA1_RATIO);
		break;

		case ADS1118_PGA_2:
			return (float)adcValue*(ADS1118_PGA2_FS/ADS1118_ADC_CONV);
		break;

		case ADS1118_PGA_4:
			return (float)adcValue*(ADS1118_PGA4_FS/ADS1118_ADC_CONV);
		break;

		case ADS1118_PGA_8:
			return (float)adcValue*(ADS1118_PGA8_FS/ADS1118_ADC_CONV);
		break;

		case ADS1118_PGA_16:
			return (float)adcValue*ADS1118_PGA16_RATIO;
		break;
	}
}

int16_t ads1118ReadConversionmV(uint8_t pgaGain, uint16_t nextConfReg)
{
	
	//TODO: Add Twos Complement conversion
	//for negative values.
	//TODO: Add factor for all the gains

	int16_t adcValue;
	uint8_t adcValueMSB, adcValueLSB;

	ADS1118_CS_LOW
	while(ADS1118_GET_SDO);
	adcValueMSB = spi_read(nextConfReg >> 8);
	adcValueLSB = spi_read(nextConfReg);
	ADS1118_CS_HIGH

	adcValue = (adcValueMSB<<8) | adcValueLSB;

	// printf("R%u\n", adcValue);

	switch(pgaGain)
	{
		case ADS1118_PGA_23:
			adcValue = (int16_t)ceil((double)adcValue*(ADS1118_PGA23_FS/ADS1118_ADC_CONV));
		break;

		case ADS1118_PGA_1:
			// return (float)adcValue*(ADS1118_PGA1_FS/ADS1118_ADC_CONV);
			adcValue = (int16_t)ceil((double)(adcValue)*ADS1118_PGA1_RATIO);
		break;

		case ADS1118_PGA_2:
			adcValue = (int16_t)ceil((double)adcValue*(ADS1118_PGA2_FS/ADS1118_ADC_CONV));
		break;

		case ADS1118_PGA_4:
			adcValue = (int16_t)ceil((double)adcValue*(ADS1118_PGA4_FS/ADS1118_ADC_CONV));
		break;

		case ADS1118_PGA_8:
			adcValue = (int16_t)ceil((double)adcValue*(ADS1118_PGA8_FS/ADS1118_ADC_CONV));
		break;

		case ADS1118_PGA_16:
			adcValue = (int16_t)ceil((double)(adcValue)*ADS1118_PGA16_RATIO);
		break;
	}

	return adcValue;
}

float ads1118GetAvgFloat(uint8_t pgaGain, uint16_t nextConfReg, uint8_t avg_count)
{
	float avg_float_val = 0;

	for (uint8_t i = 0; i < avg_count; ++i)
	{
		avg_float_val += ads1118ReadConversionFloat(pgaGain, nextConfReg);
	}

	return (avg_float_val/avg_count);
}

int16_t ads1118GetAvgInt(uint8_t pgaGain, uint16_t nextConfReg, uint8_t avg_count)
{
	float avg_float_val = 0;

	for (uint8_t i = 0; i < avg_count; ++i)
	{
		avg_float_val += ads1118ReadConversionmV(pgaGain, nextConfReg);
	}

	return (avg_float_val/avg_count);
}


float ads1118ReadTemperature(uint16_t nextConfReg)
{

	/*This asumes ads1118Config 
	  was called before to configure 
	  the device.

	  The result a 14bit left justified

	  TODO: Add negatives values support
	*/

	int16_t adcValue;
	uint8_t adcValueMSB, adcValueLSB;

	ADS1118_CS_LOW
	while(ADS1118_GET_SDO);
	adcValueMSB = spi_read(nextConfReg >> 8);
	adcValueLSB = spi_read(nextConfReg);
	// adcValue = spi_read(0) << 8;
	// adcValue |= spi_read(0);
	ADS1118_CS_HIGH

	// adcValue = make16(adcValueMSB,adcValueLSB);
	adcValue = (adcValueMSB<<8) | adcValueLSB;

    //see if the value is negative
    if (bit_test(adcValue,16))
    {
		bit_clear(adcValue,16); //clear the sign bit
		//the value from the ADC is 14bit so move 2 positions
		adcValue >>= 2; //turn right justified
		bit_set(adcValue, 16); //set the sign bit again
	}
	else
	{
		//the value is not negative so just shit 2 pos to make it 14 bit
		adcValue >>= 2; //turn right justified
	}

	return adcValue*ADS1118_ADC_TEMP_CONV;

	// if (bit_test(adcValue,16)){
	// 	adcValue = (~adcValue)+1;
	// 	adcValue >>= 2; //turn right justified
	// 	return adcValue*ADC_TEMP_CONV*(-1);
	// }
	// else{
	// 	adcValue >>= 2; //turn right justified
	// 	return adcValue*ADC_TEMP_CONV;
	// }
	//printf("ADC Raw: %Lu\n",adcValue);
}