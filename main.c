#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "buffer/buffer.h"
#include "view.h"
#include "controller.h"

static Buffer *buffer = NULL;
static View *view = NULL;

void sigint_handler(int signum) {
    (void)signum;
    ctrl_stop();
}

int main(int argc, char *argv[]) {
    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "Editor requires an interactive terminal\n");
        return 1;
    }

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
    if (view == NULL) {
        fprintf(stderr, "Can't allocate memory for the view\n");
        return EXIT_FAILURE;
    }
    view_init(view);
    view_buffer(view, buffer);

    ctrl_run(view, buffer);

    buffer_close(buffer);
    free(buffer);
    buffer = NULL;

    view_close(view);
    free(view);
    view = NULL;

    return EXIT_SUCCESS;
}
