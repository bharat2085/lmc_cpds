
/*
 * snet.c
 *
 *  Created on: 01/08/2014
 *  Modified on: 10/09/2014
 *  Last Modified:
 *      Author: bharat bhushan
 */

#include "basic.h"
#include "msg.h"
#include "board.h"

#include "data_types.h"
#include "socket.h"
#include "netapp.h"
#include "nvmem.h"
#include "wlan.h"
#include "cc3000.h"
#include "string.h"
extern uint8 uiCC3000Connected,uiCC3000DHCP;
typedef signed char int8;

#define BIND_SOCKET_TYPE   1
#define NOBIND_SOCKET_TYPE 0



#define WLAN_CONNECT_USING_PROFILE


#define RX_BUFFER_OVERHEAD   20
#define RX_BUF_LEN 			 30
#define TX_BUF_LEN 			 20

#define ANY_IP				0
#define ANY_PORT			0
#define AGENTS_COMMON_PORT       5000
#define MGR_PORT				 5099



unsigned char mgr_ip[4]= {192,168,39,2};


static char txbuffer[TX_BUF_LEN]={0};		//TODO: probably rxbuffer can be used for tx also, hence this is not required.
static char rxbuffer[RX_BUF_LEN + RX_BUFFER_OVERHEAD ];

long  hSocket_TX=-2, hSocket_RX=-2;

sockaddr mgr_addr;

void
wait_for_connection()
{

	 // wait for both dhcp && connection Event.
	        //
	          while ((uiCC3000DHCP == 0) || (uiCC3000Connected == 0))
	           {

	                __delay_cycles(12000000);

					//#ifdef WLAN_TEST_MODE
	                if(!uiCC3000Connected)
	                toggleLed(2);
	                if(!uiCC3000DHCP)
	                toggleLed(3);
					//#endif

	                //hci_unsolicited_event_handler();							//This is not required for newer sp(FW).  */
	           }
}

int senddata_to(char *buffer, int len, unsigned char* destip, int port)
{

		signed int ret;
		sockaddr sockAddr;

		if(hSocket_TX <0)
		return -1;

		  memset(&sockAddr,0, sizeof(sockaddr));

		  sockAddr.sa_family = AF_INET;

		  // the source port
		  sockAddr.sa_data[0] = (port & 0xFF00)>>8;
		  sockAddr.sa_data[1] = (port & 0x00FF);

		  sockAddr.sa_data[2] =	destip[0];
		  sockAddr.sa_data[3] =	destip[1];
		  sockAddr.sa_data[4] =	destip[2];
		  sockAddr.sa_data[5] =	destip[3];



		  ret=sendto( hSocket_TX, buffer, len, 0, &sockAddr, sizeof(sockAddr));
		  if(ret==-1){
				//error
			  _no_operation();
			  turnLedOn(4);
			  fatal();
				return 0;  //if need to return , though shouldnt.
		  }else
		  {
			  //success
			  _no_operation();
			  while(ret--){
			  turnLedOn(4);
			  __delay_cycles(3000000);
			  turnLedOff(4);
			  __delay_cycles(7000000);
			  }
		  }



	return ret;
}


int
rcvdata_from( char* buffer, uint8 len,  int ip, int port )
{
	signed int ret;
	sockaddr sockAddr;

		if(hSocket_RX < 0)
		return -1;

		 memset(buffer, 0, len);

	socklen_t addrlen = sizeof(sockaddr);
	  memset(&sockAddr,0, sizeof(sockaddr));

	  sockAddr.sa_family = AF_INET;
	  // the source port
	  sockAddr.sa_data[0] = (port & 0xFF00)>>8;
	  sockAddr.sa_data[1] = (port & 0x00FF);

	  memset(&sockAddr.sa_data[2], 0, 4); //rcv from any ip.


	  ret=recvfrom( hSocket_RX, buffer, len, 0, &sockAddr, &addrlen);
	  if(ret>=1){
		  	  	  __no_operation();

		  	  	  	  	  turnLedOn(5);
		  	    		 __delay_cycles(22000000);
		  	    		turnLedOff(5);
	  	  }
	  	  	else if(ret==0){

	    	 //timeout
	          __no_operation();

	        }
	        else if(ret<0) {
	        	 turnLedOn(5);
	        		__no_operation();
	                  fatal();

	        		ret=0;   //if need to return , data is zero, though shouldnt return.
	            }


	return ret;
}

