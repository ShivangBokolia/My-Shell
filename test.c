#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main(void){
	/*
	char **hold = malloc(sizeof(char*) * 64);
	hold[0] = "!11";
//	hold[1] = "why u there";
//	hold[2] = "how do u do";

	char *bang = malloc(sizeof(char) * 64);
	bang = strtok(hold[0], "!");
	printf("%s\n", bang);
}
*/	char *str = malloc(sizeof(char) * 64);
	size_t size = 0;
	int l = getline(&str, &size, stdin);
	//if(str[0] == '\n'){
	//	printf("NULL\n");
	printf("%d\n", l);
	
	}
	

