/* 
 * File:   spi_dspic_osm.h
 * Author: abdulhamidov
 *
 * Created on 12 августа 2020 г., 9:56
 */

#ifndef SPI_DSPIC_OSM_H
#define	SPI_DSPIC_OSM_H

#include <xc.h>
#include <spi.h> //only for checking function DataRdySPI1()

#define osm_DataRdySPI1() SPI1STATbits.SPIRBF
#define osm_DataWriteRdySPI1() SPI1STATbits.SPITBF
//#define DO_CS_memory LATAbits.LATA4  
//#define ENABLE_MEMORY_CSlow() DO_CS_memory = 0;
//#define DISABLE_MEMORY_CShigh() DO_CS_memory = 1;


extern void osm_WriteSPI1(unsigned char txData);
extern unsigned char osm_ReadSPI1();
extern unsigned char osm_ReadSPI1dummy(unsigned char dummy_byte);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_DSPIC_OSM_H */

