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
//
//*****************************************************************************
void initClk(void)
{

  // SMCLCK which will source also SPI will be sourced also by DCO, ACLK sourced by VLO.
    //


    CSCTL0_H = 0xA5;	//PSWD LOCK KEY

    CSCTL1 |= DCORSEL + DCOFSEL0 + DCOFSEL1;	 // Set max. DCO setting// 24MHZ
    CSCTL2 = SELA_1 + SELS_3 + SELM_3;		// set ACLK - VLO, the SMCLK = MCLK = DCO
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
