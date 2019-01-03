# ADS1118
XC8 Library for the TI ADS1118 ADC

## Basic usage
Make sure the SPI is configured properly in the microcontroller.

Init the device:

    ads1118Init(ADS1118_AIN0_SINGLE_PGA1_CM_128SPS);

Read  in AIN0
int
reading =  ads1118ReadConversionmV(ADS1118_PGA_1, ADS1118_AIN0_SINGLE_PGA1_CM_128SPS);

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE4NzQ4MjkzODMsOTA1NDk0Njc2LC0xMT
A2OTExNTE3XX0=
-->