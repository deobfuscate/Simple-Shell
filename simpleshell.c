#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <readline/readline.h>
#include <sys/types.h>
#include "tokenizer.h"

// cleanup function
void free_token(token *head) {
	token *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

// process a token list and pass it to execvp and output results
void execute_commands(token *list) {
	// convert token list to char ptr array for execvp
	char *argv[101];
	int counter = 0;  // used for current token legnth
	// iterate thru token list, adding to char array for execvp
	while (list != NULL) {
		if (counter == 100) { // ensure we don't add too many items for array
			printf("Too many arguments! Only 100 supported\n");
			return;
		}
		// IO redirection
		if (strcmp(list->data, "<") == 0 || strcmp(list->data, ">") == 0 ||
			strcmp(list->data, "&") == 0) {
			printf("IO redirection and background not implemented\n");
			break;
		}
		argv[counter] = list->data;
		list = list->next; // ++
		counter++;
	}
	argv[counter] = NULL; // null terminate
	// change directory functionality
	if (strcmp(argv[0], "cd") == 0) {
		if (counter != 2) // make sure cd only has 1 argument...
			printf("Accepts exactly one argument\n");
		else if (chdir(argv[1]) != 0) // ...otherwise, change dir...
			// ...or give an error
			printf("Directory does not exist or is not accessible.\n");
	}
	// print working directory functionality
	else if (strcmp(argv[0], "pwd") == 0) {
		char buf[256]; // buffer for directory string
		// if we can't get current dir, print error
		if (getcwd(buf, sizeof(buf)) == NULL)
			printf("Unable to obtain current directory\n");
		else // found cwd, print to user
			printf("%s\n", buf);
	}
	else {
		int return_code; // used to check return code of forked proc
		pid_t pid; // fork() returns pid_t from types.h
		// if negative, creation of child process was unsuccessful
		if ((pid = fork()) < 0) {
			printf("Unable to spawn program\n");
			exit(1);
		}
		else if (pid == 0) { // returned to child process
			// execute our process, if result is neg, it failed
			if (execvp(*argv, argv) < 0) {
				printf("Unable to execute %s\n", argv[0]);
				return;
			}
		}
		else { // process creation was successful
			// wait for proc to finish exec, store return code in a variable
			while (wait(&return_code) != pid) {
				continue;
			}
			// use return_code to output appropriate status message
			if (return_code == 0) printf("Process exited successfully\n");
			else printf("Process exited with error\n");
		}
	}
}

int main() {
	token *head = NULL;
	char quote = BACKSLASH;
	char *input;
	// while we are receiving input proceed, otherwise exit gracefully
	while ((input = readline("> ")) != (void*)0) {
		// pass the token list to tokenize_input function
		head = tokenize_input(head, input, quote);

		token *list = head; // reference pointer for execution
		token *tmp = NULL; // placeholder for current token list
		// loop through tokens, split by ; or |, execute token lists, repeat
		while (list != NULL) {
			// if we hit a delimiter, execute all previous tokens, carry on
			if (strcmp(list->data, ";") == 0 || strcmp(list->data, "|") == 0) {
				if (strcmp(list->data, "|") == 0)
					printf("Pipe not implemented\n");
				// check we actually have something to exec, pass to exec
				if (tmp != NULL) execute_commands(tmp);
				free_token(tmp); // clean up old token
				tmp = NULL; // get ready for new token
			}
			// this isn't a delimiter, so add to current tmp list
			else
				tmp = insert(list->data, tmp);
			list = list->next;
		}
		// execute commands if any were not finished
		if (tmp != NULL) execute_commands(tmp);

		// cleanup
		free_token(head);
		head = NULL;
		free_token(list);
		list = NULL;
		free_token(tmp);
		tmp = NULL;
		free(input);
	}
}