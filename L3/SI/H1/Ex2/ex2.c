#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


void compareFiles(char* firstFile, char* secondFile) 
{ 
printf("Comparing  %s with %s\t", firstFile,secondFile); 	
FILE *fp1 = fopen(firstFile,"r");
FILE *fp2 = fopen(secondFile,"r");

if (fp1 == NULL || fp2 == NULL )
    { 
       printf("Error : Files not open"); 
       exit(0); 
    } 
    char ch1 = getc(fp1); 
    char ch2 = getc(fp2); 
  
    int same = 0, pos = 0; 
  
    while (ch1 != EOF && ch2 != EOF) 
    { 
        pos++; 
        if (ch1 == '\n' && ch2 == '\n') 
        { 
            pos = 0; 
        } 
        else if (ch1 == ch2) 
        { 
            same++; 
        } 
  
        ch1 = getc(fp1); 
        ch2 = getc(fp2); 
    } 
  
    printf("Equal bytes : %d\n", same); 

    fclose(fp1);
	fclose(fp2);
} 



void createIfNotExistsFile(char* filename)
{
	FILE *fptr;
	fptr = fopen(filename, "w");
	if(fptr == NULL)
	{
		remove(filename);
		fptr = fopen(filename, "a");
	}
	fclose(fptr);
	

}

void writeInFile(char*  buffer,char* filename){
	FILE *file = fopen(filename,"w+");
	fprintf(file,"%s",buffer);
	fclose(file);
}


void writeMD5InFile(char* input,char* output){
createIfNotExistsFile(output);
FILE *fh;
  long filesize;
  unsigned char *buf;
  unsigned char *md5_result = NULL;
unsigned char* buffer = (char*)malloc(sizeof(char)*MD5_DIGEST_LENGTH);
unsigned char* secondBuffer = (char*)malloc(sizeof(char)*MD5_DIGEST_LENGTH);
  int i;


fh = fopen(input, "r");
  fseek(fh, 0L, SEEK_END);
  filesize = ftell(fh);
  fseek(fh, 0L, SEEK_SET);
  buf = malloc(filesize);
  fread(buf, filesize, 1, fh);
  fclose(fh);
  md5_result = malloc(MD5_DIGEST_LENGTH);
  MD5(buf, filesize, md5_result);

	strcpy(buffer,"");
  for (i=0; i < MD5_DIGEST_LENGTH; i++)
  {
	sprintf(secondBuffer,"%02x",md5_result[i]);
	strcat(buffer,secondBuffer);  
  }
writeInFile(buffer,output);
  free(md5_result);
  free(buf);
  free(buffer);


}




void writeSHAInFile(char* input,char* output){
createIfNotExistsFile(output);
FILE *fh;
  long filesize;
  unsigned char *buf;
  int i;


fh = fopen(input, "r");
  fseek(fh, 0L, SEEK_END);
  filesize = ftell(fh);
  fseek(fh, 0L, SEEK_SET);
  buf = malloc(filesize);
  fread(buf, filesize, 1, fh);
  fclose(fh);


unsigned char digest[SHA256_DIGEST_LENGTH];
 
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, buf, strlen(buf));
    SHA256_Final(digest, &ctx);
 
    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
 



writeInFile(mdString,output);
  free(buf);


}





void createMD5files(char* firstFile,char* secondFile){
writeMD5InFile(firstFile,"hash1_md5");
writeMD5InFile(secondFile,"hash2_md5");
}


void createSHAfiles(char* firstFile,char* secondFile){
writeSHAInFile(firstFile,"hash1_sha256");
writeSHAInFile(secondFile,"hash2_sha256");
}







int main(int argc, char**argv)
{
  if (argc < 2)
  {
    printf("Usage: %s <filename1> <filename2>\n",argv[0]);
    return 1;
  }
  createMD5files(argv[1],argv[2]);
  createSHAfiles(argv[1],argv[2]);

  compareFiles("hash1_md5","hash2_md5");
compareFiles("hash1_sha256","hash2_sha256");

  return 0;
}