void
test_rcv_data()
{

	rcvdata_from(rxbuffer, RX_BUF_LEN,  ANY_IP, ANY_PORT );
}

void
test_send_data_to(unsigned char destip)
{
	char txbuffer[TX_BUF_LEN]="Welcome Data MSP430!";
	unsigned char ip[4] = {192,168,11,0};


	ip[3] = destip;

	senddata_to(txbuffer,TX_BUF_LEN, ip, MGR_PORT ); //TX_BUF_LEN
}


int8
test_dataLink(unsigned char destip)
{
	//char* pbuffer = rxbuffer ;
	int ret,bytes_rcvd;
	unsigned char ip[4] = {192,168,11,0};

    ip[3] = destip;		//destip


    bytes_rcvd = rcvdata_from(rxbuffer, RX_BUF_LEN, ANY_IP, ANY_PORT );
    if(bytes_rcvd)
    {

    	ret= senddata_to(rxbuffer, TX_BUF_LEN, ip, MGR_PORT );
    	if(ret<0)
		return -1;

    	if(bytes_rcvd > TX_BUF_LEN){
		ret= senddata_to( (rxbuffer+ TX_BUF_LEN), (RX_BUF_LEN -TX_BUF_LEN), ip, MGR_PORT );
		if(ret<0)
			return -1;
    	}


    	while(bytes_rcvd--){
			turnLedOn(5);
	    	__delay_cycles(300000);
	    	turnLedOff(5);
	    	__delay_cycles(700000);
		}


   }



	return 0;
}


uint8
receive_msg_from_Mgr(Msg *msg)
{
		int ret;
		uint8 i;
		uint8 buf_len;

		buf_len= RX_BUF_LEN;
		if(buf_len<SIZE_STRUCT_MSG)
			fatal();

		/*TODO: Is there any benefit of using heap??
		char * rxbuffer;
		rxbuffer =( char *)malloc(RX_BUF_LEN); // calloc is used as it zeros the memory, what malloc doesnt.
		memset(rxbuffer, 0, sizeof(rxbuffer));
		*/


ret	=   rcvdata_from( rxbuffer, buf_len, 0, ANY_PORT );
		if(ret>= SIZE_STRUCT_MSG)
		{

				for(i=0; i<SIZE_STRUCT_MSG; i++)
				*( (char*)msg +i)= rxbuffer[i];

			return 1;
		 }


return 0;
}

int
send_msg_to_Mgr(Msg * msg)
{
	int ret,i;
	int buf_len= TX_BUF_LEN;


		if(buf_len<SIZE_STRUCT_MSG)
			fatal();

	for(i=0; i<SIZE_STRUCT_MSG; i++)
	txbuffer[i]	= *( (char*)msg + i);

	ret= senddata_to( txbuffer, TX_BUF_LEN, mgr_ip, MGR_PORT);

	return ret;
}

void
net_config_ip(unsigned long * ip)
{
 char err=11;

 char mask[4]={255,255,255,0};
 char gw[4]= {192,168,39,1};
 char dns[4]={0,0,0,0};

	  //set ip for this wlan chipset( for this lmc device).

	      err=   netapp_dhcp( ip , (unsigned long * )mask ,(unsigned long *) gw , (unsigned long *) dns );
	      if(err)
	      {
	    	fatal();
	      }

	      reset_CC3000();
	      __delay_cycles(40000000);
}



void
configureGetIpUsingDhcp()
{
	char err=11;
		  //set ip for this wlan chipset( for this lmc device).

		      err=   netapp_dhcp(0,0,0,0);
		      if(err)
		      {
		    	while(1)
		    		__delay_cycles(100000);
		      }

		      reset_CC3000();
		      __delay_cycles(30000000);

}

