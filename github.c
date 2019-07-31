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
        (void)argc;
	pid_t pid;
        int status;
	//creating pid using fork
        pid = fork();
        if(pid == 0){
		//child process created
                if(execvp(argv[0], argv) == -1){
			//if -1 the function doesn't exist
			perror(argv[0]);
		}	
		exit(EXIT_FAILURE);
        }
	//is fork fails
	else if(pid < 0){
		perror("fork failed");
	}
	//the parent waits for the child to be done
	else{
                wait(&status);
		int s = WEXITSTATUS(status);
		if(s != 0){
			fprintf(stderr, "command status: %d\n", s);
		}
        }
        return 1; 
}

///This function is similar to the previous one, but is used for printing during
//verbose being on. 
int executionVerbose(int argc, char *argv[]){
        (void)argc;
	pid_t pid;
        int status;
	//creating pid using fork
        pid = fork();
        if(pid == 0){
		//child process created
                printf("\twait for pid %d: %s\n", getpid(), argv[0]);
                printf("\texecvp: %s\n", argv[0]);
		if(execvp(argv[0], argv) == -1){
			//if -1 the function doesn't work
                	perror(argv[0]);
                }
                exit(EXIT_FAILURE);
        }
	//if fork fails
        else if(pid < 0){
                perror("fork failed");
        }
	//the parent waits for the child to be done
        else{
        	wait(&status);
		int s = WEXITSTATUS(status);
		if(s != 0){
			printf("command status: %d\n", s);
		}
        }
        return 1;
}

///this function is used for verbose printing statements. 
int verbose_print(int argc, char *argv[]){
	(void)argc;
	int len = 0;
	printf("\tcommand: ");
	//printing the command itself
	while(argv[len] != NULL){
		printf("%s ",argv[len]);
		len++;
	}
	printf("\n\n");
	printf("\tinput command tokens:\n");
	//printing input commands one by one
	for(int i=0; i<len; i++){
		printf("\t%d: %s\n", i, argv[i]);
	}
	return 1;
}

///The help function to help the user.
int help(int argc, char *argv[]){
	(void)argc;
	(void)argv;
	printf("Welcome to Mysh!\n");
	printf("Type your command name along with arguments, and hit enter.\n");
	printf("The Internal Command include:\n");
	printf("1. Bang Command: !\n");
	printf("2. Help Command: help\n");
	printf("3. History Command: history\n");
	printf("4. Quit Command: quit\n");
	printf("5. Verbose on/off: verbose on | off\n");
	printf("Enjoy your shell\n");
	return 1;
}

///This function is used to print the history added by the user.
int displayHistory(int argc, char *argv[]){
	while(argv[argc] != NULL){
		printf("\t%d: %s\n", argc+1, argv[argc]);
		argc++;
	}
	return 1;
}

///This function is used to remove the \n for the input by the user and add
//it to the history.
char* add_history(char* line){
	return strtok(line, "\n");
}

///This function is used to read the input from the user.
char* read_input(void){
	char *command;
	size_t bufsize = 0;
	getline(&command, &bufsize, stdin);
	return command;
}

///This function is used to tokenize the strig obtained by the user.
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

///This function can be considered to be the main function. It will recurse until
//the user enters the quit command. Moreover, it uns all the external and internal
//commands.Moreover it taes the command line input from the user and processes it,
//as it is expected to be.
void loop(int argc, char *argv[]){
	char *buffer;
	int status, verbose, his_num;
	char **args;
	char *bangNum = malloc(sizeof(char) * LSH_TOK_BUFSIZE);
	char **history = malloc(sizeof(char*) * LSH_TOK_BUFSIZE);
	int position = 0;
	int c = 1;
	//checking for the command line input from the user
	if(argc == 1){
		//verbose is off and history number is by default 10
		verbose = 0;
		his_num = 10;
	}
	else if(argc == 2){
		//verbose is on and history number is by default 10
		verbose = 1;
		his_num = 10;
	}
	else if(argc == 3){
		//verbose is off and history number is set by the user
		verbose = 0;
		his_num = atoi(argv[2]);
	}
	else if(argc == 4){
		//verbose is on and the history number is added by the user
		verbose = 1;
		his_num = atoi(argv[3]);
	}
	//the loop for the shell begins
	while(status != -1){
		printf("mysh[%d]> ", c);
		//taking the input from the user
		buffer = read_input();
		//checking if the user entered a blank input
		if(buffer[0] != '\n'){
			//adding the string to the history
			history[position] = add_history(strdup(buffer));
			//tokenizing the input from the user
			args = get_tokens(buffer);
			//checking if verbose mode is on
			if(verbose == 1){
				verbose_print(0, args);
			}
			//if the internal command is help
			if(strcmp(args[0], "help") == 0){
				status = help(0, args);
				c++;
				position++;
			}
			//if the internal command is history
			else if(strcmp(args[0], "history") == 0){
				if(c < his_num){
					status = displayHistory(0,history);
				}else{
					status = displayHistory(c-his_num, history);
				}
				c++;
				position++;
			}
			//if the internal command is quit
			else if(strcmp(args[0], "quit") == 0){
				status = -1;
				exit(0);
			}
			//if the internal command is verbose on/off
			else if(strcmp(args[0], "verbose") == 0){
				//checking for on
				if(strcmp(args[1], "on") == 0){
					verbose = 1;
				}
				//checking for off
				else if(strcmp(args[1], "off") == 0){
					verbose = 0;
				}else if(args[1] == NULL){
					perror("usage: verbose on | off");
				}
				//if some other input is added
				else{
					perror("usage: verbose on | off");
				}
				c++;
				position++;
			}
			//if the internal command is bang command
			else if(args[0][0] == '!'){
				char* command = malloc(sizeof(char) * 64);
				char* bang = malloc(sizeof(char) * 64);
				bang = strdup(args[0]);
				//getting the number from the bang command
				bangNum = strtok(bang, "!");
				//getting the command from the history list
				strcpy(command, history[atoi(bangNum) - 1]);
				//updating the position in history
				history[position] = add_history(strdup(command));
				args = get_tokens(command);
				//checking for the commands
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
			//if the command is not an internal command
			else{
				//checking is the verbose is on or off
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

//calling the main function
int main(int argc, char *argv[]){
        loop(argc, argv);	
}
