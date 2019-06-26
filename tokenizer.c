#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"

// inserts a token into a linked list
token *insert(char *tok, token *head) {
	// if linked list is empty, make a new one
	if (head == NULL) {
		token *new_token = (token*) malloc(sizeof(token));
		new_token->data = tok;
		new_token->next = NULL;
		return new_token;
	}
	// otherwise, add to list
	struct token *p = head;
	while (p != NULL) {
		if (p->next == NULL) {
			token *new_token = (token*) malloc(sizeof(token));
			new_token->data = tok;
			new_token->next = NULL;
			p->next = new_token;
			break;
		}
		p = p->next;
	}
	return head;
}

// prints a list of all tokens
// eg. [{a},{b},{c}]
void print_tokens(token *head) {
	token *p = head;
	printf("[");
	while (p != NULL) {
		if (p->next == NULL)
			printf("{%s}", p->data);
		else
			printf("{%s},", p->data);
		p = p->next;
	}
	printf("]\n");
}

// removes all occurences of a char from a string
char *remove_chars(char *p, char remove) {
	int len = strlen(p);
	int count = 0;
	char *new_str = malloc(len);
	for (int j = 0; j < len; j++) {
		// handle escape character in double quoted strings
		if (remove == DOUBLE_QUOTE) {
			if (*p == DOUBLE_QUOTE && *(p - 1) != BACKSLASH) {
				p++;
				count++;
				continue;
			}
		}
		else {
			if (*p == remove) {
				p++;
				count++;
				continue;
			}
		}
		*new_str = *p;
		new_str++;
		p++;
	}
	*new_str = TERMINATE;
	return new_str - len + count;
}

// formats a char pointer for insert()
token *make_token(int len, char *tok, token *head, char quote) {
	char *p = malloc(len);
	strncpy(p, tok - len, len);
	char *null = p + len;
	*null = TERMINATE;

	if (quote == BACKSLASH) {
		p = remove_chars(p, BACKSLASH);
	}
	if (quote == SINGLE_QUOTE) {
		p = remove_chars(p, SINGLE_QUOTE);
	}
	if (quote == DOUBLE_QUOTE) {
		p = remove_chars(p, DOUBLE_QUOTE);
		p = remove_chars(p, BACKSLASH);
	}

	quote = BACKSLASH;
	return insert(p, head);
}

/*
 *  parses a char ptr and returns as a token
 *  splits tokens on "|;<>&", removes whitespace,
 *  handles escape chars and quotes
 */
token *tokenize_input(token *head, char *input, char quote) {
	char *p = input;
	int len = 0;
	// increment pointer along our input string until terminate char
	while (*p != TERMINATE) {
		// special delimiters, turn them into 1 char tokens
		if (strchr("|;<>&", *p)) {
			if (len > 0) {
				head = make_token(len, p, head, quote);
				len = 0;
			}
			head = make_token(1, p + 1, head, quote);
			p++;
		}
		// back slash, skip 2 chars, the backslash and whatever char follows
		else if (strchr("\\", *p)) {
			p += 2;
			len += 2;
		}
		// double quote, handles escape chars
		else if (strchr("\"", *p)) {
			p++;
			len++;
			while (*p != TERMINATE && *p != DOUBLE_QUOTE ||
				(*(p - 1) == BACKSLASH && *p == DOUBLE_QUOTE)) {
				p++;
				len++;
			}
			quote = DOUBLE_QUOTE;
			len++;
			p++;
		}
		// single quote, does not handle escape chars
		else if (strchr("\'", *p)) {
			p++;
			len++;
			while (*p != TERMINATE && *p != SINGLE_QUOTE) {
				p++;
				len++;
			}
			quote = SINGLE_QUOTE;
			len++;
			p++;
		}
		// space
		else if (isspace(*p)) {
			if (len > 0) {
				head = make_token(len, p, head, quote);
				len = 0;
			}
			while (isspace(*p)) {
				p++;
			}
		}
		else {
			len++;
			p++;
		}
	}
	// if there's still a token waiting to be made, make it
	if (len > 0) head = make_token(len, p, head, quote);
	return head;
}