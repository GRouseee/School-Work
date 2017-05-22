#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>

#define MAX_STRING 50

void getPassword(char* password, int type);
void getName(char* firstOrLast, char* name);
void getFilename(char* inOrOut, char* name);
void strip(char* array);
void validatePassword(char* attempt, char* verified);
char * current_directory();
void combiner(char *firstName, char *lastName,char *inputFile, char *outputFile,  long* intOne, long* intTwo);
FILE *  findFile(char*, int);
void getInt(long * theInt);
void discardExtraSTDIN();
long addInts(long intOne, long intTwo);
long multInts(long intOne, long intTwo);

int main(){
    char firstName[MAX_STRING], lastName[MAX_STRING], inputFile[MAX_STRING], outputFile[MAX_STRING], password[MAX_STRING], verifyPassword[MAX_STRING];
    long intOne=0, intTwo=0;

    getName("first", firstName);
    getName("last", lastName);
    getInt(&intOne);
    getInt(&intTwo);
    getFilename("in", inputFile);
    getFilename("out", outputFile);
//	getPassword(password, 1);
//	getPassword(verifyPassword, 2);
//	validatePassword(password, verifyPassword);

    combiner(firstName, lastName, inputFile, outputFile, &intOne, &intTwo);

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
                if(strlen(buff)==MAX_STRING-1){
                    discardExtraSTDIN();
                }


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

void getPassword(char* password, int type){
	int isValid = 0;
	char buff[MAX_STRING + 1];
	regex_t passwordRegex;

	memset(buff, '\0', MAX_STRING);
	memset(password, '\0', MAX_STRING);
	regcomp(&passwordRegex, "(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&=+*])(?=\\S+$).{8,}", REG_EXTENDED);

	while(!isValid){
		if(type == 1)
			printf("Enter a password at least 8 characters long (must contain a lower case, upper case, a digit, and a special character)\r\n");
		
		if(type == 2)
			printf("Re-enter password: ");
		
		if(fgets(buff, MAX_STRING, stdin)){
            if(strlen(buff)==MAX_STRING-1){
                discardExtraSTDIN();
            }
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

void validatePassword(char* attempt, char* verified){
	int ctr = 0;
	
	while(ctr != 1){
		if(strncmp(attempt, verified, strlen(verified)) == 0){
			printf("Password valid.");
			ctr = 1;
		}
		else{
			printf("%s", "Password invalid. Try again: \r\n");
			getPassword(attempt,2);
		}
	}
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
            if(strlen(buff)==MAX_STRING-1){
                discardExtraSTDIN();
            }

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

	size_t len = strlen(array), x = 0;

	while(array[x] != '\0' && x < len){
		if(array[x] == '\r')
		 	array[x] = '\0';

	  	else if(array[x] == '\n')
		 	array[x] = '\0';
		x++;
	}
}

/**
 * Gets input from user to get an Int value. To ensure no crashes it is stored in a long variable.
 * after getting the user input, it is cast to a long value and checked if within the bounds of an
 * int value (-2147483648 to 2147483647).
 * @param theInt
 */
void getInt(long * theInt){
    int isValid = 0;
    char buff[MAX_STRING +1];
    regex_t fileRegex;

    memset(buff,'\0', MAX_STRING);
    regcomp(&fileRegex, "((^[-]?)[1-9][0-9]?){1,10}", REG_EXTENDED);

    while(!isValid){
        printf("Enter an integer value:\r\n");
        if(fgets(buff,MAX_STRING, stdin)) {
            if(strlen(buff)==MAX_STRING-1){
                discardExtraSTDIN();
            }

            if(!regexec(&fileRegex, buff, 0,0,0)){
                *theInt = atol(buff);
                if(*theInt <= 2147483647 && *theInt >= -2147483648){
                    isValid = 1;
                }
                else {
                    printf("Entered value is not within the bounds of -2147483648 to 2147483647.\n");
                }
            }
            else{
                printf("Invalid integer value.\n");
            }
        }
    }
}

/**
 * Takes the parameters gathered from the user input and compiles them into a file specified by the outputFile param,
 * followed by the contents of the inputFile param assuming outputFile is a unique file and inputFile exists.
 * Returns early if outputFile already exists or inputFile does not exist.
 * @param firstName
 * @param lastName
 * @param inputFile
 * @param outputFile
 */
void combiner(char *firstName, char *lastName,char *inputFile, char *outputFile, long* intOne, long* intTwo){
    char c;
	char * cwd = current_directory();
    FILE * input = findFile(inputFile, 1);
    FILE * output = NULL;
    FILE * checkUnique = findFile(outputFile, 1);

    if(input == NULL) {
        printf("Input file %s was not found in directory %s. FAILED TO CREATE %s.\n",inputFile,cwd, outputFile);
        free(cwd);
        return;
    }

    if(checkUnique==NULL) {
        output = findFile(outputFile, 0);
    }
    else{
        printf("Output file %s already exists. EXITING.\n", outputFile);
        free(cwd);
        return;
    }


    char name[150];
    strncpy(name, firstName, strlen(firstName));
    strncat(name, " ", 150);
    strncat(name, lastName, 150);

    fprintf(output, "First and last name: %s\n", name);
    fprintf(output, "Integer addition: %ld\n",addInts(*intOne, *intTwo));
    fprintf(output, "Integer multiplication: %ld\n",multInts(*intOne, *intTwo));

    while((c = (char) getc(input)) != EOF){
        fputc(c, output);
    }

    fclose(input);
    fclose(output);
	free(cwd);
}

/**
 * Gets the cwd and puts it in a char*
 * @return char *
 */
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

/**
 * Tries to open fileName and returns the opened filed pointer or NULL if couldnt be found.
 * @param fileName
 * @param flag
 * @return FILE
 */
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

/**
 * Clears extra input from stdin.
 */
void discardExtraSTDIN() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return;
}


long addInts(long intOne, long intTwo){
    return intOne + intTwo;
}

long multInts(long intOne, long intTwo){
    return intOne * intTwo;
}
