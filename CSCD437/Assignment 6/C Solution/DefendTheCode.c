#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING 50

void getPassword(char*);
void getName(char* firstOrLast, char* name);
void getFilename(char* inOrOut, char* name);
void strip(char* array);
char * current_directory();
void combiner();

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

void combiner(char *firstName, char *lastName,char *inputFile, char *outputFile){
    char c;
	char * cwd = current_directory();
    FILE * input = findFile(inputFile, 1);
    FILE * output = NULL;
    FILE * checkUnique = findFile(outputFile, 1);
    if(checkUnique==NULL) {
        output = findFile(outputFile, 0);
    }
    else{
        printf("Output file %s already exists. Exiting.\n", outputFile);
        return;
    }

    if(input == NULL) {
        printf("Input file %s was not fount in directory %s.\n",inputFile,cwd);
        return;
    }

    char name[150];
    strncpy(name, firstName, sizeof(firstName));
    strncat(name, " ", 150);
    strncat(name, lastName, 150);

    fprintf(output, "First and last name: %s\n", name);
    fprintf(output, "Integer addition: \n");
    fprintf(output, "Integer multiplication: \n");

    while((c = (char) getc(input)) != EOF){
        fputc(c, output);
    }

    fclose(input);
    fclose(output);
	free(cwd);
}


char * current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char *cwd_tosend = (char *)malloc(sizeof(char) * sizeof(cwd)+1);
		stpncpy(cwd_tosend, cwd, sizeof(cwd));
        return cwd_tosend;
    }
    else {
        return NULL;
    }
}

FILE *  findFile(char* fileName, int flag){
    FILE * start = NULL;
    if(flag == 1){
        start = fopen(fileName,"r");
    }
    else if(flag == 0){
        start = fopen(fileName,"w+");
    }
    if(start!=NULL){
        return start;
    }

    return NULL;
}
