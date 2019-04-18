#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <unistd.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int execution(char **args){
        pid_t pid, wpid;
        int status, flag = 0;

        pid = fork();
        if(pid == 0){
                if(execvp(args[0], args) == -1){
        //              fprintf(stderr,"%s: No such file or directory\n", args[0]);
                        flag = 1;
                }
        }else{
                do{
                        wpid = waitpid(pid, &status, WUNTRACED);
                }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        if(flag == 0){
                return 1;
        }else{
                return 0;
        }
}



int help(char *args[]){
	printf("Welcome to Mysh!\n");
	printf("Type your command name along with arguments, and hit enter.\n");
	printf("Enjoy your shell\n");
	return 1;
}

/*
int dis_history(int argc, char **args){
	if(argc<10){
		for(int i=0; i<argc; i++){
			printf("\t%d: %s\n", i+1, args[i]);
		}
	}else{
		for(int i=argc-11; i<argc; i++){
			printf("\t%d: %s\n", i+1, args[i]);
		}
	}
}
*/

int dis_history(int argc, char **args){
	while(args[argc] != NULL){
		printf("\t%d: %s\n", argc+1, args[argc]);
		argc++;
	}
	return 1;
}
/*
int bang(int argc, char *args[]){
	int status;
	if(strcmp(args[argc-1], "help") == 0){
        	status = help(args);
        }
	else if(strcmp(args[argc-1], "history") == 0){
		status = dis_history(argc, args);
	}
	else if(args[argc-1][0] == '!'){
		status = bang(args[argc-1][1], args);
		printf("Banged\n");
	}else{
		status = execution(args);		
	}
//	if(status == 0){
  //      	fprintf(stderr, "%s: No such file or directory\n", args[0]);
    //            fprintf(stderr, "command status: %d\n", argc);
      // 	}
	return status;
}
*/


char* add_history(char* line){
	return strtok(line, "\n");
}

char* read_input(void){
	char *command;
	size_t bufsize = 0;
	getline(&command, &bufsize, stdin);
	return command;
}

char** get_tokens(char* line){
	int bufsize = LSH_TOK_BUFSIZE;
        int position = 0;
        char **tokens = malloc(bufsize * sizeof(char*));
        char *token;

        token = strtok(line, LSH_TOK_DELIM);
        
	while(token != NULL){
                tokens[position] = token;
                position++;
                token = strtok(NULL, LSH_TOK_DELIM);
        }
        
	tokens[position] = NULL;
	return tokens;
}

int launch(char **args){
        if(strcmp(args[0], "help") == 0){
                return help(args);
        }
        return execution(args);
}



void loop(int his_num){
	char *buffer, *copy;
	int status;
	char **args;
	char **history = malloc(sizeof(char*) * LSH_TOK_BUFSIZE);
	int position = 0;
	int c = 1;
	do{
		printf("mysh[%d]> ", c);
		buffer = read_input();
		history[position] = add_history(strdup(buffer));
		args = get_tokens(buffer);
		if(strcmp(args[0], "help") == 0){
			status = help(args);
		}else if(strcmp(args[0], "history") == 0){
			if(c <= his_num){
				status = dis_history(0, history);
			}else{
				status = dis_history(c-his_num, history);
			}
		}else if(strcmp(args[0], "quit") == 0){
			status = -1;
			exit(0);
		}else{
			status = execution(args);
		}
		if(status == 0){
			fprintf(stderr, "%s: No such file or directory\n", args[0]);
			fprintf(stderr, "command status: %d\n", c);
		}
		c++;
		position++;
	}while(status != -1);
}


int main(int argc, char *argv[]){
        if(argc == 1){
		loop(10);
	}else if(argc == 2){
		//make verbose here
	}else if(argc == 3){
		loop(atoi(argv[2]));
	}else if(argc == 4){
		//make complete with verbose and history
	}
        return EXIT_SUCCESS;
}




