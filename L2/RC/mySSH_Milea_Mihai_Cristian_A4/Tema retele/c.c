#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
  
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
  
  
#define PORT 6635
#define BUFFER_SIZE 2000 
  
int easy_crypt(char str[BUFFER_SIZE],char key)
{	int i;
    for(i=0;i<strlen(str);i++)
        str[i]=str[i]^(key-'a'+i)%255;

};
  
  
  
int main(int argc, char**argv) 
{  
 struct sockaddr_in addr, cl_addr;  
 int sockfd, ret;  
 char buffer[BUFFER_SIZE];  
 struct hostent * server;
 char * serverAddr;

 exec("nslookup google.com");
 if (argc < 2) 
 {
  printf("Syntax: %s <ip_address>\n",argv[0]);
  exit(1);  
 }

 serverAddr = argv[1]; 
 
  //Socket
 printf("Socket[");
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf(COLOR_RED "FAIL" COLOR_RESET);
  exit(1);
 }
 printf(COLOR_GREEN "OK" COLOR_RESET);
printf("]\n");
//Socket-done  

 memset(&addr, 0, sizeof(addr));  
 addr.sin_family = AF_INET;  
 addr.sin_addr.s_addr = inet_addr(serverAddr);
 addr.sin_port = PORT;     

 printf("Connection[");
 ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
 if (ret < 0)
 {  
  printf(COLOR_RED "FAIL" COLOR_RESET "]\n");  
  exit(1);  
 }  
 printf(COLOR_GREEN "OK" COLOR_RESET "]\n");  

 memset(buffer, 0, BUFFER_SIZE);
 printf("Input: ");

 while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) 
{
  ret = sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));  
  if (ret < 0) 
  {  
   printf("Error sending data!\n\t-%s", buffer);  
  }
  ret = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);  
  if (ret < 0) 
  {  
   printf("Error receiving data!\n");    
  } 
  else 
  {
   easy_crypt(buffer,'s');
   printf("Received: ");
   fputs(buffer, stdout);
   printf("\nInput: ");
  }  
 }
 
 return 0;    
}