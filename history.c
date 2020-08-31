/**
 @file
 * History functions and global variables for history
 */

#include <stddef.h>

#include "history.h"

/**
 * Initializes init and sets up history data structures
 *
 * Parameter(s):
 * - limit: int maximum number of entries maintained
 * 
 * Returns: char pointer to the next token in the string.
 */
void hist_init(unsigned int limit)
{
    // TODO: set up history data structures, with 'limit' being the maximum
    // number of entries maintained.
}

/**
 * Destroy history
 *
 * Parameter(s):
 * - none
 * 
 * Returns: void
 */
void hist_destroy(void)
{

}

/**
 * Add to history list
 *
 * Parameter(s):
 * - none
 *
 * Returns: void
 */
void hist_add(const char *cmd)
{

}

/**
 * Print history
 *
 * Parameter(s):
 * - none
 *
 * Returns: void
 */
void hist_print(void)
{

}

/**
 * Retrieves the most recent command starting with prefix or NULL if no match found
 *
 * Parameter(s):
 * - prefix: char pointer to prefix of command
 *
 * Returns: command starting with prefix or NULL if no match found
 */
const char *hist_search_prefix(char *prefix)
{
    // TODO: Retrieves the most recent command starting with 'prefix', or NULL
    // if no match found.
    return NULL;
}

/**
 * Retrieves a particular command number and return null if no match found
 *
 * Parameter(s):
 * - command_number: command number to search for
 * 
 * Returns: char pointer to commmand a a command number
 */
const char *hist_search_cnum(int command_number)
{
    // TODO: Retrieves a particular command number. Return NULL if no match
    // found.
    return NULL;
}

/**
 * Retrieves the most recent command number
 *
 * Parameter(s):
 * - none
 *
 * Returns: int of the most recent command number
 */
unsigned int hist_last_cnum(void)
{
    // TODO: Retrieve the most recent command number
    return 0;
}
