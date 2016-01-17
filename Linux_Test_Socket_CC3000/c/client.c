/* client */
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>

#define VS_STR "1.23.02"

#define PORT 5000
#define sendto_IP "192.168.11.3"

#define LEN_BUF 11

char buf[LEN_BUF]={98,15,17,20,67};



/*******************************************************************



*******************************************************************/
int 
main()
{
 
int hsock=0,result, ret, ip_cc3000;
unsigned char i;
int  n=LEN_BUF;

struct sockaddr_in address;



printf("socket client program version %s \n", VS_STR);

/*
printf("Enter CC3000 IP\n"):
scanf("%d", &ip_cc3000);*/

printf("CC3000 IP=%s  PORT=%d\n",sendto_IP,PORT);

/*create unnamed socket */
hsock =socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
       printf("sockfd=%d \n", hsock);


memset(&address,0, sizeof(struct sockaddr_in));

/*name of socket agreed with*/
address.sin_family = AF_INET;
//address.sin_addr.s_addr =inet_addr(sendto_IP);
address.sin_port =htons(PORT);

ret=inet_aton(sendto_IP,&address.sin_addr);
if(ret)
perror("inet_aton");


//printf("address_sendto_ip= %s \n", address.sin_addr.s_addr );

/*
puts("Enter the number of digits \n");
scanf("%d",&n);
puts("Enter digits\n");
i=0;

while(i!=n){
scanf("%c",&buf[i]);
i++;
}*/



ret= sendto(hsock, buf, n,0,(struct sockaddr * ) &address, sizeof (struct sockaddr_in));
if(ret==-1)
{
perror("error send:");
puts(" \n");
}
else 
printf (" %d bytes sent to %s\n",ret,sendto_IP);


close(hsock);
exit(0);
}

