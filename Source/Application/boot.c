/**************************************************************************************************************
 * boot.c  - board specific booting.
 *
 *  Created on: Oct 22, 2013
 *  Last Modified on: 19/06/2014
 *  Last Update On:   13/02/2015
 *   Author: bharat
 ***************************************************************************************************************/
#include "msp430.h"
#include "board.h"
#include "evnt_handler.h"
#include "spi.h"
#include "wlan.h"
#include "basic.h"
#include "snet.h"

#define RUN_APP	// TODO:	put this in a header file.and include that header file here too.


extern void appMain();
extern int init_wifiDriver(void);
void reset_CC3000();


/*
 * systest_app()
 */
void
systest_app()
{
	//nothing to do currently

		signed char ret;

		ret= ConnectWithWellKnownAP();
		if(ret)
		while(1){
		__no_operation();
		fatal();
		}

}


//*****************************************************************************
//
//! bootmain
//!
//!  \param  None
//!
//!  \return uint8
//!
//!  \brief : all the board peripherals will be initialized here .
//  debug console will also be initialized for debugging info.
//
//*****************************************************************************
void  bootMain()
{
	//int status;
	//void (*p)(void);

	//
	//  Board  Initialization .
	//
					gpio_init();

					turnLedOn(1);
					__delay_cycles(5000000);
					turnLedOff(1);



					#ifdef RUN_APP
					misc_init_r(); 						//currently blank.
					#endif



				  //init drivers.

			         //  init_SYSLeds();
			         //  DispatcherUARTConfigure();
			         //  init_console(); //

			         init_wifiDriver();
			        // reset_CC3000();

			         turnLedOn(1);
			         __delay_cycles(5000000); 				//to indicate that we are active and initiated WLAN successfully
			         turnLedOff(1);


			        // reset_CC3000();

					#ifdef RUN_APP
			         misc_init_s();						//currently blank.
					#endif




			         //__bis_SR_register(GIE);  //


			         __delay_cycles(5000000);
			         turnLedOn(1);
			         __delay_cycles(3000000);
			         turnLedOff(1);



					#ifdef RUN_APP
			         appMain();
					#else
			        systest_app();
					#endif


			         while(1)
			         {
			        	 //__bis_SR_register(LPM2_bits + GIE);

			        	 toggleLed(SYSLED1);
			        	 __no_operation();
			        	 __delay_cycles(12000000);

						#ifdef SERVICE_PACK_OLDER_THAN_
			        	 //hci_unsolicited_event_handler();		// ---- will call the function ------->    CC3000_AsyncEventCallback() { CC3000.c}
						#endif

			        	 //status=wlan_ioctl_statusget();

			         }


}
