# Project 2: Command Line Shell

## General Info
* Simple shell implementation that runs in both current directory and in the PATH environment

## Components
* Prompt - Displays shell prompt with helpful information
* Scripting - Supports scripting mode to run test cases
* Built-In Commands - Supports built-in commands including (cd, #, history, !, jobs, exit)
* Signal Handling - Able to grancefully handle user pressing Ctrl+C
* Redirection - Supports file output redirection and pipe redirection
* Background Jobs - Run command in the background if command ends in &
* Tab Completion - Autocomplete command of user if user presses tab key

## Run Code
```
make
./crash
```

## Testing

To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

```
# Run all test cases:
make test

# Run a specific test case:
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'
```
See: https://www.cs.usfca.edu/~mmalensek/cs326/assignments/project-2.html
