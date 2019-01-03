/*! 
* @file ads1118.h
*
* @brief ADS1118 Driver
*
* @author Xavier Garcia Herrera (xavier.garciah@gmail.com).
*
* @par
* Created: 2014
* Rev: 2017
*/
#ifndef _ADS1118_H
#define _ADS1118_H


#include <stdint.h> //generic typedefs
#include <stdbool.h> //bool typedef
#include <math.h>
#include "../mcc_generated_files/mcc.h"


//uses SPI2, change accordingly
#if defined(__XC8)
#define spi_write 	SPI2_Exchange8bit
#define spi_read	SPI2_Exchange8bit
#define delay_ms	__delay_ms
#define ADS1118_CS_HIGH	ADC_CS_SetHigh();
#define ADS1118_CS_LOW	ADC_CS_SetLow();
#define ADS1118_GET_SDO	SDI2_GetValue()


#define bit_test(x,n) (x & (0x01<<n))
#define bit_set(x,n) (x=x | (0x01<<n))
#define bit_clear(x,n) (x=x & ~(0x01<<n))


#endif

#ifndef __XC8 //using CSS Compiler

#ifndef ADS1118_CS
#define ADS1118_CS	PIN_C0
#endif

#ifndef ADS1118_DOUT
#define ADS1118_DOUT    PIN_D1
#endif

#define ADS1118_CS_HIGH	output_high(ADS1118_CS);
#define ADS1118_CS_LOW	output_low(ADS1118_CS);
#define ADS1118_GET_SDO	input_state(ADS1118_DOUT);

#endif
/*! @name ADS1118 definitions
** @{
*/
/*!
	Config Register

	B15: SS - Single Shot
	B14: MUX2
	B13: MUX1
	B12: MUX0 
	B11: PGA2 
	B10: PGA1
	B09: PGA0
	B08: MODE
	B07: DR2 - Data Rate
	B06: DR1
	B05: DR0
	B04: TS_MODE
	B03: PULL_UP_EN
	B02: NOP1
	B01: NOP0
	B00: Not Used
*/

#define ADS1118_REG_BIT_SS      	15
#define ADS1118_REG_BIT_MUX2		14
#define ADS1118_REG_BIT_MUX1		13
#define ADS1118_REG_BIT_MUX0		12
#define ADS1118_REG_BIT_PGA2    	11
#define ADS1118_REG_BIT_PGA1    	10
#define ADS1118_REG_BIT_PGA0    	9
#define ADS1118_REG_BIT_MODE    	8
#define ADS1118_REG_BIT_DR2     	7
#define ADS1118_REG_BIT_DR1     	6
#define ADS1118_REG_BIT_DR0     	5
#define ADS1118_REG_BIT_TS_MODE 	4
#define ADS1118_REG_BIT_PULL_UP 	3
#define ADS1118_REG_BIT_NOP1    	2
#define ADS1118_REG_BIT_NOP0    	1

/*!
	Configurations
*/
#define ADS1118_CONF_START_SINGLE_CONV		(1<<ADS1118_REG_BIT_SS)
#define	ADS1118_CONF_AIN0_AIN1_DIFF			0
#define	ADS1118_CONF_AIN0_AIN3_DIFF			(1<<ADS1118_REG_BIT_MUX0)
#define	ADS1118_CONF_AIN1_AIN3_DIFF			(2<<ADS1118_REG_BIT_MUX0)
#define	ADS1118_CONF_AIN2_AIN3_DIFF			(3<<ADS1118_REG_BIT_MUX0)
#define	ADS1118_CONF_AIN0_SINGLE			(4<<ADS1118_REG_BIT_MUX0)
#define	ADS1118_CONF_AIN1_SINGLE			(5<<ADS1118_REG_BIT_MUX0)
#define	ADS1118_CONF_AIN2_SINGLE			(6<<ADS1118_REG_BIT_MUX0)
#define	ADS1118_CONF_AIN3_SINGLE			(7<<ADS1118_REG_BIT_MUX0)
/*!
	PGA Gain

	Setting		FS(V)
	2/3         +/- 6.144
	1           +/- 4.096
	2           +/- 2.048
	4           +/- 1.024
	8           +/- 0.512
	16          +/- 0.256
*/
#define ADS1118_CONF_PGA_GAIN_2_3    (0<<ADS1118_REG_BIT_PGA0)
#define ADS1118_CONF_PGA_GAIN_1      (1<<ADS1118_REG_BIT_PGA0)
#define ADS1118_CONF_PGA_GAIN_2      (2<<ADS1118_REG_BIT_PGA0)
#define ADS1118_CONF_PGA_GAIN_4      (3<<ADS1118_REG_BIT_PGA0)
#define ADS1118_CONF_PGA_GAIN_8      (4<<ADS1118_REG_BIT_PGA0)
#define ADS1118_CONF_PGA_GAIN_16     (7<<ADS1118_REG_BIT_PGA0)
#define ADS1118_CONF_CONT_MODE       0
#define ADS1118_CONF_SINGLE_MODE     (1<<ADS1118_REG_BIT_MODE)
#define ADS1118_CONF_DR_8SPS         0
#define ADS1118_CONF_DR_16SPS        (1<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_DR_32SPS        (2<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_DR_64SPS        (3<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_DR_128SPS       (4<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_DR_250SPS       (5<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_DR_475SPS       (6<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_DR_860SPS       (7<<ADS1118_REG_BIT_DR0)
#define ADS1118_CONF_ADC_MODE        0
#define ADS1118_CONF_TEMP_MODE       (1<<ADS1118_REG_BIT_TS_MODE)
#define ADS1118_CONF_ENABLE_PULL_UP  (1<<ADS1118_REG_BIT_PULL_UP)
#define ADS1118_CONF_UPDATE_CONFIG   (1<<ADS1118_REG_BIT_NOP0)

