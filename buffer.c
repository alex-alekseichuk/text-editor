#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include "buffer_impl.h"

Buffer *make_buffer() {
    return malloc(sizeof(Buffer));
}

size_t buffer_get_len(Buffer *buffer) {
    return buffer->len;
}

Line *buffer_get_line(Buffer *buffer, size_t i_line) {
    if (i_line >= buffer->len)
       return NULL;
    return buffer->lines[i_line];
}

char *buffer_line_text(Buffer *buffer, size_t i_line) {
    if (i_line >= buffer->len)
        return "";
    if (NULL == buffer->lines[i_line])
        return "";
    return line_get_str(buffer->lines[i_line]);
}

int buffer_line_len(Buffer *buffer, size_t i_line) {
    if (i_line >= buffer->len)
        return 0;
    if (NULL == buffer->lines[i_line])
        return 0;
    return line_get_len(buffer->lines[i_line]);
}

void buffer_close(Buffer *buffer) {
    if (buffer->lines) {
        for (int i = 0; i < buffer->size; i++) {
            if (buffer->lines[i]) {
                line_free(buffer->lines[i]);
                free(buffer->lines[i]);
            }
        }
        free(buffer->lines);
        buffer->lines = NULL;
        buffer->len = 0;
        buffer->size = 0;
    }
    if (buffer->file_path) {
        free(buffer->file_path);
        buffer->file_path = NULL;
    }
}

int buffer_resize(Buffer *buffer, size_t new_size) {
    int prev_size = 0;
    int is_realloc = buffer->size > 0;
    if (new_size == 0) {
        if (buffer->size > 0) {
            prev_size = buffer->size;
            new_size = buffer->size * 2;
        } else {
            new_size = 32;
        }
    } else {
        if (buffer->size >= new_size)
            return 1;
        prev_size = buffer->size;        
    }
    if (is_realloc) {
        void *new_lines = realloc(buffer->lines, new_size * sizeof(Line *));
        if (!new_lines)
            return 0;
        buffer->lines = new_lines;
    } else {
        buffer->lines = calloc(new_size, sizeof(Line *));
        buffer->len = 0;
    }
    if (!buffer->lines) {
        return 0;
    }
    for (int i = prev_size; i < new_size; i++)
        buffer->lines[i] = NULL;
    buffer->size = new_size;
    return 1;
}

int buffer_add_line(Buffer *buffer, char *str, size_t read) {
    while (buffer->len >= buffer->size) {
        if (!buffer_resize(buffer, 0)) {
            fprintf(stderr, "Can't allocate memory for the buffer\n");
            return 0;
        }
    }

    Line *line = make_line();
    if (!line) {
        fprintf(stderr, "Can't allocate memory for the line\n");
        return 0;
    }

    if (!line_ncpy(line, str, read)) {
        fprintf(stderr, "Can't allocate memory for the line\n");
        free(line);
        return 0;
    }

    buffer->lines[buffer->len] = line;
    buffer->len++;
    return 1;
}

int buffer_join_lines(Buffer *buffer, int i_line) {
    if (i_line >= buffer->len - 1) return -1;
    int new_x = line_get_len(buffer->lines[i_line]);
    line_cat(buffer->lines[i_line], line_get_str(buffer->lines[i_line + 1]));

    line_free(buffer->lines[i_line + 1]);
    free(buffer->lines[i_line + 1]);
    buffer->lines[i_line + 1] = NULL;
    if (i_line < buffer->len - 1)
        memmove(&buffer->lines[i_line + 1], &buffer->lines[i_line + 2], sizeof(Line*) * (buffer->len - i_line - 2));
    buffer->len--;
    buffer->lines[buffer->len] = NULL;

    return new_x;
}

int buffer_split_line(Buffer *buffer, int i_line, int pos) {
    while (buffer->len >= buffer->size) {
        if (!buffer_resize(buffer, 0)) {
            fprintf(stderr, "Can't allocate memory for the buffer\n");
            return 0;
        }
    }

    if (i_line < buffer->len - 1)
        memmove(&buffer->lines[i_line + 2], &buffer->lines[i_line + 1], sizeof(Line*) * (buffer->len - i_line - 1));

    buffer->lines[i_line + 1] = make_line();
    size_t n = line_get_len(buffer->lines[i_line]);
    if (pos < n) {
        int size = n - pos;
        const char *s = line_get_str(buffer->lines[i_line]);
        line_ncpy(buffer->lines[i_line + 1], &s[pos], size);

        line_truncate(buffer->lines[i_line], pos);
    } else {
        line_cpy(buffer->lines[i_line + 1], "\0");
    }

    buffer->len++;
    return 1;
}

