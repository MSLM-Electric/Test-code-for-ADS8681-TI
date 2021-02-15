/*
 * File:   newmainXC16.c
 * Author: Abdulhamidov
 *
 * Created on 30 èþëÿ 2020 ã., 12:40
 */


#include "mainxc16_header.h"
#define DI_triggerOfAnSignal PORTBbits.RB14
#define DO_testOutput PORTBbits.RB3

long LBuffer[6];
long L2Buffer[6];
int IBuffer[6];
/* Holds the information about SPI configuartion */
   unsigned int SPICON1Value;
   unsigned int SPICON2Value;
/* Holds the information about SPI Enable/Disable */
   unsigned int SPISTATValue;
   
   unsigned int Timer3Config = 0;
   unsigned int Timer3Period;

int main(void) 
{
    //////////////////////////INITIALIZATION IO-Ports///////////////////////////////
    TRISA = 0b0000000000000011;
    TRISB = 0b0000000000000011 | (1<<14) | (1<<6);
    /*----------------Peripheral PIN select INITIALIZATION------------------------*/
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); //Unlock the Peripheral Pin Select
    PPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP6); // Maps SPI1 input to RP6 pin
    PPSOutput(OUT_FN_PPS_SDO1, OUT_PIN_PPS_RP7); // Maps SPI1 output to RP7
    PPSOutput(OUT_FN_PPS_SCK1, OUT_PIN_PPS_RP5); //SPI1 SCK pin to RP5 as output clk (Master Mode)
    PPSInput(IN_FN_PPS_C1RX, IN_PIN_PPS_RP13);   //ECAN1 RX input to RP13
    PPSOutput(OUT_FN_PPS_C1TX, OUT_PIN_PPS_RP12); //ECAN1 TX to RP12    
    AD1PCFGL = 0xFFFF; //All analog pins configure to digital
    /*---------------------Change Notification n Interrupt------------------------*/
    /*-----------------------------Pull-UP ENABLE---------------------------------*/
    ////////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////DISABLE SOME MODULES/////////////////////////////////
    PMD1bits.T5MD = 1;
    PMD1bits.T4MD = 1;
    PMD1bits.DCIMD = 1;  //*
    PMD1bits.I2C1MD = 1; 
    PMD1bits.U2MD = 1;
    PMD1bits.U1MD = 1;   //*
    PMD1bits.SPI1MD = 0; //*
    PMD1bits.SPI2MD = 1; 
    PMD1bits.C1MD = 1;   //ECAN1 module disable*
    PMD1bits.AD1MD = 1;  //ADC1 module disable
    PMD2 = 0xFFFF;       //Input capture 8 module (IC8M) disable, IC7 module disable, IC2M, IC1M, OC4...1 modules disable
    PMD3 = 0xFFFF;       //Comparator, RTC Clock, PMP, CRC, DAC1 modules disable
    /////////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////INITIALIZATION SPI///////////////////////////////////
    CloseSPI1();  CloseSPI2();    
    //config1
    SPICON1Value  = ENABLE_SDO_PIN & SPI_MODE16_OFF &
                    SPI_SMP_OFF & SPI_CKE_ON &               //CKE = 1
                    SLAVE_ENABLE_OFF & ENABLE_SCK_PIN &
                    CLK_POL_ACTIVE_HIGH &                    //CKP = 0 should be for working with the ADS8681
                    MASTER_ENABLE_ON &
                    SEC_PRESCAL_2_1 &
                    PRI_PRESCAL_1_1;
    //config2
    SPICON2Value  = FRAME_ENABLE_OFF & FRAME_SYNC_OUTPUT & 
                    FRAME_POL_ACTIVE_LOW & FRAME_SYNC_EDGE_PRECEDE;
    //config3
    SPISTATValue  = SPI_ENABLE & SPI_IDLE_CON &
                    SPI_RX_OVFLOW_CLR;
    OpenSPI1(SPICON1Value, SPICON2Value, SPISTATValue);
    
    ////////////////////////////INITIALIZATION ECAN//////////////////////////////////
    
    /////////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////INITIALIZATION TIMER3/////////////////////////////////
    CloseTimer3();
    Timer3Config = T3_OFF & T3_IDLE_CON & T3_GATE_OFF & T3_PS_1_8 & T3_SOURCE_INT;// Idle continious, internal source clock to Timer3
    OpenTimer3(Timer3Config, Timer3Period = 170);
    __delay32(50000); //50 ms
   // osm_Timer3ON();
    //InitInterrupts();    
    while(1)
    {
        switch((PORTA & 0x03) | (PORTB & 0x01))
        {
            case 0: {/*DO_testOutput = 1;*/
            asm("nop");
            asm("nop");
            asm("nop"); break;                
            }
            case 1: { } break;
            case 2: {
                __delay32(10);
//              LBuffer[0] = ADS86_testReadWithCMD(READ_HWORD, DATAOUT_CTL_REG);
//              LBuffer[1] = ADS86_testReadWithCMD(READ, RANGE_SEL_REG+1);
//              LBuffer[2] = ADS86_testReadWithCMD(READ, RANGE_SEL_REG+2);
//              LBuffer[3] = ADS86_testReadWithCMD(READ, RANGE_SEL_REG);                
                LBuffer[2] = ADS86_testReadWithCMD(READ_HWORD, SDI_CTL_REG);
                LBuffer[4] = ADS86_testReadWithCMD(READ_HWORD, RANGE_SEL_REG);
                 __delay32(1000);  //1000us
                L2Buffer[0] = ADS86_WriteLSBcmd(RANGE_SEL_REG, 0x0004);
                __delay32(100);
                //L2Buffer[2] = ADS86_WriteHWORD(RANGE_SEL_REG, 0x0004);
                asm("nop");    
                __delay32(10);
                __delay32(1000); 
		LBuffer[5] = ADS86_testReadWithCMD(0, 0);
            } break;
            case 3: {
                __delay32(10);               
                asm("nop");  
                __delay32(60);
                LBuffer[0] = ADS86_testReadWithCMD(READ, RANGE_SEL_REG);
                LBuffer[2] = ADS86_testReadWithCMD(READ_HWORD, RANGE_SEL_REG);
                LBuffer[5] = ADS86_testReadWithCMD(READ_HWORD, DEVICE_ID_REG);
                asm("nop");                
                __delay32(1000);
                L2Buffer[0] = ADS86_WriteLSBcmd(RANGE_SEL_REG, 0x0004);
            } break;
            case 4: {
                asm("nop");            
            } break;
            case 5: {
                ADS86_testReadWithCMD(0b11001000, 0x04);
                asm("nop");            
            } break;
            default: break;
        }
        
    }    
    return 0;
}
