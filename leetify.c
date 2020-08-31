/**
 @file
 * Taken from leetify.c in lab 6. Pipes command and executes
 */
#include "leetify.h"
#include "logger.h"

/**
 * Pipes command and runs the command
 *
 * Parameter(s):
 * - cmds: struct command_line that deternmines if the last command in the pipeline and whether the final result gets written to a file or terminal
 *
 * Returns: void
 */
void execute_pipeline(struct command_line *cmds)
{
    /**
     * TODO: design an algorithm that sets up a pipeline piece by piece.
     * Solutions will probably either iterate over the pieces of the pipeline or
     * work recursively. Imagine you have three commands in a pipeline:
     *
     *  - cat
     *  - tr
     *  - sed
     *
     * Use 'stdout_pipe' to determine when you've reached the last command in
     * the pipeline, and 'stdout_file' to decide whether the final result gets
     * written to a file or the terminal.
     *
     * If we aren't at the last command, then we need to set up a pipe for the
     * current command's output to go into. For example, let's say our command
     * is `cat file.txt`. We will create a pipe and have the stdout of the
     * command directed to the pipe. Before running the next command, we'll set
     * up the stdin of the next process to come from the pipe, and
     * execute_pipeline will run whatever command comes next (for instance,
     * `tr '[:upper:]' '[:lower:]'`).
     *
     * Here's some pseudocode to help:
     *
     * create a new pipe
     * fork a new process
     * if pid is the child:
     *     dup2 stdout to pipe[1]
     *     close pipe[0]
     *     execvp the command
     * if pid is the parent:
     *     dup2 stdin to pipe[0]
     *     close pipe[1]
     *     move on to the next command in the pipeline
     *
     * The special case here is when there are no more commands left. In that
     * case, you can simply execvp the command (no need to create another pipe).
     * If you created a handler process in main(), then it will be replaced by
     * this last call to execvp.
     */
	if(!cmds->stdout_pipe) {
		if(cmds->stdout_file!=NULL) {
			int open_flags = O_RDWR | O_CREAT | O_TRUNC;

			int open_perms = 0666;

			int fd = open(cmds->stdout_file, open_flags, open_perms);
			if(fd == -1) {
				perror("open");
				return;
			}

			if(dup2(fd, fileno(stdout)) == -1) {
				perror("dup2");
				return;
			}
		}
		
		if(execvp(cmds->tokens[0], cmds->tokens) == -1) {
			perror("execvp");
			close(STDIN_FILENO);
		}
		
		return;
	} else {
		int fd[2];
		if(pipe(fd) == -1) {
			perror("pipe");
		}
		
		pid_t pid = fork();
		if(pid == 0) {
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			if(execvp(cmds->tokens[0], cmds->tokens) == -1) {
				perror("execvp");
				close(STDIN_FILENO);
			}
		} else {
			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
			execute_pipeline(cmds + 1);
		}
	}
}

