#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "line.h"

typedef struct Line {
    char *str;
    size_t len;
    size_t size;
} Line;

Line *make_line() {
    Line *line = (Line *)malloc(sizeof(Line));
    if (line == NULL) return NULL;
    line->str = NULL;
    line->len = 0;
    line->size = 0;
    return line;
}

size_t line_get_len(Line *line) {
    return line->len;
}

size_t line_get_size(Line *line) {
    return line->size;
}

char *line_get_str(Line *line) {
    return line->str;
}

int line_ncpy(Line *line, const char *str, size_t n) {
    if (n > strlen(str)) n = strlen(str);
    if (!line_resize(line, n)) return 0;
    strncpy(line->str, str, n);
    line->str[n] = '\0';
    line->len = n;
    return 1;
}

int line_cpy(Line *line, const char *str) {
    return line_ncpy(line, str, strlen(str));
}

int line_ncat(Line *line, const char *str, size_t n) {
    if (n > strlen(str)) n = strlen(str);
    size_t n_existing = line_get_len(line);
    if (!line_resize(line, n_existing + n)) return 0;
    strncpy(line->str + n_existing, str, n);
    line->str[n_existing + n] = '\0';
    line->len = n_existing + n;
    return 1;
}

int line_cat(Line *line, const char *str) {
    return line_ncat(line, str, strlen(str));
}

void line_truncate(Line *line, size_t n) {
    if (n >= line->len) return;
    line->len = n;
    line->str[n] = '\0';
}

void line_free(Line *line) {
    if (line->str) {
        free(line->str);
        line->str = NULL;
        line->size = 0;
        line->len = 0;
    }
}

int line_resize(Line *line, size_t new_size) {
    int prev_size = 0;
    int is_realloc = line->size > 0;
    if (new_size == 0) {
        if (line->size > 0) {
            prev_size = line->size;
            new_size = line->size * 2;
        } else {
            new_size = 32;
        }
    } else {
        if (line->size >= new_size)
            return 1;
        prev_size = line->size;
        new_size = new_size;
    }
    if (is_realloc) {
        void *tmp = realloc(line->str, new_size + 1);
        if (!tmp)
            return 0;
        line->str = tmp;
    } else {
        line->str = malloc(new_size + 1);
        line->len = 0;
    }
    if (!line->str) {
        return 0;
    }
    for (int i = prev_size; i < new_size; i++)
        line->str[i] = '\0';
    line->size = new_size;
    return 1;
}

int line_insert_char(Line *line, int pos, char ch) {
    if (pos > line->len) return 0;

    while (line->len >= line->size) {
        if (!line_resize(line, 0)) {
            fprintf(stderr, "Can't allocate memory for the line\n");
            return 0;
        }
    }

    if (pos < line->len) {
        memmove(&line->str[pos + 1], &line->str[pos], line->len - pos);
    }

    line->str[pos] = ch;
    line->len++;

    return 1;
}

int line_delete_char(Line *line, int pos) {
    if (line == NULL) return 0;
    if (line->len == 0 || pos >= line->len) return 0;
    if (pos < line->len - 1)
        memmove(&line->str[pos], &line->str[pos + 1], line->len - pos - 1);
    line->len--;
    line->str[line->len] = '\0';
    return 1;
}

int line_backspace_char(Line *line, int pos) {
    if (line == NULL) return 0;
    if (pos == 0 || pos > line->len) return 0;
    if (pos < line->len)
        memmove(&line->str[pos-1], &line->str[pos], line->len - pos);
    line->len--;
    line->str[line->len] = '\0';
    return 1;
}
