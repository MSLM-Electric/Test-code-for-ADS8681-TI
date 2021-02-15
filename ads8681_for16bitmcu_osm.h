/* 
 * File:   ads8681_for16bitmcu_osm.h
 * Author: abdulhamidov
 *
 * Created on 7 августа 2020 г., 17:10
 */

#ifndef ADS8681_FOR16BITMCU_OSM_H
#define	ADS8681_FOR16BITMCU_OSM_H

#include "spi_dspic_osm.h"
//#include <xc.h>
//#include <libpic30.h>

//---------------------------------//My macros//---------------------------------/
#define DO_CS_ads86 LATBbits.LATB2 
#define ENABLE_ADS86_CSlow() DO_CS_ads86 = 0;
#define DISABLE_ADS86_CShigh() DO_CS_ads86 = 1;
//---------------------------------//---------//---------------------------------/

/*
00h DEVICE_ID_REG     Device ID register
04h RST_PWRCTL_REG    Reset and power control register
08h SDI_CTL_REG       SDI data input control register
0Ch SDO_CTL_REG       SDO-x data input control register
10h DATAOUT_CTL_REG   Output data control register
14h RANGE_SEL_REG     Input range selection control register
20h ALARM_REG         ALARM output register
24h ALARM_H_TH_REG    ALARM high threshold and hysteresis register
28h ALARM_L_TH_REG    ALARM low threshold register 
 */
/********List of addresses Commands register*********/
#define DEVICE_ID_REG    0x00
#define RST_PWRCTL_REG   0x04
#define SDI_CTL_REG      0x08
#define SDO_CTL_REG      0x0C
#define DATAOUT_CTL_REG  0x10
#define RANGE_SEL_REG    0x14
#define ALARM_REG        0x20
#define ALARM_H_TH_REG   0x24
#define ALARM_L_TH_REG   0x28

/********List of Input Commands*********/
#define CLEAR_HWORD 0b11000000
#define READ_HWORD  0b11001000
#define READ        0b01001000
#define WRITE       0b11010000
#define WRITE_MSB   0b11010010   //Most Significant Bit  0x80EA  0x80 MSB(8bit)
#define WRITE_LSB   0b11010100   //Least Significant Bit 0x80EA  0xEA LSB(8bit)
#define SET_HWORD   0b11011000
/***************page 43*****************/


unsigned long ADS86_readID();
int ADS86_testRead();
unsigned long ADS86_testReadX4();
unsigned long ADS86_testReadWithCMD(unsigned char cmd, unsigned char address);
unsigned int ADS86_WriteMSBcmd(unsigned char cmdREG_address, unsigned int data);
unsigned int ADS86_WriteLSBcmd(unsigned char cmdREG_address, unsigned int data);
unsigned long ADS86_WriteCMD(unsigned char cmd, unsigned char address);
unsigned int ADS86_WriteHWORD(unsigned char cmdREG_address, unsigned int data);


#ifdef	__cplusplus
extern "C" {
#endif



#ifdef	__cplusplus
}
#endif

#endif	/* ADS8681_FOR16BITMCU_OSM_H */
//*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*/