void
set_mac_addr(unsigned char * mac_addr)
{
	unsigned char err;

	err= nvmem_set_mac_address (mac_addr);
	if(err)
		fatal();
}

void
set_netapp_timeouts()
{
    int ret;
	unsigned long aucDhcp,aucArp, aucKeepalive,aucInactivity;

    aucDhcp=14400;			//default value- 4hours.
    aucArp=120;			    //dont know its use.set randomly- 2min.
    aucKeepalive=0;			//keepalive not required currently.
    aucInactivity=0; 		//socketinactivity=infinity, as socket should never close.

    ret= netapp_timeout_values( &aucDhcp, &aucArp, &aucKeepalive, &aucInactivity);
    if(ret!=0)
               fatal();

}

int
setsockopt_rcv_timeout( unsigned long timeout )
{
	 int ret;


    //timeout= 10000; //milliseconds
	 if(hSocket_RX<0)
		 return -1;

   ret =setsockopt(hSocket_RX,SOL_SOCKET, SOCKOPT_RECV_TIMEOUT, &timeout,sizeof(timeout) );
    if(ret== -1)
    {
      //prints("\r Error changing rec_timeout \n");
      fatal();

    }

    return 0;
}


//*****************************************************************************
//	OpenUdpPort_Mgr()
//
//
//
//
//
//*****************************************************************************
int8
OpenUdpPort_Mgr(long *ptrSd, int port, int8 type_rx )
{
	     int8 ret=0;


		// unsigned char mgr_ip[]={192,168,39,7};		//mgrs ip addr
        //sockaddr mgr_addr;		// same as above//

	     sockaddr sockAddr;

    	 // int port  = AGENTS_COMMON_PORT;

        //prints("\r Inside open connection with mgr. \n");

    	  if ( (uiCC3000DHCP == 0) || (uiCC3000Connected == 0))
    	  {
    		  	return -1;
    	   }


    	//create socket
        ret =*ptrSd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//SOCK_DGRAM
                    if(ret==-1)
                    {

                    	          closesocket((long)1);
                    	          closesocket((long)3);
                    	          closesocket((long)2);
                    	          closesocket((long)4);

                    	          *ptrSd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//SOCK_DGRAM
                    	          if(*ptrSd==-1)
                    	          {
                    	        	  prints("\rError opening Socket\n");

                    	        	  fatal();
                    	          }

                     }

if(type_rx){

					// bind socket to listen on the port defined for this app.

                      	memset(&sockAddr,0, sizeof(sockaddr));

                         sockAddr.sa_family = AF_INET;

                        // the  port
                        sockAddr.sa_data[0] = (port & 0xFF00)>>8;
                        sockAddr.sa_data[1] = (port & 0x00FF);


                        // the IP.
                        //Only need to bind port with the socket.
                        //  ip  must set as 0.0.0.0 ( cc3000 will automatically pick up the ip obtained by dhcp).
                        memset(&sockAddr.sa_data[2], 0, 4);


                    ret= bind(*ptrSd, &sockAddr, sizeof(sockaddr));
                    if(ret != 0)
                    {
                     // prints("\r Error : binding socket\n");
                      fatal();
                    }
  }



return ret;
}



int8
open_rxPort_Mgr()
{
	int8 ret;

ret=	OpenUdpPort_Mgr( &hSocket_RX, AGENTS_COMMON_PORT, BIND_SOCKET_TYPE);

	return ret;
}


int8
open_txPort_Mgr()
{
	int8 ret;
ret= OpenUdpPort_Mgr( &hSocket_TX , ANY_PORT, NOBIND_SOCKET_TYPE);

	return ret;
}




void
testconnect_unsecured()
{
	char ret;
	// Disable Profiles and Fast Connect
	ret= wlan_ioctl_set_connection_policy(0,0,0);
	if(ret){
	          prints("\r Error : set connection policy \n");
	         while(1);
	        //  fatal();
	        }


	ret=wlan_connect(WLAN_SEC_UNSEC,"BSNL_AP",7,
	                   NULL, 0, 0);
	if(ret)
		fatal();

}

