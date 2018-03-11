/*
 * mySSH: inter-machines communication (client-side)
 * Copyright (C) 2018  qxZap
 */
#include <stdio.h> 
#include <stdlib.h>  
#include <sys/types.h> 
#include <sys/socket.h>  
#include <string.h>
#include <netinet/in.h>  
#include <netdb.h>
  
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
  

#define BUFFER_SIZE 2000 
  
int easy_crypt(char str[BUFFER_SIZE],char key)
{	int i;
    for(i=0;i<strlen(str);i++)
        str[i]=str[i]^(key-'a')%255;

};
  
  
  
int main(int argc, char**argv) 
{  
 struct sockaddr_in address, client_address;  
 int socket_fd, returned;  
 char main_buffer[BUFFER_SIZE];  
 struct hostent * server;
 char * server_Address;

 if (argc < 3) 
 {
  printf("Syntax: %s <ip_address> <port>\n",argv[0]);
  exit(1);  
 }

 server_Address = argv[1]; 
 
  //Socket
 printf("Socket[");
 socket_fd = socket(AF_INET, SOCK_STREAM, 0);
 if (socket_fd < 0) {
  printf(COLOR_RED "FAIL" COLOR_RESET);
  exit(1);
 }
 printf(COLOR_GREEN "OK" COLOR_RESET);
printf("]\n");
//Socket-done  

 memset(&address, 0, sizeof(address));  
 address.sin_family = AF_INET;  
 address.sin_addr.s_addr = inet_addr(server_Address);
 address.sin_port = atoi(argv[2]);     

 printf("Connection[");
 returned = connect(socket_fd, (struct sockaddr *) &address, sizeof(address));  
 if (returned < 0)
 {  
  printf(COLOR_RED "FAIL" COLOR_RESET "]\n" COLOR_RED "Invalid host(check host/internet connection)\n");  
  exit(1);  
 }  
 printf(COLOR_GREEN "OK" COLOR_RESET "]\n");  

 memset(main_buffer, 0, BUFFER_SIZE);
 printf(">");

 while (fgets(main_buffer, BUFFER_SIZE, stdin) != NULL) 
{
  if(!strncmp(main_buffer,"disconnect",strlen("disconnect"))&&main_buffer[strlen("disconnect")+1]==0)
	{
		printf(COLOR_YELLOW "Exiting server\n" COLOR_RESET);
		easy_crypt(main_buffer,'s');
		returned = sendto(socket_fd, main_buffer, BUFFER_SIZE, 0, (struct sockaddr *) &address, sizeof(address));  
		if (returned < 0) 
		{  
			printf("Error sending data!\n\t-%s", main_buffer);  
		}
		exit(1);
	}

  //encrypt sent text
  easy_crypt(main_buffer,'s');
	
  returned = sendto(socket_fd, main_buffer, BUFFER_SIZE, 0, (struct sockaddr *) &address, sizeof(address));  
  if (returned < 0) 
  {  
   printf("Error sending data!\n\t-%s", main_buffer);  
  }
  returned = recvfrom(socket_fd, main_buffer, BUFFER_SIZE, 0, NULL, NULL);  
  if (returned < 0) 
  {  
   printf("Error receiving data!\n");    
  } 
  else 
  {
   easy_crypt(main_buffer,'s');
   //printf("Received: ");
   fputs(main_buffer, stdout);
   printf(">");
  }  
 }
 
 return 0;    
}