/*!
	ADS1118 Conversion Constants
*/
#define ADS1118_ADC_CONV      	32768.0 //2^15
#define ADS1118_ADC_TEMP_CONV 	0.03125
#define ADS1118_PGA23_FS      	6.144
#define ADS1118_PGA1_FS       	4.096
#define ADS1118_PGA2_FS       	2.048
#define ADS1118_PGA4_FS       	1.024
#define ADS1118_PGA8_FS       	0.512
#define ADS1118_PGA16_FS      	0.256
#define ADS1118_PGA_23        	0
#define ADS1118_PGA_1         	1
#define ADS1118_PGA_2         	2
#define ADS1118_PGA_4         	3
#define ADS1118_PGA_8         	4
#define ADS1118_PGA_16        	5
#define ADS1118_PGA16_RATIO   	7.8 //7.8uV
#define ADS1118_PGA1_RATIO   	0.125 //125uV
// #define ADS1118_PGA1_RATIO   	0.000125 //125uV
// #define ADS1118_PGA1_RATIO   	125.0 //125uV

/*!
	Common Configurations
*/
#define ADS1118_TC1_8SPS_PGA1_SS  		0x830A //Pull-Up enable starts ss
#define ADS1118_TC1_8SPS_PGA16_SS 		0x8F0A //Pull-Up enable starts ss
#define ADS1118_TC1_8SPS_PGA16_NOSS 	0x0F0A //Pull-Up enable starts ss
#define ADS1118_TC2_8SPS_PGA1_SS  		0xB30A
#define ADS1118_TEMP_MODE_DEF        	0x859A
#define ADS1118_TEMP_MODE_DEF_NOSS   	0x059A
//ADS1118_CONF_AIN0_SINGLE | ADS1118_CONF_PGA_GAIN_1 
// | ADS1118_CONF_SINGLE_MODE | ADS1118_CONF_DR_128SPS 
// | ADS1118_CONF_ADC_MODE | ADS1118_CONF_ENABLE_PULL_UP | ADS1118_CONF_UPDATE_CONFIG
#define ADS1118_AIN0_SINGLE_PGA1_SS_128SPS		0xC38B //4.096 FS
#define ADS1118_AIN0_SINGLE_PGA1_CM_128SPS		0xC28B //4.096 FS Continuos Mode Pull up enabled
#define ADS1118_AIN0_SINGLE_PGA1_CM_064SPS		0xC26B //4.096 FS Continuos Mode Pull up enabled
#define ADS1118_AIN1_SINGLE_PGA1_CM_128SPS		0xD28B //4.096 FS Continuos Mode Pull up enabled
#define ADS1118_AIN2_SINGLE_PGA1_CM_128SPS		0xE28B //4.096 FS Continuos Mode Pull up enabled
#define ADS1118_AIN3_SINGLE_PGA1_CM_128SPS		0xF28B //4.096 FS Continuos Mode Pull up enabled
#define ADS1118_AIN0_SINGLE_PGA1_CM_250SPS		0xC2AA //4.096 FS Continuos Mode Pull up enabled
#define ADS1118_AIN0_SINGLE__NPU_PGA1_SS_128SPS	0xC382 //4.096 FS No Pull-Up
#define ADS1118_AIN1_SINGLE_PGA1_SS_128SPS		0xD38A
#define ADS1118_AIN2_SINGLE_PGA1_SS_128SPS		0xE38A
#define ADS1118_AIN3_SINGLE_PGA1_SS_128SPS		0xF38A

void ads1118Init(uint16_t configRegister);
void ads1118Config(uint16_t configRegister);
//uint16_t ads1118ReadConversionRaw(void);
uint16_t ads1118ReadConversionRaw(uint16_t nextConfReg);
int16_t ads1118ReadConversionRawSS(uint16_t nextConfReg);
//float ads1118ReadConversionFloat(int8 pgaGain);
float ads1118ReadConversionFloat(uint8_t pgaGain, uint16_t nextConfReg);
int16_t ads1118ReadConversionmV(uint8_t pgaGain, uint16_t nextConfReg);
//float ads1118ReadTemperature(void);
float ads1118GetAvgFloat(uint8_t pgaGain, uint16_t nextConfReg, uint8_t avg_count);
int16_t ads1118GetAvgInt(uint8_t pgaGain, uint16_t nextConfReg, uint8_t avg_count);
float ads1118ReadTemperature(uint16_t nextConfReg);

#endif /* _ADS1118_H */