#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <unistd.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int execution(int argc, char *argv[]){
        pid_t pid, wpid;
        int status;
        pid = fork();
        if(pid == 0){
                if(execvp(argv[0], argv) == -1){
			perror(argv[0]);
			fprintf(stderr, "command status: %d\n", argc);
		}	
		exit(EXIT_FAILURE);
        }
	else if(pid < 0){
		perror("fork failed");
	}
	else{
                do{
                        wpid = waitpid(pid, &status, WUNTRACED);
                }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1; 
}


int executionVerbose(int argc, char *argv[]){
        pid_t pid, wpid;
        int status;
        pid = fork();
        if(pid == 0){
                if(execvp(argv[0], argv) == -1){
                        perror(argv[0]);
                        fprintf(stderr, "command status: %d\n", argc);
                }
                exit(EXIT_FAILURE);
        }
        else if(pid < 0){
                perror("fork failed");
        }
        else{
		printf("\twait for pid %d: %s\n", pid, argv[0]);
                printf("\texecvp: %s\n", argv[0]);
                do{
                        wpid = waitpid(pid, &status, WUNTRACED);
                }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1;
}


void verbose_print(char *args[]){
	int len = 0;
	printf("\tcommand: ");
	while(args[len] != NULL){
		printf("%s ",args[len]);
		len++;
	}
	printf("\n\n");
	printf("\tinput command tokens:\n");
	for(int i=0; i<len; i++){
		printf("\t%d: %s\n", i, args[i]);
	}
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
	char *buffer, *copy;
	int status, verbose, his_num;
	char **args;
	char *bangnum = malloc(sizeof(char) * LSH_TOK_BUFSIZE);
	char **history = malloc(sizeof(char*) * LSH_TOK_BUFSIZE);
	int position = 0;
	int c = 1;
	if(argc == 1){
		verbose = 0;
	}
	else if(argc == 2){
		verbose = 1;
	}
	else if(argc == 3){
		his_num = atoi(argv[2]);
	}
	else if(argc == 4){
		verbose = 1;
		his_num = atoi(argv[3]);
	}
	while(status != -1){
		printf("mysh[%d]> ", c);
		buffer = read_input();
		history[position] = add_history(strdup(buffer));
		args = get_tokens(buffer);
		if(verbose == 1){
			verbose_print(args);
		}
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
		else if(args[0][0] == '!'){
			
			if(strcmp(history[args[0][1]-1] , "help") == 0){
				printf("%s\n", history[args[0][1]]);
//			status = bang((args[0][1] - '0'), history);
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
}


int main(int argc, char *argv[]){
        
		loop(argc, argv);
		/*
	}else if(argc == 2){
		loop(a);
	}else if(argc == 3){
		loop(1, 0, atoi(argv[2]));
	}else if(argc == 4){
		//make complete with verbose and history
	}
        return EXIT_SUCCESS;
	*/
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
//      if(status == 0){
  //            fprintf(stderr, "%s: No such file or directory\n", args[0]);
    //            fprintf(stderr, "command status: %d\n", argc);
      //        }
        return status;
}
*/




