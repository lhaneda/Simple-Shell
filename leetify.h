/**
 @file
 * Holds command line struct and function for execute pipeline
 */
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @struct command_line
 * @brief command line set up and tokens
 * @var command_line::tokens
 * @var command_line::stdout_pipe
 * @var command_line::stdout_file
 */
struct command_line {
	char **tokens;
	bool stdout_pipe;
	char *stdout_file;
};

void execute_pipeline(struct command_line *cmds);
