/**
 @file
 * Simple shell implementation that runs in both current directory and in the PATH environment
 * 
 * Compontents:
 *  - Prompt - Displays shell prompt with helpful information
 *  - Scripting - Supports scripting mode to run test cases
 *  - Built-In Commands - Supports built-in commands including (cd, #, history, !, jobs, exit)
 *  - Signal Handling - Able to grancefully handle user pressing Ctrl+C
 *  - Redirection - Supports file output redirection and pipe redirection
 *  - Background Jobs - Run command in the background if command ends in &
 *  - Tab Completion - Autocomplete command of user if user presses tab key
 *
 */

#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "history.h"
#include "leetify.h"
#include "logger.h"
#include "ui.h"

#define USERNAME_SIZE 32
#define HOSTNAME_SIZE 64
#define PATH_MAX 4096
#define PROMPT_MAX 8000
#define LIST_SIZE 100
#define BUF 100

int command_number;
char username[USERNAME_SIZE];
char hostname[HOSTNAME_SIZE];
char cwd[PATH_MAX];
char prompt[PROMPT_MAX];
pid_t pid_list[10];
int list_index;
char *line_list[10];
bool exec;
int i_tok;
char *command;

/**
 * Gets Prompt
 *
 * Parameter(s):
 * - none
 * 
 * Returns: char string of prompt
 */
char *get_prompt() {
	getlogin_r(username, USERNAME_SIZE);
	gethostname(hostname, HOSTNAME_SIZE);
	getcwd(cwd, PROMPT_MAX);
	sprintf(prompt, "[][%d]%s@%s:~%s]", command_number, username, hostname, cwd);
	return prompt;
}

/**
 * Handles sigint and makes sure to program does not stop from Ctrl+c
 *
 * Parameter(s)::
 * - signo: int representing signal number of sigint
 *
 * Returns: void
 */
void sigint_handler(int signo) {
	if(!exec) {
		read_command();
	}
	fflush(stdout);
}

/**
 * Signal handler for when the child quits
 *
 * Parameter(s):
 * - signo: int representing child quit
 *
 * Returns: void
 */
void sigchld_handler(int signo) {
	int status;
	pid_t child = waitpid(-1, &status, WNOHANG);

	for(int i = 0; i < list_index; i++) {
		if(pid_list[i] == child) {
			free(line_list[i]);
			for(int j = i; j < list_index; j++) {
				pid_list[j] = pid_list[j + 1];
				line_list[j] = line_list[j + 1];
			}
			list_index--;
			break;
		}
	}
	fflush(stdout);
}

/**
 * Handles cd command and changes directory
 *
 * Parameter(s):
 * - tokens[]: string array for chdir relocation
 *
 * Returns: void
 */
void cd_command(char *tokens[]) {
	char *dir = getenv("HOME");
	if(tokens[1] == NULL) {
		chdir(dir);
	} else {
		chdir(tokens[1]);
	}
}

/**
 * Removes comment from command
 *
 * Parameter(s):
 * - command: char to change
 * 
 * Returns: command without comment
 */
char *remove_comment(char *command) {
	char *ptr;
	ptr = strchr(command, '#');
	if(ptr != NULL) {
		*ptr = '\0';
	}
	return command;
}

/**
 * Main function. Initializes init ui and takes in command
 * 
 * Parameter(s):
 * - none
 *
 * Returns: void
 */
int main(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGCHLD, sigchld_handler);

    init_ui();

	exec = true;
	char command_temp[BUF] = {0};
    while (true) {
		exec = false;
        command = read_command();
		close(STDIN_FILENO);
        if (command == NULL) {
            break;
        }
		
		command = remove_comment(command);
		
		struct command_line cmds[1] = {0};

		strcpy(command_temp, command);
		command_temp[strcspn(command_temp, "\n")] = 0;
		char *p = strtok(command_temp, " ");
		char *command_token[BUF];
		
		i_tok = 0;
		while(p) {
			command_token[i_tok++] = p;
			p = strtok(NULL, " ");
		}
		command_token[i_tok] = NULL;
		

		if(strcmp(command_token[0], "exit") == 0) {
			exit(0);
		}
		if(strcmp(command_token[0], "cd") == 0) {
			cd_command(command_token);
			continue;
		}

		cmds[0].tokens = command_token;
		cmds[0].stdout_pipe = false;
		cmds[0].stdout_file = NULL;

		pid_t pid = fork();
		if(pid < 0) {
			wait(0);
			perror("fork");
			exit(1);
		} else if(pid == 0) {
			execute_pipeline(cmds);
			close(STDIN_FILENO);
		} else {
			wait(0);
		}


        LOG("Input command: %s\n", command);
    }

    return 0;
}