void
testconnect_secured_wep()
{
int ret=5;
// Disable Profiles and Fast Connect
ret= wlan_ioctl_set_connection_policy(0,0,0);    //auto reconnect mode(3.UseProfiles) not supported for wlan_connect -connect but returns (-2)  fatal error in wlan_connect,(2.)fast connect no error,doesnt reconnect on signal reentry  but reset reconnect work.
if(ret){
          prints("\r Error : set connection policy \n");
         while(1)
          fatal();
        }

reset_CC3000();
/*ret=wlan_connect(WLAN_SEC_WEP,"BSNL_AP",7,
                   NULL, "INDIA", 5);*/			 	//sendto not working with wep .
/*
ret=wlan_connect(WLAN_SEC_UNSEC,"BSNL_AP",7,
                   NULL, NULL, 0);*/  				//send working fine with open(unsecured).
ret=wlan_connect(WLAN_SEC_WPA2,"BSNL_AP",7,
                   NULL, "testindia", 9);			//send working fine with  wpa2
        if(ret)
        {
          //prints(" Error : connecting with AP\n");
          fatal();

        }
//*/


}

signed char
ConnectWithWellKnownAP()
{
	signed char ret=-1;

wlan_disconnect();
ret=wlan_ioctl_del_profile(255);
if(ret)
 fatal();

ret= wlan_ioctl_set_connection_policy(0,0,0);
if(ret)
 fatal();

reset_CC3000();
wlan_disconnect();

ret=wlan_add_profile(WLAN_SEC_WPA2,"BSNL_AP",7, NULL, 0, 0x18, 0x1e, 2,"testindia", 9); //working
if(ret<0)
 fatal();
/*
 ret=wlan_add_profile(WLAN_SEC_WEP,"BSNL_AP",7, NULL,0 ,5, 0,1,"INDIA", 0);
 if(ret<0)
 fatal();

 ret=wlan_add_profile(WLAN_SEC_WPA,"BSNL_AP",7, NULL,0 ,0, 0,1,"testindia", 9); //not able to connect TODO: try more
  if(ret<0)
   fatal();*/


 /*ret=wlan_add_profile(WLAN_SEC_UNSEC,"BSNL_AP",7, NULL,0 ,0, 0,1,NULL, 0);
  if(ret<0)
  fatal();*/
//
 //wlan_disconnect();

ret= wlan_ioctl_set_connection_policy(0,1,1); // autoconnect mode, only fast connect  & using profiles, DONT USE  open ap.
if(ret)
 fatal();


//reset CC3000 to apply policy.
         reset_CC3000();


        return ret;
}

/********************************************************************************************************************/
/********************************************************************************************************************/
/********************************************************************************************************************/






/*
                         // set above   IP address
                         sockAddr.sa_data[2] =  ip[0];
                         sockAddr.sa_data[3] =  ip[1];
                         sockAddr.sa_data[4] =  ip[2];
                         sockAddr.sa_data[5] =  ip[3];
                      */

#ifdef USE_TCP_SOCKET
void
connect_Socket_Mgr()
{


   //5.
                    ///*
                    //Connect
                    //Establish tcp connection  with mgr.
                    //
                    port = (SERVER_PORT);
                    mgr_addr.sa_family = AF_INET;
                    // the source port
                    mgr_addr.sa_data[0] = (port& 0xFF00)>>8;
                    mgr_addr.sa_data[1] = (port& 0x00FF);
                    // set above   IP address
                    mgr_addr.sa_data[2] =  mgr_ip[0];
                    mgr_addr.sa_data[3] =  mgr_ip[1];
                    mgr_addr.sa_data[4] =  mgr_ip[2];
                    mgr_addr.sa_data[5] =  mgr_ip[3];

                    ret= connect(hSocket,(const sockaddr*)&mgr_addr,sizeof(mgr_addr));

//*/



}
#endif

