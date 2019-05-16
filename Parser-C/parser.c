#include <ctype.h>
#include "parser.h"

typedef struct {
    char* keyword;
    uint length;
    TokenType token;
} KeywordToken;

KeywordToken keywords_token[] = {
    {"if", 2, TOKEN_IF},
    {"then", 4, TOKEN_THEN},
    {"else", 4, TOKEN_ELSE},
    {"while", 5, TOKEN_WHILE},
    {"do", 2, TOKEN_DO},
    {"int", 3, TOKEN_INT},
    {"float", 5, TOKEN_FLOAT},
    {NULL, 0, TOKEN_UNKNOWN}
};

static TokenType is_keyword(const char* start, uint length) {
    uint idx = 0;
    while (keywords_token[idx].keyword != NULL) {
        if (keywords_token[idx].length == length && 
            memcmp(keywords_token[idx].keyword, start, length) == 0) {
                return keywords_token[idx].token;
        }
        idx++;
    }
    return TOKEN_IDENTIFIER;
}

bool is_letter(const char c) {
    if ((c>='a'&&c<='z') || (c>='A'&&c<='Z')) {
        return true;
    }
    return false;
}

bool is_num(const char c) {
    if (c>='0' && c<='9') return true;
    return false;
}

char look_ahead_char(Parser* parser) {
    return *(parser->next_char_ptr);
}

static void get_next_char(Parser* parser) {
    parser->cur_char = *parser->next_char_ptr++;
}

static bool match_next_char(Parser* parser, char expected_char) {
    if (look_ahead_char(parser) == expected_char) {
        get_next_char(parser);
        return true;
    }
    return false;
}

/*bool is_space(const char c) {
    if (c=='\n' || c=='\t' || c=='\x20' || c=='\r') {
        return true;
    }
    return false;
}*/

static void skip_blanks(Parser* parser) {
    while(isspace(parser->cur_char)) {
        if (parser->cur_char == '\n') {
            parser->cur_token.line_no++;
        }
        get_next_char(parser);
    }
}

/**
 * 已支持小数
 */
static void parse_num_without_sign(Parser* parser) {
    while (is_num(parser->cur_char)) {
        get_next_char(parser);
    }
    if (parser->cur_char == '.' && is_num(look_ahead_char(parser))) {
        get_next_char(parser);
        while (is_num(parser->cur_char)) {
            get_next_char(parser);
        }
    }
    uint length = (uint)(parser->next_char_ptr-parser->cur_token.start-1);
    parser->cur_token.type = TOKEN_NUM;
    parser->cur_token.length = length;
    parser->cur_token.value = NUM_TO_VALUE(strtod(parser->cur_token.start, NULL));
}

static void parse_identifier(Parser* parser, TokenType type) {
    while (is_letter(parser->cur_char)||is_num(parser->cur_char)||parser->cur_char=='_') {
        get_next_char(parser);
    }
    uint length = (uint)(parser->next_char_ptr-parser->cur_token.start-1);
    if (type != TOKEN_UNKNOWN) {
        parser->cur_token.type = type;
    } else {
        parser->cur_token.type = is_keyword(parser->cur_token.start, length);
    }
    parser->cur_token.length = length;
}

/**
 * 解析字符串
 * TODO: 支持转义字符
 */
static void parse_string(Parser* parser) {
    while (1) {
        get_next_char(parser);
        if (parser->cur_char == '\0') {
            /* 错误处理 */
            fprintf(stderr, "%s", "");
        } else if (parser->cur_char == '\"') {
            parser->cur_token.type = TOKEN_STRING;
            break;
        } else {
            /* 其他字符 */
        }
    }
}

static void parse_char(Parser* parser) {
    while (1) {
        get_next_char(parser);
        if (parser->cur_char == '\0') {
            /* 错误处理 */
            fprintf(stderr, "%s", "");
        } else if (parser->cur_char == '\"') {
            parser->cur_token.type = TOKEN_STRING;
            break;
        } else {
            /* 其他字符 */
        }
    }
}

/* 跳过一行 */
/* static void skip_line(Parser* parser) {
    get_next_char(parser);
    while (parser->cur_char != '\0') {
        if (parser->cur_char == '\n') {
            parser->cur_token.line_no++;
            get_next_char(parser);
            break;
        }
        get_next_char(parser);
    }
} */

void get_next_token(Parser* parser) {
    parser->pre_token = parser->cur_token;
    skip_blanks(parser);
    parser->cur_token.type = TOKEN_EOF;
    parser->cur_token.length = 0;
    parser->cur_token.start = parser->next_char_ptr - 1;
    while (parser->cur_char != '\0') {
        switch (parser->cur_char) {
            case '(':
                parser->cur_token.type = TOKEN_LEFT_PAREN;
                break;
            case ')':
                parser->cur_token.type = TOKEN_RIGHT_PAREN;
                break;
            case '[':
                parser->cur_token.type = TOKEN_LEFT_BRACKET;
                break;
            case ']':
                parser->cur_token.type = TOKEN_RIGHT_BRACKET;
                break;
            case '{':
                parser->cur_token.type = TOKEN_LEFT_BRACE;
                break;
            case '}':
                parser->cur_token.type = TOKEN_RIGHT_BRACE;
                break;
            /*case '.':
                parser->cur_token.type = TOKEN_DOT;
                break;*/
            case '+':
                parser->cur_token.type = TOKEN_ADD;
                break;
            case '-':
                /* 在语法分析时处理负数 */
                parser->cur_token.type = TOKEN_SUB;
                break;
            case '*':
                parser->cur_token.type = TOKEN_MUL;
                break;
            case '/':
                parser->cur_token.type = TOKEN_DIV;
                break;
            case '>':
                if (match_next_char(parser, '=')) {
                    parser->cur_token.type = TOKEN_GREATE_EQUAL;
                } else {
                    parser->cur_token.type = TOKEN_GREATE;
                }
                break;
            case '<':
                if (match_next_char(parser, '=')) {
                    parser->cur_token.type = TOKEN_LESS_EQUAL;
                } else {
                    parser->cur_token.type = TOKEN_LESS;
                }
                break;
            case '=':
                if (match_next_char(parser, '=')) {
                    parser->cur_token.type = TOKEN_EQUAL;
                } else {
                    parser->cur_token.type = TOKEN_ASSIGN;
                }
                break;
            case ';':
                parser->cur_token.type = TOKEN_SEMICOLON;
                break;
            case ',':
                parser->cur_token.type = TOKEN_COMMA;
                break;
            case '\'':
                parse_char(parser);
                break;
            case '\"':
                parse_string(parser);
                break;
            case '!':
                if (match_next_char(parser, '=')) {
                    parser->cur_token.type = TOKEN_NOT_EQUAL;
                }
                /* error */
                break;
            default:
                /**
                 * 处理变量名及数字
                 */
                if (is_letter(parser->cur_char) || parser->cur_char=='_') {
                    get_next_char(parser);
                    parse_identifier(parser, TOKEN_UNKNOWN);
                } else if (is_num(parser->cur_char)) {
                    get_next_char(parser);
                    parse_num_without_sign(parser);
                }
                return;
        }
        parser->cur_token.length = (uint)(parser->next_char_ptr - parser->cur_token.start);
        get_next_char(parser);
        return;
    }
}