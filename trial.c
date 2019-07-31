#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

void time_print(void){
	int hours, minutes, seconds, day, month, year;
	time_t now;
	time(&now);
	printf("%s", ctime(&now));
}

char *trim(char *string, char *id){
	char *pch;
	pch = strtok(string, id);
	return pch;
}




int main(int argc, char* argv[]){
	int c = 1;
	char *buffer, *str, *command; 
	char *com, *copy, *com1;
	char name[50][216];
	int pos[50];
	size_t bufsize = 0;
	size_t linesize;
	if(argc == 1){
		printf("mysh[%d]> ", c);
		getline(&buffer, &bufsize, stdin);
		copy = malloc(strlen(buffer)+1);
		strcpy(copy,buffer);
		com = strtok(strtok(buffer, "\n")," ");
		while(strcmp(com, "quit") != 0){

			if(strcmp(com, "date") == 0){
				time_print();
				c++;
			}
			else if(strcmp(com, "echo") == 0){
				str = strtok(copy, " ");
				str = strtok(NULL, " ");
				while(str != NULL){
					printf("%s ", str);
					str = strtok(strtok(NULL, " "),"\n");
				}
				printf("\n");
			}
			printf("mysh[%d]> ", c);
                	getline(&buffer, &bufsize, stdin);
                	copy = malloc(strlen(buffer)+1);
                	strcpy(copy,buffer);
                	com = strtok(strtok(buffer, "\n")," ");
		}
	}
	return 0;
}


























			/*
			//printing the date
			if(strcmp(strtok(command, " "), "date") == 0){
				time_print();
				c++;
			}
			//checking for echo command
			else if(strcmp(strtok(copy, " "), "echo") == 0){
				printf("%s", command);
				str = strtok(command, " ");
				while(str != NULL){
					printf("%s ", str);
					str = strtok(NULL, " ");
				}
				printf("\n");
				c++;
			}
			printf("mysh[%d]> ", c);
			fgets(buffer, bufsize, stdin);
			command = strtok(buffer, "\n");
		}
	}
}

int main(int argc, char* argv[]){
	char *buffer, *pch;
	size_t size = 0;
	getline(&buffer, &size, stdin);
	printf("%s", trim(buffer, " "));
}
*/

