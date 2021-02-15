 /*
 * Author          : MSLM Electric
 * Company         : & Engineering
 * Project         : TI ADC ADS8681 Library for MPLAB XC16 
 * Microcontroller : dsPIC 33FJ128GP802
 * Created on August 12, 2020, 10:18 
 */

#include "ads8681_for16bitmcu_osm.h"



/*******************************************************************************
* Function Name  : 
* Description    : Read from SPI interface 
* Input          : 8bit input
* Output         : 
* Return         : 8bit output
* Example        : 
*******************************************************************************/
unsigned long ADS86_readID()
{
    unsigned long longbuffer;
    ENABLE_ADS86_CSlow();
    unsigned char buffer[4];
    osm_WriteSPI1(0b11001000);
    osm_WriteSPI1(DEVICE_ID_REG);
    buffer[0] = osm_ReadSPI1dummy(0);
    buffer[1] = osm_ReadSPI1dummy(0);
    osm_WriteSPI1(0b11001000);
    osm_WriteSPI1(DEVICE_ID_REG + 2);
    buffer[2] = osm_ReadSPI1dummy(0);
    buffer[3] = osm_ReadSPI1dummy(0);
    DISABLE_ADS86_CShigh();
    longbuffer = (uint32_t)(buffer[3] << 24) | (uint32_t)(buffer[2] << 16) | (uint16_t)(buffer[1] << 8) | (uint8_t)buffer[0];
    return longbuffer;
}
//ñì. 46-óþ ñòðàíèöó ADS8688 datasheet


int ADS86_testRead()
{
    int buffer;
    ENABLE_ADS86_CSlow();
    buffer = osm_ReadSPI1();
    buffer |= osm_ReadSPI1() << 8;
    DISABLE_ADS86_CShigh();
    return buffer;
}

unsigned long ADS86_testReadX4()
{
    unsigned long longbuffer;
    unsigned char buffer[4];
    ENABLE_ADS86_CSlow();
    //asm("nop"); //wait for tconv_max time
    buffer[0] = osm_ReadSPI1dummy(0);
    buffer[1] = osm_ReadSPI1dummy(0);
    buffer[2] = osm_ReadSPI1dummy(0);
    buffer[3] = osm_ReadSPI1dummy(0);
    longbuffer = (uint32_t)(buffer[3] << 24) | (uint32_t)(buffer[2] << 16) | (uint16_t)(buffer[1] << 8) | (uint8_t)buffer[0]; //Âîò òàê âîò íàäî âûêîâûðèâàòüñÿ. Õîòÿ â C# òîæå òàê.
    DISABLE_ADS86_CShigh();
    return longbuffer;
}

/*******************************************************************************
* Function Name  : 
* Description    : Read from SPI interface 
* Input          : 8bit input
* Output         : 
* Return         : 32bit output
* Example        : ADS86_testReadWithCMD(READ_HWORD, ALARM_H_TH_REG);
*******************************************************************************/
unsigned long ADS86_testReadWithCMD(unsigned char cmd, unsigned char address)
{
    unsigned long longbuffer;
    ENABLE_ADS86_CSlow();
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    unsigned char buffer[8];
    osm_WriteSPI1(cmd);                 //0b11001xx0 - READ_HWORD for example. See the list of definition input commands
    osm_WriteSPI1(address);
    osm_WriteSPI1(0);
    osm_WriteSPI1(0);
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    DISABLE_ADS86_CShigh();
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    ENABLE_ADS86_CSlow();    
//    osm_WriteSPI1(cmd);                 
//    osm_WriteSPI1(address+2);    
    buffer[3] = osm_ReadSPI1dummy(0);   //In here I get sample code
    buffer[2] = osm_ReadSPI1dummy(0);   //In here I get sample code
    buffer[1] = osm_ReadSPI1dummy(0);   //In here I get nothing
    buffer[0] = osm_ReadSPI1dummy(0);   //In here I get nothing
    
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    DISABLE_ADS86_CShigh();
    longbuffer = (unsigned long)(buffer[3]) << 24;
    longbuffer |= (unsigned long)(buffer[2]) << 16;
    longbuffer |= (unsigned long)(buffer[1]) << 8;
    longbuffer |= (unsigned long)(buffer[0]);      
    return longbuffer;
}

