#pragma once
#include "buffer.h"

typedef struct Buffer {
    Line **lines;
    size_t len;
    size_t size;
    char *file_path;
} Buffer;
