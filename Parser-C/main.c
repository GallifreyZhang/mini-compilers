#include "parser.h"

char* token_array[] = {
    "TOKEN_UNKNOWN",
    "TOKEN_NUM",
    "TOKEN_STRING",
    "TOKEN_CHAR",
    "TOKEN_IDENTIFIER",
    "TOKEN_IF",
    "TOKEN_THEN",
    "TOKEN_ELSE",
    "TOKEN_WHILE",
    "TOKEN_DO",
    "TOKEN_INT",
    "TOKEN_FLOAT",
    "TOKEN_SEMICOLON",
    "TOKEN_COMMA",
    "TOKEN_LEFT_BRACKET",      
    "TOKEN_RIGHT_BRACKET",     
    "TOKEN_LEFT_BRACE",        
    "TOKEN_RIGHT_BRACE",       
    "TOKEN_DOT",               
    "TOKEN_LEFT_PAREN",
    "TOKEN_RIGHT_PAREN",
    "TOKEN_ADD",
    "TOKEN_SUB",
    "TOKEN_MUL",
    "TOKEN_DIV",
    "TOKEN_ASSIGN",
    "TOKEN_GREATE",
    "TOKEN_LESS",
    "TOKEN_EQUAL",
    "TOKEN_GREATE_EQUAL",
    "TOKEN_LESS_EQUAL",
    "TOKEN_NOT_EQUAL",
    "TOKEN_EOF"
};

bool match_token(Parser* parser, TokenType expected) {
    if (parser->cur_token.type == expected) {
        get_next_token(parser);
        return true;
    }
    return false;
}

void init_parser(Parser* parser, const char* file_name, const char* source_code) {
    parser->file_name = file_name;
    parser->source_code = source_code;
    parser->cur_char = *source_code;
    parser->next_char_ptr = source_code + 1;

    parser->cur_token.line_no = 1;
    parser->cur_token.start = NULL;
    parser->cur_token.length = 0;
    parser->cur_token.type = TOKEN_UNKNOWN;

    parser->pre_token = parser->cur_token;
}

static void run_file(const char* path) {
     const char* source_code = read_file(path);
     Parser parser;
     init_parser(&parser, path, source_code);

    //  #include "token.list"
     while (parser.cur_token.type != TOKEN_EOF) {
         get_next_token(&parser);
         char* tmp = token_array[parser.cur_token.type];
         printf("%dL: %s ", parser.cur_token.line_no, tmp);
         uint idx = 0;
         while (idx < parser.cur_token.length) {
             printf("%c", *(parser.cur_token.start+idx++));
         }
         printf("\n");
     }
}

/**
 * command: gcc -g -Wall main.c parser.c utils.c -o parser
 */
int main(int argc, const char** args) {
    if (argc > 1) {
        run_file(args[1]);
    } else {
        const char* file_name = "source.txt";
        run_file(file_name);
    }
    return 0;
}