/*******************************************************************************
* Function Name  : 
* Description    : Write only the MSB(16bit H) of 32bit command register from SPI interface to ADC86
* Input          : 8bit Command (CMD) address which is already defined, see the list of cmds in .h, 16bit data input 
* Output         : 16bit (or 32bit?)
* Return         : Xbit output
* Example        : ADS86_WriteMSBcmd(ALARM_REG, 0x0001);
*******************************************************************************/
unsigned int ADS86_WriteMSBcmd(unsigned char cmdREG_address, unsigned int data)
{
    unsigned int buffer;
    ENABLE_ADS86_CSlow();
    osm_WriteSPI1(WRITE_MSB);
    osm_WriteSPI1((cmdREG_address + 2));
    buffer = (uint8_t)(osm_ReadSPI1dummy(data >> 8)) << 8;     //((uint8_t)(data >> 8));
    buffer |= (uint8_t)(osm_ReadSPI1dummy(data & 0xFF)) & 0x00FF; //((uint8_t)(data & 0x00FF));
    DISABLE_ADS86_CShigh();
    return buffer;
}

unsigned int ADS86_WriteLSBcmd(unsigned char cmdREG_address, unsigned int data)
{
    unsigned int buffer;
    ENABLE_ADS86_CSlow();
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    osm_WriteSPI1(WRITE_LSB);
    osm_WriteSPI1(cmdREG_address);
    buffer = (uint8_t)(osm_ReadSPI1dummy(data >> 8)) << 8;     //((uint8_t)(data >> 8));
    buffer |= (uint8_t)(osm_ReadSPI1dummy(data & 0xFF)) & 0x00FF; //((uint8_t)(data & 0x00FF));
    //del then
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    DISABLE_ADS86_CShigh();
    return buffer;
}

unsigned int ADS86_WriteHWORD(unsigned char cmdREG_address, unsigned int data)
{
    unsigned int buffer;
    ENABLE_ADS86_CSlow();
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    osm_WriteSPI1(WRITE);
    osm_WriteSPI1(cmdREG_address);
    buffer = (uint8_t)(osm_ReadSPI1dummy(data >> 8)) << 8;     //((uint8_t)(data >> 8));
    buffer |= (uint8_t)(osm_ReadSPI1dummy(data & 0xFF)) & 0x00FF; //((uint8_t)(data & 0x00FF));
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
    DISABLE_ADS86_CShigh();
    return buffer;
}


unsigned long ADS86_WriteCMD(unsigned char cmd, unsigned char address)
{
    ENABLE_ADS86_CSlow();
    osm_WriteSPI1(cmd);  
    
}



//unsigned long ADS86_WriteMSBcmd(unsigned char cmd_address, unsigned int data)
//{
//    ENABLE_ADS86_CSlow();
//    osm_WriteSPI1(WRITE_MSB);
//    osm_WriteSPI1(cmd_address);
//    osm_WriteSPI1(data >> 8);   //(uint8_t(data >> 8));
//    osm_WriteSPI1(data & 0x00FF);
//    DISABLE_ADS86_CShigh();
//}


/*unsigned long ADS86_testReadWithCMD(unsigned char cmd, unsigned char address)
{
    unsigned long longbuffer;
    ENABLE_ADS86_CSlow();
/*    __delay32(10);
    unsigned char buffer[8];
    osm_WriteSPI1(cmd);                 //0b11001xx0 - READ_HWORD for example. See the list of definition input commands
    osm_WriteSPI1(address);
    buffer[3] = osm_ReadSPI1dummy(0xAA);
    buffer[2] = osm_ReadSPI1dummy(0xAA);
    buffer[1] = osm_ReadSPI1dummy(0xAA);
    buffer[0] = osm_ReadSPI1dummy(0xAA);
    DISABLE_ADS86_CShigh();
    ENABLE_ADS86_CSlow();    
    osm_WriteSPI1(cmd);                 //0b11001xx0 - READ_HWORD for example. See the list of definition input commands
    osm_WriteSPI1(address+2);
    buffer[7] = osm_ReadSPI1dummy(0xAA);
    buffer[6] = osm_ReadSPI1dummy(0xAA);
    buffer[5] = osm_ReadSPI1dummy(0xAA);
    buffer[4] = osm_ReadSPI1dummy(0xAA);
    longbuffer = (unsigned long)(buffer[3]) << 24;
    longbuffer |= (unsigned long)(buffer[2]) << 16;
    longbuffer |= (unsigned long)(buffer[1]) << 8;
    longbuffer |= (unsigned long)(buffer[0]);        
    DISABLE_ADS86_CShigh();
    return longbuffer;
}*/
