#include<sys/socket.h>
#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>

#define Ver_Str  "1.2.13.02"

#define PORT 5099
#define SERVER_IP "192.168.11.9"


#define BUFFER_LEN 30

char buf[BUFFER_LEN]={0};


int main()
{
 int sockfd=0,ret=-1;
int i=0;
socklen_t len, sender_len=  sizeof (struct sockaddr_in);

struct sockaddr_in address,sender;

puts("CC3000 testing server program\n");
printf(" \nSocket programming mgr side for CC3000 testing,  version=%s \n", Ver_Str);


/*create unnamed socket */
sockfd =socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
printf("socket created - sockfd=%d \n", sockfd);



memset(&address,0, sizeof(struct sockaddr_in));

/*name of socket agreed with*/
address.sin_family = AF_INET;
ret=address.sin_addr.s_addr =inet_addr(SERVER_IP);
if(ret==-1) 
perror("error inet_addr");

address.sin_port =htons(PORT);

ret=bind(sockfd,(struct sockaddr *) &address, sizeof(address));
if(ret)	perror("Error bind: ");

//printf("\n");

while(1)
{
printf("\nwaiting....\n");
ret=recvfrom(sockfd, buf, sizeof(buf),0, (struct sockaddr *)&sender, &sender_len);
if(ret<0)
perror("\nerror recfrom:");
else
{
printf("\n %d bytes received: ",ret);
while(i<ret )
{
printf("%d",buf[i]);
//putchar(buf[i]);

i++;
}
printf("\n");
i=0;
}

 sender_len=  sizeof (struct sockaddr_in);
}


close(sockfd);
}


