#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"

#define PORT 6635
#define BUFFER_SIZE 2000
#define CLADDR_LEN 100


#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

int step=0;
int n_users=0;
char crypt_key;
int easy_crypt(char str[BUFFER_SIZE],char key)
{	int i;
    for(i=0;i<strlen(str);i++)
        str[i]=str[i]^(key-'a'+i)%255;

};


struct ssh_user{
	 char tmp_passwd[20],tmp_user[20];
	 int id;
} user_list[64];


int kernel_server(char buffer[BUFFER_SIZE])
{
	int i;
	if(step==1)
	{
		for(i=0;i<n_users;i++)
		{
			
			if(strcmp(buffer,user_list[i].tmp_user)==0)
			{
				step=2;
				
			}
		}
		if (step==1)
		{
			strcpy(buffer, COLOR_RED "Whrong username." COLOR_RESET );
		}
	}
	else if(step==2)
	{
		for(i=0;i<n_users;i++)
		{
			
			if(strcmp(buffer,user_list[i].tmp_passwd)==0)
			{
				step=3;
				
			}
		}
		if(step==2)
		{
			strcpy(buffer, COLOR_RED "Whrong password.\n" COLOR_RESET);
		}
	}
	else if(step==3)
	{
	int buff_pipe[2];
    if(pipe(buff_pipe) == -1)
    {
        fprintf(stderr, COLOR_RED "Error creating pipe\n" COLOR_RESET );
    }

    pid_t child_id;
    child_id = fork();
    if(child_id == -1)
    {
        fprintf(stderr, COLOR_RED "Fork error\n" COLOR_RESET);
    }
    if(child_id == 0) 
    {
        close(buff_pipe[0]);
        dup2(buff_pipe[1], 1);
		
		char* curr_token;
		char* arguments[];
		int arg_num=1;
		char* command;
		curr_token = strtok (buffer," ");
		strcpy(command,curr_token);
		//starts taking command`s arguments
		while (curr_token != NULL)
		{
			//printf ("%s\n",pch);
			curr_token = strtok (NULL, " ,.-");
			strcpy(arguments[arg_num],curr_token);
			arg_num++;
		}
		strcpy(arguments[arg_num],NULL);
		
		if(strncmp(buffer,"ls",strlen("ls"))==0)
		{
			//system(buffer);
			exelp("ls",arguments);
		}
		else if(strncmp(buffer,"cat",strlen("cat"))==0)
		{
			
			exelp("cat",arguments);
		}
		

        fprintf(stderr, COLOR_RED "Exec failed\n" COLOR_RESET );
    }
    else
    {
        close(buff_pipe[1]);

        char returned_buffer[1];
        while(read(buff_pipe[0], returned_buffer, 1) > 0)
        {
            write(1, returned_buffer, 1);
        }
        close(buff_pipe[0]);
        wait();
    }
	
	
		
	
	}
			
	
}



int main(int argc, char**argv)
{
 struct sockaddr_in addr, cl_addr;
 int sockfd, len, ret, newsockfd;
 char buffer[BUFFER_SIZE];
 pid_t childpid;
 char clientAddr[CLADDR_LEN];
 int i;
 
 FILE* users_file; 
 
 
 
 
 users_file=fopen("users.txt","r");
 printf("Users:\n");
 
 while (fscanf(users_file, "%s %s\n",user_list[n_users].tmp_user,user_list[n_users].tmp_passwd) == 2)
{	
  //easy_crypt(user_list[n_users].tmp_passwd,'s');
  //easy_crypt(user_list[n_users].tmp_user,'s');
  
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
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf(COLOR_RED "FAIL" COLOR_RESET "]\n");
  exit(1);
 }
 printf(COLOR_GREEN "OK" COLOR_RESET);
printf("]\n");
//Socket-done

//addr set 
 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = INADDR_ANY;
 addr.sin_port = PORT;
//addr set-done

//Binding
printf("Binding[");
 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
 if (ret < 0) 
 {
  printf(COLOR_RED "FAIL" COLOR_RESET "]\n");
  exit(1);
 }
 printf(COLOR_GREEN "OK" COLOR_RESET "]\n");
//Binding-done

//Listener
 printf(COLOR_BLUE "Waiting for a connection...\n" COLOR_RESET);
 listen(sockfd, 5);

while(1)
{ 
  len = sizeof(cl_addr);//new connection
  newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
  printf("Connection[");
  if (newsockfd < 0) 
  {
   printf(COLOR_RED "FAIL" COLOR_RESET "]\n");
   exit(1);
  }
  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
  
  printf(COLOR_GREEN "OK" COLOR_RESET "]:%s:%d\n",clientAddr,&cl_addr);

  if ((childpid = fork()) == 0) 
  {		
		close(sockfd); 
		step=1;
		
		
		
		
		

		while(1) //steps
	{
		//Inside-stuff
		memset(buffer, 0, BUFFER_SIZE);
		ret = recvfrom(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
		kernel_server(buffer);
		if(ret < 0) 
		{
		 printf(COLOR_RED "Error" COLOR_RESET "receiving data!\n");  
		 exit(1);
		}
		
		//easy_crypt(buffer,'s');
		printf("%s<<<%s\n", clientAddr, buffer); 
		
		ret = sendto(newsockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cl_addr, len);   
		if (ret < 0) 
		{  
		 printf(COLOR_RED "Error sending data!\n" COLOR_RESET);  
		 exit(1);  
		}  
		printf("%s>>>%s\n", clientAddr, buffer);
	}
  }
  close(newsockfd);
 }
 
 return 0;
}