#ifdef TEST_PURPOSE_ONLY
int test_Connection()
 {
   signed long ret=0;
   char buffer[16]= {0,2,4,6,8,10,12,14,16,18,20};

   unsigned int tofrom_IP[4]={ 192,168,39,4};
 //  unsigned int mgr_IP[4]={ 192,168,39,2};
   sockaddr mgr_Addr, Addr;
   socklen_t addrlen = sizeof(sockaddr);
   unsigned long aucDhcp,aucArp, aucKeepalive,aucInactivity;
char a;
char b,c ;
char *p;

//socklen_t  optlen;


//void *rxbuffer;

/*
rxbuffer= malloc(RX_BUF_LEN);
if(rxbuffer==NULL)while(1);
*/

  unsigned long timeout= 30000; //milliseconds





  	  memset(&mgr_Addr, 0, sizeof(sockaddr));

   while ((uiCC3000DHCP == 0) || (uiCC3000Connected == 0))__delay_cycles(100000);;

   //
        //create socket
        //
           hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//SOCK_DGRAM
                      if(hSocket==-1)
                      {
                        prints("\rError opening Socket\n");
                        //fatatl error //
                        fatal();
                       }


                      aucDhcp=14400;
                      aucArp=3600;
                      aucKeepalive=60;
                      aucInactivity=150;

                     ret= netapp_timeout_values(0,0,0,0);
                     if(ret!=0)
                    	 while(1);

                        mgr_Addr.sa_family = AF_INET;

                        // the source port
                        mgr_Addr.sa_data[0] =0x12;
                        mgr_Addr.sa_data[1] =0x9d;
                        ///*


                        memset(&mgr_Addr.sa_data[2],0,4);

                        /*
                         // set above   IP address
                         mgr_Addr.sa_data[2] =  mgr_IP[0];
                         mgr_Addr.sa_data[3] =  mgr_IP[1];
                         mgr_Addr.sa_data[4] =  mgr_IP[2];
                         mgr_Addr.sa_data[5] =  mgr_IP[3];
                         */
                       /*
                          // set above   IP address
                         mgr_Addr.sa_data[2] =  0xc0;
                         mgr_Addr.sa_data[3] =  0xa8;
                         mgr_Addr.sa_data[4] =  0x27;
                         mgr_Addr.sa_data[5] =  0x03;
                           //0x03;
                         */


//only bind the port with the socket on which do you want to hear in this  app, ip must  be kept 0.0.0.0 . cc3000  will automatically do the right.( bind with the ip obtained using dhcp).

                         ret= bind(hSocket, &mgr_Addr, sizeof(sockaddr));
                                            if(ret != 0)
                                            	while(1);


                         ret =setsockopt(hSocket,SOL_SOCKET, SOCKOPT_RECV_TIMEOUT, &timeout,sizeof( timeout ) );
                         if(ret!=0)
                         {
                        	 while(1);
                         }

 	 	 	 	 	 	 Addr.sa_family = AF_INET;

                         // the source port
                         Addr.sa_data[0] =0x12;
                         Addr.sa_data[1] =0x9d;
                         ///*
                          // set above   IP address
                          Addr.sa_data[2] =  tofrom_IP[0];
                          Addr.sa_data[3] =  tofrom_IP[1];
                          Addr.sa_data[4] =  tofrom_IP[2];
                          Addr.sa_data[5] =  tofrom_IP[3];

                         // /*
                             ret=sendto(hSocket, buffer,16,0, &Addr, sizeof(sockaddr));
                             if(ret<=0)
                             {
                          while(1);
                            // prints("\r bytes sent: to unicast  192.168.39.03, port 4444 \n");
                             }
                          //*/

   while(1)
   {

   ret=recvfrom(hSocket,rxbuffer,RX_BUF_LEN,0,&mgr_Addr, &addrlen);
   		    	   if(ret>=1)
   		    	   {
   		    		   //while(1);
   		    		   p= (char *)rxbuffer;

   		    	   a=p[0];
   		    	   b=p[1];
   		    	   c=p[2];

   		    	   addrlen=sizeof(sockaddr);
   		    	   }
   		    	   else if(ret==0)
   		    	   {//timeout
   		    		   __no_operation();
   		    	   }
   		    	   else if(ret<0){
   		    		__no_operation();
   		    		//error
   		    	   }



   }
  // return ret;
 }
#endif
