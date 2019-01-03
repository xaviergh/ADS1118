# ADS1118
XC8 Library for the TI ADS1118 ADC

## Basic usage
Make sure the SPI is configured properly in the microcontroller.

Init the device:

    ads1118Init(ADS1118_AIN0_SINGLE_PGA1_CM_128SPS);

Read  AIN0 in mV:

    int16_t reading;
    reading =  ads1118ReadConversionmV(ADS1118_PGA_1, ADS1118_AIN0_SINGLE_PGA1_CM_128SPS);


<!--stackedit_data:
eyJoaXN0b3J5IjpbMTg0MTEzMDEwOSw5MDU0OTQ2NzYsLTExMD
Y5MTE1MTddfQ==
-->