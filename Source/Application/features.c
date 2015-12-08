/*
 * features.c
 *
 *  Created on: Aug 3, 2014
 *      Author: bharat
 */
#include "defs.h"
#include "basic.h"
#include "features.h"
#include "fpc.h"

#include "msg.h"


/*
//power distribution lpds board/system.
	 //sslc
*/
 void doAction_pds(Msg* p)
 {
	 //power distribution lpds board/system.
	 //sslc
	int cmd;

	 __no_operation();

	 cmd= p->msg;


	switch(cmd)
	{

	case CMD_ACTION_SWITCH_OUTX_ONOFF:
		break;


	case CMD_SET_OUTX_LEVEL:
		break;

	case CMD_SET_OUTX_SCHEDULE_ONTIME:
		break;

	case CMD_SET_OUTX_SCHEDULE_OFFTIME:
		break;

	case CMD_ENABLE_OUTX_ONSCHEDULER:		//will enable/disable on scheduler.
		break;

	case CMD_ENABLE_OUTX_OFFSCHEDULER:
		break;



	default:
		_no_operation();
		//reply_mgr(UNKNOWN_COMMAND);
		break;

	}



 }


/*
 //DOOR UNLOCK, KEYLESS ENTRY.
  *
  */
void doAction_duke(Msg* p)
{

	//DOOR UNLOCK, KEYLESS ENTRY.
		int cmd;
	__no_operation() ;


		 cmd= p->msg;


		switch(cmd)
		{

		case CMD_ACTION_DOORLOCK_UNLATCH:
			break;

		case CMD_ACTION_OPEN_DOOR:
			break;

			//setting the security feature,  setting key for entry.
			//Alerts on door open.
			//

		default:

			break;
		}

}


void doAction_playtones(Msg* p){
	__no_operation() ;
}
 void doAction_glassOpacCntrl(Msg* p){
	 __no_operation() ;
 }
void doAction_rtc(Msg* p){
	__no_operation() ;
}
 void doAction_appsAlerts(Msg* p){
	 __no_operation() ;
 }






void doAction_fwUpdate(Msg* msg){
	__no_operation();
}
 void doAction_SystemInfo(Msg* msg){
	 __no_operation() ;
 }
 void doAction_fwReboot(Msg* msg){
	 __no_operation();
 }




void doAction_nullsupport()
{
	_no_operation();
	//reply_Mgr(FEATURE_NOT_SUPPORTED_IN_SRCCODE_BUILD_OF_THIS_BOARD)
}

void
feature_Interpreter(Msg *m)
 {
	 uint8 feature;
	 feature = m->feature;

	 	    //command interpreter.
	 	switch (feature)
	 	{
	 	case FEATURE_PDS:
			#ifdef BUILD_FEATURE_PDS
	 				doAction_pds(m);
	 				break;
			#else
	 				doAction_nullsupport();
			#endif

	 	case FEATURE_DUKE:
			#ifdef BUILD_FEATURE_DUKE
	 				doAction_duke(m);
	 				break;
			#else
	 				doAction_nullsupport();
			#endif

	 	case FEATURE_PLAYTONE:
			#ifdef BUILD_FEATURE_PLAYTONE
	 				doAction_playtones(m);
	 				break;
			#else
	 				doAction_nullsupport();
			#endif

	 	case FEATURE_OPACCNTRL:
			#ifdef BUILD_FEATURE_GLASSTRANSCNTRL
	 				doAction_glassOpacCntrl(m);
	 				break;
			#else
	 				doAction_nullsupport();
			#endif


	 	case FEATURE_ALERTS:
	 				#ifdef BUILD_FEATURE_ALERTS
	 		 				doAction_appsAlerts(m);
	 		 				break;
	 				#else
	 		 				doAction_nullsupport();
	 				#endif


	 	case FEATURE_SCHEDULE:
	 				#ifdef BUILD_FEATURE_SCHEDULE
	 		 				doAction_schedule(m);
	 		 				break;
	 				#else
	 		 				doAction_nullsupport();
	 				#endif


	 	case FEATURE_RTC:
			#ifdef BUILD_FEATURE_RTC
	 				doAction_rtc(m);
	 				break;
			#else
	 				doAction_nullsupport();
			#endif




	 	case FEATURE_FWUPDATE:
	 				doAction_fwUpdate(m);
	 				break;

	 	case FEATURE_SYSINFO:
	 				doAction_SystemInfo(m);
	 				break;

	 	case FEATURE_REBOOT:
	 				doAction_fwReboot(m);
	 				break;


	 default:
	 	_no_operation();
	 	//reply_Mgr(UNKNOWN_FEATURE);
	 	   break;
	 }

 }
