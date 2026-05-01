#pragma once
#include <stdlib.h>

typedef struct Line Line;

Line *make_line();
size_t line_get_len(Line *line);
size_t line_get_size(Line *line);
char *line_get_str(Line *line);
int line_ncpy(Line *line, const char *str, size_t n);
int line_cpy(Line *line, const char *str);
int line_ncat(Line *line, const char *str, size_t n);
int line_cat(Line *line, const char *str);
void line_truncate(Line *line, size_t n);
void line_free(Line *line);
int line_resize(Line *line, size_t new_size);
int line_insert_char(Line *line, int pos, char ch);
int line_delete_char(Line *line, int pos);
int line_backspace_char(Line *line, int pos);
