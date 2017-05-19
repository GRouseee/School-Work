#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING 50

void getPassword(char*);
void getName(char* firstOrLast, char* name);
void getFilename(char* inOrOut, char* name);
void strip(char* array);

int main(){
    char firstName[MAX_STRING], lastName[MAX_STRING], inputFile[MAX_STRING], outputFile[MAX_STRING], password[MAX_STRING];

    getName("first", firstName);
    getName("last", lastName);
	getPassword(password);
	getFilename("in", inputFile);
	getFilename("out", outputFile);

    printf("\r\n%s %s\r\n", firstName, lastName);
	printf("%s\r\n", inputFile);
	printf("%s\r\n", outputFile);

    return 0;
}

void getName(char* firstOrLast, char* name){
    int isValid = 0;
    char buff[MAX_STRING + 1];
    regex_t nameRegex;

	memset(buff, '\0', MAX_STRING);
	memset(name, '\0', MAX_STRING);
    regcomp(&nameRegex, "^[A-Za-z'-]{1,50}$", REG_EXTENDED);

    while (!isValid){
        printf("Enter your %s name (50 or fewer characters)\r\n", firstOrLast);
        
        if (fgets(buff, MAX_STRING, stdin)){
			strip(buff);

			if (!regexec(&nameRegex, buff, 0, 0, 0)){
				isValid = 1;
			}
			else{
				printf("%s", "Invalid name\r\n");
			}
		}
		else{
			printf("%s", "Invalid name\r\n");
		}
    }

    strncpy(name, buff, strlen(buff));
    regfree(&nameRegex);
}

void getPassword(char* password){
	int isValid = 0;
	char buff[MAX_STRING + 1];
	regex_t passwordRegex;

	memset(buff, '\0', MAX_STRING);
	memset(password, '\0', MAX_STRING);
	regcomp(&passwordRegex, "(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&=+*])(?=\\S+$).{8,}", REG_EXTENDED);

	while(!isValid){
		printf("Enter a password at least 8 characters long (must contain a lower case, upper case, a digit, and a special character)\r\n");

		if(fgets(buff, MAX_STRING, stdin)){
			strip(buff);

			if(!regexec(&passwordRegex, buff, 0, 0, 0)){
				isValid = 1;
			}
			else{
				printf("%s", "Invalid password\r\n");
			}
		}
		else{
			printf("%s", "Invalid password\r\n");
		}
	}

	strncpy(password, buff, strlen(buff));
	regfree(&passwordRegex);
}

void getFilename(char* inOrOut, char* name){
	int isValid = 0;
	char buff[MAX_STRING + 1];
	regex_t fileRegex;

	memset(buff, '\0', MAX_STRING);
	memset(name, '\0', MAX_STRING);
	regcomp(&fileRegex, "^[A-Za-z0-9]{1,50}(.txt)$", REG_EXTENDED);

	while(!isValid){
		printf("Enter an %sput file (must be a .txt file in the same directory as the application)\r\n", inOrOut);
		
		if (fgets(buff, MAX_STRING, stdin)){
			strip(buff);

			if (!regexec(&fileRegex, buff, 0, 0, 0)){
				isValid = 1;
			}

			else{
				printf("%s", "Invalid filename\r\n");
			}
		}

		else{
			printf("%s", "Invalid filename\r\n");
		}
	}

	strncpy(name, buff, strlen(buff));
    regfree(&fileRegex);
}

void strip(char *array){
	if(array == NULL){
		perror("array is null");
		exit(-99);
	}

	int len = strlen(array), x = 0;

	while(array[x] != '\0' && x < len){
	  if(array[x] == '\r')
		 array[x] = '\0';

	  else if(array[x] == '\n')
		 array[x] = '\0';

	  x++;
    }
}