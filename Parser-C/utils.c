#include "utils.h"

// static char* root_dir = NULL;

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        /* IO错误 */
    } 
    struct stat file_stat;
    stat(path, &file_stat);
    size_t file_size = file_stat.st_size;
    char* file_content = malloc(file_size+1);
    if (file_content == NULL) {

    }
    size_t num_read = fread(file_content, sizeof(char), file_size, file);
    if (num_read < file_size) {

    }
    file_content[file_size] = '\0';
    fclose(file);
    return file_content;
}