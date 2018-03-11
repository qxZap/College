/*
 * mySSH: inter-machines communication (server-side)
 * Copyright (C) 2018  qxZap
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>

#define BUFFER_SIZE 2000
#define CLADDR_LEN 100
#define PATH_LEN 1024

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

int n_users=0;
int step=1;
int user_id;
int kick=3;
static char *propStringDuplicate(char *temp_string) {
    char *next_one = malloc (strlen (temp_string) + 1);
    if (next_one)
        strcpy (next_one, temp_string);
    return next_one;
}

int easy_crypt(char str[BUFFER_SIZE],char key)
{	int i;
    for(i=0;i<strlen(str);i++)
        str[i]=str[i]^(key-'a')%255;

};


struct ssh_user{
	 char tmp_passwd[20],tmp_user[20];
	 int id;
} user_list[64];

int kernel_server(char ker_buffer[BUFFER_SIZE])
{
	//printf("Debug x1: %s",ker_buffer);
	int i;
	if(step==1)
	{
		for(i=0;i<n_users;i++)
		{
			//first i was using strcmp and didn`t work that well. 
			//printf("Debug x2: %s",ker_buffer[strlen(user_list[i].tmp_user)+1]);
			if(!strncmp(ker_buffer,user_list[i].tmp_user,strlen(user_list[i].tmp_user))&&ker_buffer[strlen(user_list[i].tmp_user)+1]==0)
			{
				step=2;
				strcpy(ker_buffer,COLOR_MAGENTA "Password for ");
				strcat(ker_buffer,user_list[i].tmp_user);
				strcat(ker_buffer, COLOR_RESET);
				user_id=i;
				i+=n_users;
				
			}
		}
		if (step==1)
		{
			strcpy(ker_buffer, COLOR_RED "Wrong username.\n" COLOR_RESET );
			kick--;
		}
	}
	else if(step==2)
	{
		
			
			if(!strncmp(ker_buffer,user_list[user_id].tmp_passwd,strlen(user_list[user_id].tmp_passwd))&&ker_buffer[strlen(user_list[user_id].tmp_passwd)+1]==0)
			{
				step=3;
				strcpy(ker_buffer,COLOR_GREEN "Access granted! Welcome " COLOR_CYAN);
				strcat(ker_buffer,user_list[user_id].tmp_user);
				strcat(ker_buffer, COLOR_RESET);
				strcat(ker_buffer,"\n");
				i+=n_users;
			}
		
		if(step==2)
		{
			strcpy(ker_buffer, COLOR_RED "Wrong password.\n" COLOR_RESET);
			kick--;
		}
	}
	else if(step==3)
	{
	//here it`s goin` to split the ker_buffer into words used as arguments next time needed
	char back_up_buffer[BUFFER_SIZE];
	strcpy(back_up_buffer,ker_buffer);
    char *arguments[100];
    int number_of_arguments = 0;
	int cased=0;
    char *temp_token = strtok (ker_buffer, " ");
    while (temp_token != NULL) {
        arguments[number_of_arguments++] = propStringDuplicate(temp_token);
		temp_token=strtok(NULL, " ");
    }
    arguments[number_of_arguments]=NULL;
	strcpy(ker_buffer,back_up_buffer);
	memset(back_up_buffer,0, sizeof(char)*BUFFER_SIZE );
	
    char* test_case;
	test_case=strchr(ker_buffer,'<');
	if(test_case!=NULL)
		cased=1;
	test_case=strchr(ker_buffer,'>');
	if(test_case!=NULL)
		cased=1;
	test_case=strchr(ker_buffer,'|');
	if(test_case!=NULL)
		cased=1;
	test_case=strchr(ker_buffer,';');
	if(test_case!=NULL)
		cased=1;
	test_case=strstr(ker_buffer,"2>");
	if(test_case!=NULL)
		cased=1;
	test_case=strstr(ker_buffer,"&&");
	if(test_case!=NULL)
		cased=1;
	test_case=strstr(ker_buffer,"||");
	if(test_case!=NULL)
		cased=1;


	char returned_path[PATH_LEN];
	if(!strncmp(ker_buffer,"disconnect",strlen("disconnect"))&&ker_buffer[strlen("disconnect")+1]==0)
		{
			printf(COLOR_YELLOW "%s logged out\n" COLOR_RESET,user_list[user_id].tmp_user);
			exit(1);
		}
	else if(!strncmp(ker_buffer,"cd",strlen("cd")))
		{
			char* tmp_dir_mode=ker_buffer+3;
			getcwd(returned_path, sizeof(returned_path));
			strcat(returned_path,"/");
			strcat(returned_path,tmp_dir_mode);
			chdir(returned_path);
			//printf(COLOR_CYAN "Debug x3:%s changed dir to %s\n" COLOR_RESET ,user_list[user_id].tmp_user,returned_path);
		}
	else if(!strncmp(ker_buffer,"pwd",strlen("pwd")))
		{
			getcwd(returned_path, sizeof(returned_path));
		}
	else if(!strncmp(ker_buffer,"help",strlen("help")))
		{
			char * help = "Available commands:\n\
    disconnect          logout from server\n\
    cd          		changes working directory\n\
    help    			prints current messege\n\
    dir       			prints files & directories from the current directory\n\
    pwd    				prints path\n\
    mv   				moves file to another location\n\
    mkdir    			creates directory\n\
    rmdir    			deletes a directory\n\
    rm    				deletes a file\n\
	echo    			prints a string\n\
    ls    				lists files from current directory\n\
    wget    			downloads a file\n\
    who		   			shows connected users\n\
    mcedit    			edits a file with mc editor\n\
    nslookup   			prints info about a host\n\
    touch   			creates a file\n\
    zip		   			packs an achive\n\
    unzip    			unpacks an archive\n\
    chmod   			changes user rights\n\
    cp   				copy file/directory to location\n";
		strcpy(ker_buffer,help);
		return 0;
		}
	

	

	int tmp_file = open("tmp.tmp", O_WRONLY|O_CREAT, 0666); //creating/using tmp file for transfer
	if (!fork()) 
	{
	  close(1);
	  dup(tmp_file); //redirecting stdout to tmp.tmp
	if(!cased)
	{	if(!strncmp(ker_buffer,"pwd",strlen("pwd")))
		{
				printf("%s\n",returned_path);
		}
		else if(!strncmp(ker_buffer,"ls",strlen("ls")))
		{
			execvp("ls",arguments);
		}
		else if(!strncmp(ker_buffer,"dir",strlen("dir")))
		{
			execvp("dir",arguments);
		}
		else if(!strncmp(ker_buffer,"nslookup",strlen("nslookup")))
		{
			execvp("nslookup",arguments);
		}
		else if(!strncmp(ker_buffer,"stat",strlen("stat")))
		{
			execvp("stat",arguments);
		}
		else if(!strncmp(ker_buffer,"touch",strlen("touch")))
		{
			execvp("touch",arguments);
		}
		else if(!strncmp(ker_buffer,"rm",strlen("rm")))
		{
			execvp("rm",arguments);
		}
		else if(!strncmp(ker_buffer,"cat",strlen("cat")))
		{
			execvp("cat",arguments);
		}
		else if(!strncmp(ker_buffer,"cp",strlen("cp")))
		{
			execvp("cp",arguments);
		}
		else if(!strncmp(ker_buffer,"mv",strlen("mv")))
		{
			execvp("mv",arguments);
		}
		else if(!strncmp(ker_buffer,"rmdir",strlen("rmdir")))
		{
			execvp("rmdir",arguments);
		}
		else if(!strncmp(ker_buffer,"mkdir",strlen("mkdir")))
		{
			execvp("mkdir",arguments);
		}
		else if(!strncmp(ker_buffer,"wget",strlen("wget")))
		{
			execvp("wget",arguments);
		}
		else if(!strncmp(ker_buffer,"who",strlen("who")))
		{
			execvp("who",arguments);
		}
		else if(!strncmp(ker_buffer,"zip",strlen("zip")))
		{
			execvp("zip",arguments);
		}
		else if(!strncmp(ker_buffer,"unzip",strlen("unzip")))
		{
			execvp("unzip",arguments);
		}
		else if(!strncmp(ker_buffer,"chmod",strlen("chmod")))
		{
			execvp("chmod",arguments);
		}
		else if(!strncmp(ker_buffer,"echo",strlen("echo")))
		{
			execvp("echo",arguments);
		}
		else if(!strncmp(ker_buffer,"grep",strlen("grep")))
		{
			execvp("grep",arguments);
		}
		else if(!strncmp(ker_buffer,"wc",strlen("wc")))
		{
			execvp("wc",arguments);
		}
		else
			execvp(arguments[0],arguments);
	}
	else 
		system(ker_buffer);
		//printf("Debug x9:%s\n",ker_buffer);
		exit(1);
	} 
	else 
	{
	  close(tmp_file); //closing tmp.tmp (for now)
	  wait(NULL);
	}
	//Now stdout went to tmp.tmp. Time to read it

	FILE    *temporary_file;
	char    *tmp_buffer;
	long    number_of_bytes;
	
	temporary_file = fopen("tmp.tmp", "r");
 
//gotta see if there is still the tmp file 
	if(temporary_file == NULL)
    {
		printf("tmp.tmp[" COLOR_RED "FAIL" COLOR_RESET "]");
	}
 
//get number of bytes
fseek(temporary_file, 0L, SEEK_END);
number_of_bytes = ftell(temporary_file);
//reset indicator to the begining of the file
fseek(temporary_file, 0L, SEEK_SET);	
//allocationg enough memory for the temporary buffer
tmp_buffer = (char*)calloc(number_of_bytes, sizeof(char));	
 
//memory error
if(tmp_buffer == NULL)
    if(temporary_file == NULL)
    {
		printf("Memory[" COLOR_RED "FAIL" COLOR_RESET "]");
	}
 
//text -> buffer
fread(tmp_buffer, sizeof(char), number_of_bytes, temporary_file);
fclose(temporary_file);
//from tmp_buffer to ker_buffer
strcpy(ker_buffer,tmp_buffer);
//printf(COLOR_BLUE "Debug x5:BUFFER redr from tmp to ker:\n%s\n" COLOR_RESET,tmp_buffer);
//printf(COLOR_BLUE "Debug x6:BUFFER copyed to ker:\n%s\n" COLOR_RESET,ker_buffer);
//gettin free the space for rewrite
free(tmp_buffer);
remove("tmp.tmp");

 }


	
}



int main(int argc, char**argv)
{
 struct sockaddr_in address, client_address;
 int socket_fd, len, returned, new_socket_fd;
 char main_buffer[BUFFER_SIZE];
 pid_t childpid;
 char client_Address[CLADDR_LEN];
 int i;
 
 FILE* users_file; 

 if (argc < 2) 
 {
  printf("Syntax: %s <port>\n",argv[0]);
	exit(1);  
 }
 
 
 
 users_file=fopen("users.txt","r");
 printf("Users:\n");
 while (fscanf(users_file, "%s %s\n",user_list[n_users].tmp_user,user_list[n_users].tmp_passwd) == 2)
{	
  easy_crypt(user_list[n_users].tmp_passwd,'s');
  easy_crypt(user_list[n_users].tmp_user,'s');
  
  user_list[n_users].id=n_users;
  n_users++;
}
for(i=0;i<n_users;i++)
{
	printf("%d:%s-%s\n",user_list[i].id,user_list[i].tmp_user,user_list[i].tmp_passwd);
	//easy_crypt(user_list[i].tmp_user,'s');
	//easy_crypt(user_list[i].tmp_passwd,'s');
	//printf("%d:%s-%s\n",user_list[i].id,user_list[i].tmp_user,user_list[i].tmp_passwd);
}

 fclose(users_file);
 
 //Socket
 printf("Socket[");
 socket_fd = socket(AF_INET, SOCK_STREAM, 0);
 if (socket_fd < 0) {
  printf(COLOR_RED "FAIL" COLOR_RESET "]\n");
  exit(1);
 }
 printf(COLOR_GREEN "OK" COLOR_RESET);
printf("]\n");
//Socket-done

//addr set 
 memset(&address, 0, sizeof(address));
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = INADDR_ANY;
 address.sin_port = atoi(argv[1]);
//addr set-done

//Binding
printf("Binding[");
 returned = bind(socket_fd, (struct sockaddr *) &address, sizeof(address));
 if (returned < 0) 
 {
  printf(COLOR_RED "FAIL" COLOR_RESET "]\n");
  exit(1);
 }
 printf(COLOR_GREEN "OK" COLOR_RESET "]\n");
//Binding-done

//Listener
 printf(COLOR_BLUE "Waiting for a connection...\n" COLOR_RESET);
 listen(socket_fd, 5);

while(1)
{ 
  len = sizeof(client_address);//new connection
  new_socket_fd = accept(socket_fd, (struct sockaddr *) &client_address, &len);
  printf("Connection[");
  if (new_socket_fd < 0) 
  {
   printf(COLOR_RED "FAIL" COLOR_RESET "]\n");
   exit(1);
  }
  inet_ntop(AF_INET, &(client_address.sin_addr), client_Address, CLADDR_LEN);
  //inet_ntop(AF_INET, &(client_address.sin_addr), client_Address, strlen(client_Adress));
  
  printf(COLOR_GREEN "OK" COLOR_RESET "]:%s:%d\n",client_Address,&client_address);

  if ((childpid = fork()) == 0) 
  {		close(socket_fd); 
		
		
		
		
		
		

		while(1)//step 3
		{
		//Inside-stuff
		memset(main_buffer, 0, BUFFER_SIZE);
		returned = recvfrom(new_socket_fd, main_buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_address, &len);
		if(returned < 0) 
		{
		 printf(COLOR_RED "Error" COLOR_RESET "receiving data!\n");  
		 exit(1);
		}
		easy_crypt(main_buffer,'s');
		printf("%s<<<%s", client_Address, main_buffer); 
		
		char *position;								////
		if ((position=strchr(main_buffer, '\n')) != NULL)//||This is going to delete the new line at the end of the command. Command recived looks like '..... \n\0'. And that \n is my problem.
				*position = '\0';					////
		
		kernel_server(main_buffer);
		if(kick==0)
			strcpy(main_buffer,COLOR_RED "You tryed to many times. You`ve been kicked from the server" COLOR_RESET);
		//printf("Debug x7: notCrypted Sending\n%s\n",buffer);
		easy_crypt(main_buffer,'s');
		//printf("Debug x8: Crypted Sent\n%s\n",buffer);
		
		returned = sendto(new_socket_fd, main_buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_address, len);   
		if (returned < 0) 
		{  
		 printf(COLOR_RED "Error sending data!\n" COLOR_RESET);  
		 exit(1);  
		}  
		printf("%s>>>%s", client_Address, main_buffer);
		if(kick==0)
			exit(1);
	   }
  }
  close(new_socket_fd);
 }
 
 return 0;
}
