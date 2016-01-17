
/*
 * appmain.c
 *
 *  Created on: Jul 4, 2014
 *  Last Updated: feb 12, 2015
 *      Author: bharat
 */
#include "basic.h"
#include "msg.h"
#include "board.h"


typedef signed char int8;

//#define TEST_CONNECTION

//#define FIRST_TIME_RUN

//#define TEST_SOCKET_RECEIVE
extern unsigned char nvmem_read_sp_version(unsigned char*);
extern void  testconnect_secured_wep();
extern void  testconnect_unsecured();
extern signed char ConnectWithWellKnownAP();
extern void  wait_for_connection();
extern void  configureGetIpUsingDhcp();
extern void  net_config_ip(unsigned long * );
extern int   setsockopt_rcv_timeout( unsigned long);
extern void  set_mac_addr(unsigned char *);
//extern int8 OpenUdpPort_Mgr(long*, int );
extern  int8 open_rxPort_Mgr();
extern  int8 open_txPort_Mgr();

extern uint8 receive_msg_from_Mgr(Msg *);
extern int	 send_msg_to_Mgr(Msg *);

extern int8 test_dataLink();
extern void test_send_data_to(unsigned char);
extern int test_rcv_data();
extern int test_Connection();
extern void feature_Interpreter(Msg*);

Msg 	gsm;			/*creating as auto variable in Execute_Mgr_Cmd(),
 	 	 	 	 	 	 results into malfunctioning due to stack overflow. hence made  global.*/

/************************************************************************************************
 * 		Execute_Mgr_Cmd()
 *
 *
 *
 *
 *****************************************************************************************************/
void Execute_Mgr_Cmd()
{
	uint8 ret;


	ret=receive_msg_from_Mgr(&gsm);
	if(!ret){
		_no_operation();
		return;
	}
	else{

		turnLedOn(SYSLED2);
		// 	__delay_cycles(22000000);


		//Execute Commands.
		feature_Interpreter(&gsm);

		turnLedOff(SYSLED2);

		return;
	}
}

inline
void Ist_run_config()
{
	//FIRST_TIME_CONNECTION_SETTING_PROFILES


	//this is temporary arrangement.
	//TODO: need to implement in a proper way.


/*
		char ip[4]={192,168,39,99};
			unsigned char mac[]={0x0, 0x25, 0xca, 0x03, 0x10, 0xef};



			//do all configurations.
			set_mac_addr(mac);				//EACH DEVICE NEED TO HAVE/SET A UNIQUE ADDR.()
		  // configureGetIpUsingDhcp();
			net_config_ip((unsigned long *)ip); 	//NEED TO CONNECT TO THE REGISTRATION DEVICE WITH OR WITHOUT DHCP.
*/
		   //then connect.
		  // testconnect_unsecured();


	         set_netapp_timeouts();     //must be done here.

			ConnectWithWellKnownAP();				//NEED TO CONNECT TO THE REGISTRATION DEVICE.
}



void register_Me()
{


}

/*********************************************************************************************
 * 					appMain()
 *
 *
 *
 *********************************************************************************************/
void
appMain()
{
	uint8  err, i,count;
	unsigned char patchVer[2],patchid, patchversion;

						__delay_cycles(5000000);
							turnLedOn(1);				//booting Done!!




		#ifdef 	FIRST_TIME_RUN		//we need to set connection settings && profile on first run only ( Ist_run_config() )
									// after that only wait_for_connection(), however   this build configuration is temporary arrangement, rather there should be a checkpt for first run.
		Ist_run_config();			//FIRST_TIME_CONNECTION_SETTING_PROFILES
		#endif



//		testconnect_secured_wep();   //wlan_connect() takes some time to return.
		wait_for_connection();



		err=	open_txPort_Mgr();
				if(err)
				{
					//fatal error , failed to open socket with mgr.
					// restart .
					fatal();
				}


err=	open_rxPort_Mgr();
		if(err)
		{
			//fatal error , failed to open socket with mgr.
			// restart .
			fatal();
		}

		err= setsockopt_rcv_timeout( 5000 ); //5s
		if(err){
					fatal();
			}


    	// register_Me();
 		//TODO: this need to be substituted with registration related functions, which will be probably triggered  on button press.


				         turnLedOn(7);
						__delay_cycles(1000000);
						turnLedOff(7);
						__delay_cycles(5000000);


		test_send_data_to(9);

    	 //Infinite loop.
    	 while(1) {


    		 	test_dataLink(9);

 		 	//test_send_data_to(9);
    		 	//test_rcv_data();


    		 	 //Execute_Mgr_Cmd();
    		// test_send_data_to(9);

    		 	 turnLedOn(7);
    		 	 __delay_cycles(7000000);
    		 	turnLedOff(7);
    		 	__delay_cycles(3000000);
    	 }


//  return;
}

