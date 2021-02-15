 /*
 * Author          : MSLM Electric
 * Company         : & Engineering
 * Project         : SPI Library for MPLAB XC16 
 * Microcontroller : dsPIC 33FJ128GP802
 * Created on August 12, 2020, 9:59 PM
 */
#include "spi_dspic_osm.h"

extern void osm_WriteSPI1(unsigned char txData)
{
//    unsigned char temp;
//    temp = SPI1BUF;
    SPI1BUF = txData & 0xff;
    while(!osm_DataWriteRdySPI1()); //Transmit Wait
}

/*******************************************************************************
* Function Name  : 
* Description    : Read from SPI interface by writing dummy byte (0xAA)
* Input          : 8bit input
* Output         : 
* Return         : 8bit output
*******************************************************************************/
extern unsigned char osm_ReadSPI1dummy(unsigned char dummy_byte)
{
     unsigned char temp;
     temp = SPI1BUF;
    //unsigned char buffer;
//    if (SPI1STATbits.SPIRBF)
//    {        
        SPI1BUF = dummy_byte;  //dummy byte
        SPI1STATbits.SPIROV = 0;
        while(!osm_DataRdySPI1()); //Receive Wait
        //SPI1STATbits.SPIROV = 0;
        //buffer = SPI1BUF;
        return SPI1BUF;
//    } 
//    return 0xFF;    
}


/*******************************************************************************
* Function Name  : 
* Description    : Read from SPI interface by writing dummy byte (0xAA)
* Input          : 8bit input
* Output         : 
* Return         : 8bit output
*******************************************************************************/
extern unsigned char osm_ReadSPI1()
{
    //unsigned char buffer;
    SPI1BUF = 0xAA;  //dummy byte
    while(!osm_DataRdySPI1()); //Receive Wait
    SPI1STATbits.SPIROV = 0;
    //buffer = SPI1BUF;
    return SPI1BUF;
}




