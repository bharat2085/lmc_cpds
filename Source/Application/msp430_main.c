/*****************************************************************************
*Date: 22.10.2013 v11
*Date: (imp) modification on 19.06.2014
*Date: name changed on 13.02.2015
*Date: last update: 13.02.2015
*****************************************************************************/
#include <msp430.h>
#include "basic.h"

typedef unsigned char   uint8;

extern void bootMain();
//*****************************************************************************
//
//! \brief  Restarts the MSP430
//!
//! Restarts the MSP430 completely. One must be careful
//!
//! \return never
//!
//
//*****************************************************************************
void restartMSP430()
{

    PMMCTL0 |= PMMSWPOR;

    // This function will never exit since it forces a complete
    // restart of the MSP430.
}


//*****************************************************************************
//
//! init clk
//!
//!  \param  None
//!
//!  \return none
//!
//!  \brief :
//!
//!       For CPU:   there is one single clock signal.
//!
//! 			 MCLK,  master clock is used by the cpu & system.
//!
//		For Peripherals:   two clock signals are available, and each peripheral need to be configured to select one of these signal as their active clock source.
//!
//! 			SMCLK, subsystem master clock  is used by peripherals. ( each peripheral)
//!				ACLK,  Auxilliary clock is  used by peripherals ( configurable for each peripheral)
//
//!            These clock signals can select one of any of the following clock sources.
//!			 Moreover these clock signals  can be configured to select the the clock comming from the selected clock after getting divided by one of the divisors(1,2,4,8,16,32).
//!
//!		Available Clock Sources in MSP430:
//!		External sources:
//!						XT1CLK(low frequency crystal-3.27 Mhz watch crystal)  & XT2CLK(high frequency crystal-24Mhz) : both are on the same pins in this device.
//!     INTERNAL Sources:
//!                      DCO (Digitally controlled Oscillator)  && VLO (low frequency oscillator-very low power, very low frequency 10khz).
//!						 DCO has three selectable fixed frequencies.

//!
//*****************************************************************************
void initClk(void)
{

	// MCLK (master clock)  used by cpu , is being sourced by DCO here.
  // SMCLCK   used by SPI_B(ucB0) for wlan, is being   sourced  by DCO. (configuration for spi_b used for wlan  to select smclk done in init_spi() in spi.c)
 //  ACLK   used by other peripherals is sourced by VLO.
   //

	// set ACLK=VLO, the SMCLK = DCO,   MCLK= DCO
	// divisor=1
	// password = 0xA5  ( fix value by default).


	//CSCTL0 REGISTER: PASSWORD TO UNLOCK  CS REGISTERS.
    CSCTL0_H = 0xA5;	//PSWD unLOCK KEY  to unlock access to CS(clock system) registers

    //CSCTL1 REGISTER: DCO range select register.
    CSCTL1 |= DCORSEL + DCOFSEL0 + DCOFSEL1;	 // Set max. DCO setting// 24MHZ

    //CSCTL2 REGISTER: Select  aux clk(SELA), select SMCLK(SELS), select master clk(SELM).
    CSCTL2 = SELA_1 + SELS_3 + SELM_3;		// set ACLK=VLO,  SMCLK = DCO,   MCLK= DCO

    //CSCTL3 REGISTER: Divisor Select.
    CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;		// set all dividers to 0

}

//********************************************************************************************
//! testmain
//!
//!  \param  None
//!
//!  \return uint8
//!
//!  \brief	: processor dependent boot , no info regarding board peripherals available here.
//      debug console not initiialised , so can use connected leds for debug purpose.
//********************************************************************************************
void testMain()
{
//do something to test that processor is running , without any issue.
while(1)
{
	 __no_operation();
__delay_cycles(110000);   //

__no_operation();
}

}


//**************************************************************************************************
//
//! main
//!
//!  \param  None
//!
//!  \return none
//!
//!  \brief  - processor dependent boot , no info regarding board peripherals available here.
//
//*************************************************************************************************

void main(void)
{


	  //  WATCHDOG TIMER
	//						//(Stop/disable WDT).
			WDTCTL = WDTPW + WDTHOLD;

	//  SYSTEM CLOCK  Initialization .
			initClk();
			__delay_cycles(110000);   ////wait for System Clock Stablization.


	#ifndef _DEBUG_MSP_TESTBOOT

			 bootMain();
	#else

			testMain();
	#endif

		// Indefinite while loop
		//
			 while(1)
			 {
				 __no_operation();

			 }


//	return;

}
/*************************************************************************************/
// *******************      end of main       ***************************************//
