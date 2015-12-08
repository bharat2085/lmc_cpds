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
		while(1)
		__no_operation();

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

					#ifdef RUN_APP
					misc_init_r(); 						//currently blank.
					#endif

				  //init drivers.

			         //  init_SYSLeds();
			         //  DispatcherUARTConfigure();
			         //  init_console(); //

			         init_wifiDriver();

			         turnLedOn(SYSLED1); 				//to indicate that we are active and initiated WLAN successfully

					#ifdef RUN_APP
			         misc_init_s();						//currently blank.
					#endif

			         //unsolicicted_events_timer_init();	// not required in the newer service packs of cc3000.


			         //__bis_SR_register(GIE);

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



/*
void
Print_fw_version_number()
{

//
	// Generate the event to CLI: send a version string
	/
	//DispatcherUartSendPacket(pucUARTExampleAppString, sizeof(pucUARTExampleAppString));
        version_string[0]= 0x30 + PALTFORM_VERSION;
        version_string[1]= '.';
        version_string[2]= 0x30 + APPLICATION_VERSION_NUMBER_1;
        version_string[3]= 0x30 + APPLICATION_VERSION_NUMBER_2;
        version_string[4]= '.';
        version_string[5] = 0x30 + SPI_VERSION_NUMBER;
        version_string[6]= '.';
        version_string[7]= 0x30 + DRIVER_VERSION_NUMBER_1 ;
        version_string[8]= 0x30 + DRIVER_VERSION_NUMBER_2 ;
        version_string[9]=  '\n';
		version_string[10]=	'\r';
        //DispatcherUartSendPacket(version_string, SL_VERSION_LENGTH);
}
 *
 *
 */
