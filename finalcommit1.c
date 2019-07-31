///@author: SHIVANG BOKOLIA
//MY SHell Project 3
//CSCI 243
#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <unistd.h>

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

///this function is used to run all the external commands that can be used in LINUX
//if the pid is 0 then the child is created else the parents wait for the child to finish
int execution(int argc, char *argv[]){
        pid_t pid;
        int status;
	//creating pid using fork
        pid = fork();
        if(pid == 0){
		//child process created
                if(execvp(argv[0], argv) == -1){
			//if -1 the function doesn't exist
			perror(argv[0]);
			fprintf(stderr, "command status: %d\n", argc);
		}	
		exit(EXIT_FAILURE);
        }
	//is fork fails
	else if(pid < 0){
		perror("fork failed");
	}
	//the parent waits for the child to be done
	else{
                do{
                        waitpid(pid, &status, WUNTRACED);
                }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1; 
}

///This function is similar to the previous one, but is used for printing during
//verbose being on. 
int executionVerbose(int argc, char *argv[]){
        pid_t pid;
        int status;
	//creating pid using fork
        pid = fork();
        if(pid == 0){
		//child process created
                if(execvp(argv[0], argv) == -1){
			//if -1 the function doesn't work
                        perror(argv[0]);
                        fprintf(stderr, "command status: %d\n", argc);
                }
                exit(EXIT_FAILURE);
        }
	//if fork fails
        else if(pid < 0){
                perror("fork failed");
        }
	//the parent waits for the child to be done
        else{
		printf("\twait for pid %d: %s\n", pid, argv[0]);
                printf("\texecvp: %s\n", argv[0]);
                do{
                        waitpid(pid, &status, WUNTRACED);
                }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
	printf("command status:
        return 1;
}

///this function is used for verbose printing statements. 
int verbose_print(int argc, char *argv[]){
	(void)argc;
	int len = 0;
	printf("\tcommand: ");
	while(argv[len] != NULL){
		printf("%s ",argv[len]);
		len++;
	}
	printf("\n\n");
	printf("\tinput command tokens:\n");
	for(int i=0; i<len; i++){
		printf("\t%d: %s\n", i, argv[i]);
	}
	return 1;
}

int help(int argc, char *argv[]){
	(void)argc;
	(void)argv;
	printf("Welcome to Mysh!\n");
	printf("Type your command name along with arguments, and hit enter.\n");
	printf("Enjoy your shell\n");
	return 1;
}


int displayHistory(int argc, char *argv[]){
	while(argv[argc] != NULL){
		printf("\t%d: %s\n", argc+1, argv[argc]);
		argc++;
	}
	return 1;
}

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


void loop(int argc, char *argv[]){
	char *buffer;
	int status, verbose, his_num;
	char **args;
	char *bangNum = malloc(sizeof(char) * LSH_TOK_BUFSIZE);
	char **history = malloc(sizeof(char*) * LSH_TOK_BUFSIZE);
	int position = 0;
	int c = 1;
	if(argc == 1){
		verbose = 0;
		his_num = 10;
	}
	else if(argc == 2){
		verbose = 1;
		his_num = 10;
	}
	else if(argc == 3){
		verbose = 0;
		his_num = atoi(argv[2]);
	}
	else if(argc == 4){
		verbose = 1;
		his_num = atoi(argv[3]);
	}
	while(status != -1){
		printf("mysh[%d]> ", c);
		buffer = read_input();
		if(buffer[0] != '\n'){
			history[position] = add_history(strdup(buffer));
			args = get_tokens(buffer);
			if(verbose == 1){
				verbose_print(0, args);
			}
			if(strcmp(args[0], "help") == 0){
				status = help(0, args);
				c++;
				position++;
			}
			else if(strcmp(args[0], "history") == 0){
				if(c < his_num){
					status = displayHistory(0,history);
				}else{
					status = displayHistory(c-his_num, history);
				}
				c++;
				position++;
			}
			else if(strcmp(args[0], "quit") == 0){
				status = -1;
				exit(0);
			}
			else if(strcmp(args[0], "verbose") == 0){
				if(strcmp(args[1], "on") == 0){
					verbose = 1;
				}else if(strcmp(args[1], "off") == 0){
					verbose = 0;
				}else if(args[1] == NULL){
					perror("usage: verbose on | off");
				}else{
					perror("usage: verbose on | off");
				}
				c++;
				position++;
			}
			else if(args[0][0] == '!'){
				char* command = malloc(sizeof(char) * 64);
				char* bang = malloc(sizeof(char) * 64);
				bang = strdup(args[0]);
				bangNum = strtok(bang, "!");
				strcpy(command, history[atoi(bangNum) - 1]);
				args = get_tokens(command);
				if(strcmp(args[0], "help") == 0){
	                        	status = help(0, args);
		                }
		                else if(strcmp(args[0], "history") == 0){
		                        if(c <= his_num){
		                                status = displayHistory(0, history);
		                        }else{
		                                status = displayHistory(c-his_num, history);
		                        }
		                }
		                else if(strcmp(args[0], "quit") == 0){
		                        status = -1;
		                        exit(0);
		                }
		                else if(strcmp(args[0], "verbose") == 0){
		                        if(strcmp(args[1], "on") == 0){
		                                verbose = 1;
		                        }else if(strcmp(args[1], "off") == 0){
		                                verbose = 0;
		                        }else if(args[1] == NULL){
		                                perror("usage: verbose on | off");
		                        }else{
		                                perror("usage: verbose on | off");
		                        }
		                }
				else{
					if(verbose == 0){
	                                	status = execution(c, args);
	                     		}else if(verbose == 1){
	                                	status = executionVerbose(c, args);
	                        	}
				}
				c++;
				position++;
			}
			else{
				if(verbose == 0){
					status = execution(c, args);
				}else if(verbose == 1){
					status = executionVerbose(c, args);
				}
				c++;
				position++;
			}
		}
	}
}


int main(int argc, char *argv[]){
        loop(argc, argv);	
}
