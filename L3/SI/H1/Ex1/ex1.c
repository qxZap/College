#include <openssl/conf.h>
#include <openssl/evp.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char iv[] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";



char *wordPadding(char *buffer) {
    int i = strlen(buffer);
    char *currentKey = (char *) malloc(50);
    strcpy(currentKey, buffer);
    for (; i < 16; i++) {
        strcat(currentKey, "\xff");
    }
    currentKey[i] = '\0';
    return currentKey;
}


unsigned char *encryptAES_CBC(unsigned char *plainText, int *len, unsigned char *keyData) {
    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(e, EVP_aes_128_cbc(), NULL, keyData, iv);
    int chipherLen = *len + 128;
    int fileLen = 0;
    unsigned char *cipherText = (unsigned char *) malloc(chipherLen);

    EVP_EncryptUpdate(e, cipherText, &chipherLen, plainText, *len);
    EVP_EncryptFinal_ex(e, cipherText + chipherLen, &fileLen);

    *len = chipherLen + fileLen;
    return cipherText;
}


unsigned char *encryptAES_ECB(unsigned char *plainText, int *len, unsigned char *keyData) {
    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(e, EVP_aes_128_ecb(), NULL, keyData, NULL);
    int chipherLen = *len + 128;
    int fileLen = 0;
    unsigned char *cipherText = (unsigned char *) malloc(chipherLen);

    EVP_EncryptUpdate(e, cipherText, &chipherLen, plainText, *len);
    EVP_EncryptFinal_ex(e, cipherText + chipherLen, &fileLen);

    *len = chipherLen + fileLen;
    return cipherText;
}

unsigned char *decryptAES_CBC(unsigned char *cipherText, int *len, unsigned char *keyData) {
    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(e, EVP_aes_128_cbc(), NULL, keyData, iv);
    int processLen = *len, fileLen = 0;
    unsigned char *plainText = (unsigned char *) malloc(processLen);
    EVP_DecryptUpdate(e, plainText, &processLen, cipherText, *len);
    EVP_DecryptFinal_ex(e, plainText + processLen, &fileLen);

    *len = processLen + fileLen;
    return plainText;
}

unsigned char *decryptAES_ECB(unsigned char *cipherText, int *len, unsigned char *keyData) {
    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(e, EVP_aes_128_ecb(), NULL, keyData, iv);
    int processLen = *len, fileLen = 0;
    unsigned char *plainText = (unsigned char *) malloc(processLen);
    EVP_DecryptUpdate(e, plainText, &processLen, cipherText, *len);
    EVP_DecryptFinal_ex(e, plainText + processLen, &fileLen);

    *len = processLen + fileLen;
    return plainText;
}



char *readFileContent(const char *filename) {
    FILE *target = fopen(filename, "r");
    fseek(target, 0, SEEK_END);
    long fileSize = ftell(target);
    fseek(target, 0, SEEK_SET);
    char *fileContent = (char *) malloc(fileSize + 1);

    fread(fileContent, fileSize, 1, target);
    fclose(target);
    return fileContent;

}




void removeNewLine(char *word) {
    if (word[strlen(word) - 1] == '\n') {
        word[strlen(word) - 1] = '\0';
    }
}

int main(int argc, char *argv[]) {
	char * option;
	char* plainText;
	int plainTextSize;
	if(argc!=5){
		printf("USAGE: %s <option> <key_number> <plaintext> <input>\noption: ECB or CBC\n",argv[0]);
		
		exit(1);
	}
	option = argv[1];
	plainText = readFileContent(argv[3]);
	plainTextSize = strlen(plainText);
	
	if (strcmp(option, "ECB") && strcmp(option, "CBC")) {
		printf("wrong method\n");
		exit(-1);
	}

	    
	    int keyNumber = atoi(argv[2]);
	    printf("%d th key was chosen\n", keyNumber);

	    FILE *dic = fopen("wordlist.txt", "r");
		if(dic == NULL){
			printf("wordlist.txt not found\n");
			exit(1);
		}		

	    char *word = NULL;
	    size_t wordLen = 0;

	    for (int i = 0; i < keyNumber; i++)
            getline(&word, &wordLen, dic);
	
		removeNewLine(word);
        printf("key: %s\n",word);
        unsigned char *keyData = wordPadding(word);
	    unsigned char *cipherText;

	    if (!strcmp(option, "ECB")) {
            cipherText = encryptAES_ECB((unsigned char *) plainText, &plainTextSize, keyData);
	    } else if (!strcmp(option, "CBC")) {
            cipherText = encryptAES_CBC((unsigned char *) plainText, &plainTextSize, keyData);
	    }


	    FILE *ciphetTextFile = fopen(argv[4], "w");
		if(ciphetTextFile == NULL){
			printf("%s not found\n",argv[4]);
			exit(1);
		}

	    fprintf(ciphetTextFile, "%s", (const char *) cipherText);
	    fclose(ciphetTextFile);
        printf("\n");


        FILE *dictionary = fopen("wordlist.txt", "r");
    	if(dictionary == NULL){
			printf("wordlist.txt not found\n");
			exit(1);
		}

        char *newPossibleKey = NULL;
        size_t newPossibleKeyLength = 0;
        size_t fileReadFD;
        int i = 0;

        while ((fileReadFD = getline(&newPossibleKey, &newPossibleKeyLength, dictionary)) != -1) {
            removeNewLine(newPossibleKey);
            newPossibleKey = wordPadding(newPossibleKey);
            unsigned char *possibleCipherText;
            int plainTextLength = strlen(plainText);

            if (!strcmp(option, "ECB")) {
                possibleCipherText = encryptAES_ECB((unsigned char *) plainText, &plainTextLength, (unsigned char *) newPossibleKey);
            } else if (!strcmp(option, "CBC")) {
                possibleCipherText = encryptAES_CBC((unsigned char *) plainText, &plainTextLength,  (unsigned char *) newPossibleKey);
            }

            if (strcmp((const char *) possibleCipherText, cipherText) == 0) {
                printf("\nKey Found:%s\n", newPossibleKey);
                    exit(0);
            }
        }

    fclose(dictionary);
    free(option);


		





	return 0;
}



