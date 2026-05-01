#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "buffer_impl.h"

int buffer_open_file(Buffer *buffer, const char *file_path) {
    if (!file_path) return 0;

    buffer_close(buffer);

    FILE *file;

    file = fopen(file_path, "r");
    if (!file) {
        if (access(file_path, F_OK) == -1) {
            if (!buffer_resize(buffer, 0)) {
                fprintf(stderr, "Can't allocate memory for the buffer\n");
                return 0;
            }
            return 1;
        }
        return 0;
    }

    if (!buffer_resize(buffer, 0)) {
        fprintf(stderr, "Can't allocate memory for the buffer\n");
        return 0;
    }
    char *str = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&str, &len, file)) != -1) {
        buffer_add_line(buffer, str, read-1);
    }

    free(str);
    fclose(file);

    if (buffer->file_path) free(buffer->file_path);
    buffer->file_path = strdup(file_path);

    return 1;
}

int buffer_save_file(Buffer *buffer, const char *file_path) {
    if (!file_path) file_path = buffer->file_path;
    if (!file_path) return 0;

    FILE *file;

    file = fopen(file_path, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < buffer->len; i++) {
        fputs(buffer_line_text(buffer, i), file);
        fputs("\n", file);
    }

    fclose(file);

    return 1;
}
