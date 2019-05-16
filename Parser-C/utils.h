#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned int uint;
typedef int bool;

#define true 1;
#define false 0;

/*typedef struct {
    char* str;
    uint length;
} String;*/

/* 字符串缓冲区 */
/*typedef struct {
    uint length;
    char start[0];
} CharValue;*/

/* 读取源代码文件 */ 
char* read_file(const char*);