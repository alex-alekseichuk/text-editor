#pragma once
#include <stdlib.h>
#include "line.h"

typedef struct Buffer Buffer;

Buffer *make_buffer();
void buffer_close(Buffer *buffer);
int buffer_resize(Buffer *buffer, size_t new_size);

int buffer_add_line(Buffer *buffer, char *str, size_t read);
int buffer_join_lines(Buffer *buffer, int i_line);
int buffer_split_line(Buffer *buffer, int i_line, int pos);

int buffer_open_file(Buffer *buffer, const char *file_path);
int buffer_save_file(Buffer *buffer, const char *file_path);

char *buffer_line_text(Buffer *buffer, size_t i_line);
int buffer_line_len(Buffer *buffer, size_t i_line);

size_t buffer_get_len(Buffer *buffer);
Line *buffer_get_line(Buffer *buffer, size_t i_line);

