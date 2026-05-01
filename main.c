#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include "buffer.h"
#include "view.h"
#include "controller.h"

static Buffer *buffer = NULL;
static View *view = NULL;

void exit_app() {
    if (buffer != NULL)
        buffer_close(buffer);
    if (view != NULL)
        exit(view_close(view));
    else
        exit(EXIT_SUCCESS);
}

void sigint_handler(int signum) {
    exit_app();
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);
    signal(SIGQUIT, sigint_handler);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <text-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_path = argv[1];
    
    buffer = make_buffer();
    if (buffer == NULL) {
        fprintf(stderr, "Can't allocate memory for the buffer\n");
        return EXIT_FAILURE;
    }

    if (!buffer_open_file(buffer, file_path)) {
        fprintf(stderr, "Can't open/create file: %s\n", file_path);
        return EXIT_FAILURE;
    }

    view = make_view();
    view_init(view);
    view_buffer(view, buffer);

    int result = ctrl_run(view, buffer);

    exit_app();
    return result;
}
