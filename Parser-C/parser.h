/**
 * 关键字就是keyword中的那些；
 * 标识符（identifier）就处理为变量名和函数名；
 * 操作符，把;和'单独拿出来，分别作为分隔符和单个字符标志；
 * 扩展添加了"作为字符串的标志
 */

#include "utils.h"
#include "header_obj.h"

#define NUM_TO_VALUE(num) ((Value){VT_NUM, {num}})

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_NUM,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_IDENTIFIER,
    /* KeyWord */
    TOKEN_IF,                // if
    TOKEN_THEN,              // then
    TOKEN_ELSE,              // else
    TOKEN_WHILE,             // while
    TOKEN_DO,                // do
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_SEMICOLON,         //;
    TOKEN_COMMA,             //,
    TOKEN_LEFT_BRACKET,      //[
    TOKEN_RIGHT_BRACKET,     //]
    TOKEN_LEFT_BRACE,        //{
    /* } */
    TOKEN_RIGHT_BRACE,       //}
    /* . */
    TOKEN_DOT,
    TOKEN_LEFT_PAREN,        //(
    TOKEN_RIGHT_PAREN,       //)
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_ASSIGN, //=
    TOKEN_GREATE, //>
    TOKEN_LESS, //<
    TOKEN_EQUAL, //==
    TOKEN_GREATE_EQUAL,// >=
    TOKEN_LESS_EQUAL, //<=
    TOKEN_NOT_EQUAL, //!=
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    uint length;
    uint line_no;
    Value value;
} Token;

typedef struct _Parser Parser;

struct _Parser {
    const char *file_name;
    const char *source_code;
    const char *next_char_ptr;
    char cur_char;
    Token cur_token;
    Token pre_token;
};

/* 返回当前字符cur_char的下一个字符 */
char look_ahead_char(Parser *parser);

/* 词法分析器的入口函数 */
void get_next_token(Parser *parser);

// bool match_token(Parser* parser, TokenType expected);

/* 判断是否是字母，包括大写和小写 */
bool is_letter(const char);

/* 判断是否是一个数字字符 */
bool is_num(const char);

