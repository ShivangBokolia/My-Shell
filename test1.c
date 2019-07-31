#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"


void main(){
	int bufsize = LSH_TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *line = malloc(bufsize * sizeof(char*));
	char *token;

	strcpy(line, "echo hello there");

	token = strtok(line, LSH_TOK_DELIM);
	while(token != NULL){
		tokens[position] = token;
		position++;
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	for(int i=0; i<position; i++){
		printf("%s ",tokens[i]);
	}
}	
