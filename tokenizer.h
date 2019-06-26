typedef struct token {
	char *data;
	struct token *next;
} token;

// constants for readability
#define BACKSLASH '\\'
#define TERMINATE '\0'
#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '"'

// inserts a token into a linked list
token *insert(char *tok, token *head);

// prints a list of all tokens
// eg. [{a},{b},{c}]
void print_tokens(token *head);

// removes all occurences of a char from a string
char *remove_chars(char *p, char remove);

// formats a char pointer for insert()
token *make_token(int len, char *tok, token *head, char quote);

/*
 *  parses a char ptr and returns as a token
 *  splits tokens on "|;<>&", removes whitespace,
 *  handles escape chars and quotes
 */
token *tokenize_input(token *head, char *input, char quote);