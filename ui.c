/**
 @file
 * UI initialization and functions
 */
#include <stdio.h>
#include <readline/readline.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "history.h"
#include "logger.h"
#include "ui.h"

#define BUF 4096

bool exit_status;

int list_index;
char username[LOGIN_NAME_MAX];
char hostname[HOST_NAME_MAX];
char temp_cwd[PATH_MAX];
char cwd[PATH_MAX];
char home_dir[PATH_MAX] = "/home/";
char temp_dir[PATH_MAX] = "~";

/* -- Private function forward declarations -- */
static int readline_init(void);
static int key_up(int count, int key);
static int key_down(int count, int key);
static char **command_completion(const char *text, int start, int end);
static char *command_generator(const char *text, int state);

static char *line = NULL;
static size_t line_sz = 0;

static char prompt_str1[BUF] = "";
static char prompt_str2[BUF] = "-> ";

static char exit_status_success[] = {0xF0,0x9F,0x98,0x81,'\0'};
static char exit_status_fail[] = {0xF0,0x9F,0xA4,0xAE,'\0'};

/**
 * Initialize UI and sets up global variable
 *
 * Parameter(s):
 * - none
 *
 * Returns: void.
 */
void init_ui(void)
{
    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");

	getlogin_r(username, LOGIN_NAME_MAX);
	gethostname(hostname, HOST_NAME_MAX);
	getcwd(cwd, PATH_MAX);

    rl_startup_hook = readline_init;
}

/**
 * Creates prompt line1
 *
 * Parameter(s):
 * - none
 *
 * Returns: char pointer to prompt line 1
 */
char *prompt_line1(void){
	strcat(prompt_str1, "]-[");
	char index[sizeof(int) * 4 + 1];
	sprintf(index,"%d", list_index);
	strcat(prompt_str1, index);
	strcat(prompt_str1, "]-[");
	strcat(prompt_str1, username);
	strcat(prompt_str1, "@");
	strcat(prompt_str1, hostname);
	strcat(prompt_str1, ":");
	if(strstr(cwd, home_dir) != NULL) {
		strcat(prompt_str1, "~");
		strcat(home_dir, username);
		char *match;
		int len = strlen(home_dir);
		while((match = strstr(cwd, home_dir))) {
			*match = '\0';
			strcat(cwd, match+len);
		}
	}
	strcat(prompt_str1, cwd); 	
	
    return prompt_str1;
}

/**
 * Creates prompt line 2
 *
 * Parameter(s):
 * - none
 *
 * Returns: char pointer to prompt line 2
 */
char *prompt_line2(void) {
    return prompt_str2;
}

/**
 * Read command and check if command is script or stdin
 *
 * Parameter(s):
 * - none
 *
 * Returns: char pointer of prompt and asks for input
 */
char *read_command(void)
{
	if(!isatty(STDIN_FILENO)) {
		ssize_t nread = getline(&line, &line_sz, stdin);
		if(nread == -1) {
			LOGP("Reached end of input stream. \n");
			return NULL;
		}
		size_t newline = strcspn(line, "\n");
		line[newline] = '\0';
		return line;
	} else {
		strcpy(prompt_str1, "");
		strcpy(prompt_str1, "╭─[");
		if(!exit_status) {
			strcat(prompt_str1, exit_status_success);
		} else {
			strcat(prompt_str1, exit_status_fail);
		}
    	puts(prompt_line1());
    	return readline(prompt_line2());
	}
	close(STDIN_FILENO);	
}

/**
 * Initialize Readline for key recognition
 *
 * Parameter(s):
 * - none
 *
 * Returns: int of key of keyboard
 */
int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    return 0;
}

/**
 * Accepts up key arrow and will reverse history search with up key
 *
 * Parameter(s):
 * - count: int counter for prefix start
 * - key: int
 *
 * Returns: location in history
 */
int key_up(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'up' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: reverse history search

    return 0;
}

/**
 * Accepts down key arrow and will foward history search with down key
 *
 * Parameter(s):
 * - count: int counter for prefix start
 * - key: int
 *
 * Returns: char pointer to the next token in the string.
 */
int key_down(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'down' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: forward history search

    return 0;
}

/**
 * Tell readline that if there are no suitable completion, it should fall back on its built-in filename completion
 *
 * Parameter(s):
 *  - text: char
 *  - start: int
 *  - end: int
 *
 * Returns: char
 */
char **command_completion(const char *text, int start, int end)
{
    /* Tell readline that if we don't find a suitable completion, it should fall
     * back on its built-in filename completion. */
    rl_attempted_completion_over = 0;

    return rl_completion_matches(text, command_generator);
}

/**
 * This function is called repeatedly by the readline library to build a list of
 * possible completions. It returns one match per function call. Once there are
 * no more completions available, it returns NULL.
 */
char *command_generator(const char *text, int state)
{
    // TODO: find potential matching completions for 'text.' If you need to
    // initialize any data structures, state will be set to '0' the first time
    // this function is called. You will likely need to maintain static/global
    // variables to track where you are in the search so that you don't start
    // over from the beginning.

    return NULL;
}
