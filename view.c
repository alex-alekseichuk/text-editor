#include <ncurses.h>
#include <locale.h>
#include "buffer.h"

typedef struct View {
    int cols;
    int rows;
    size_t top_line;
    int x, y;
    Buffer *buffer;
} View;

View *make_view() {
    return malloc(sizeof(View));
}

void view_init(View *view) {
    setlocale(LC_ALL, "");
    initscr();
    view->cols = getmaxx(stdscr);
    view->rows = getmaxy(stdscr);

    cbreak();
    noecho();
    curs_set(0);
}

int view_close(View *view) {
    return endwin() == ERR;
}

void clear_line(int y) {
    move(y, 0);
    clrtoeol();
}
void show_line(View *view, int y) {
    char *s = buffer_line_text(view->buffer, y + view->top_line);
    if (s != NULL && s[0] != '\0') {
        mvaddstr(y, 0, s);
    }

}

void show_cursor(View *view) {
    size_t buffer_len = buffer_get_len(view->buffer);
    if (view->y >= buffer_len - view->top_line) view->y = buffer_len - view->top_line - 1;
    int len = buffer_line_len(view->buffer, view->y + view->top_line);
    if (view->x > len)
       view->x = len;
    move(view->y, view->x);
}

void show_view(View *view) {
    erase();
    for (int i = 0; i < view->rows; i++)
        show_line(view, i);
    refresh();
    show_cursor(view);
}

void view_buffer(View *view, Buffer *buffer) {
    view->buffer = buffer;

    view->top_line = 0;
    view->x = 0;
    view->y = 0;

    show_view(view);
    curs_set(1);
}

void cmd_up(View *view) {
    if (view->y > 0) {
        view->y--;
        show_cursor(view);
    } else {
        if (view->top_line > 0) {
            view->top_line--;
            show_view(view);
        }
    }
}

void cmd_down(View *view) {
    size_t buffer_len = buffer_get_len(view->buffer);
    if (view->y < view->rows - 1) {
        if (view->y < buffer_len - view->top_line - 1) {
            view->y++;
            show_cursor(view);
        }
    } else {
        if (view->top_line + view->rows < buffer_len) {
            view->top_line++;
            show_view(view);
        }
    }
}

void cmd_left(View *view) {
    if (view->x > 0) {
        view->x--;
        show_cursor(view);
    }
}

void cmd_right(View *view) {
    int n = buffer_line_len(view->buffer, view->y + view->top_line);
    if (view->x < view->cols - 1 && view->x < n) {
        view->x++;
        show_cursor(view);
    }
}

void cmd_begin_line(View *view) {
    if (view->x == 0) return;
    view->x = 0;
    show_cursor(view);
}

void cmd_end_line(View *view) {
    int n = buffer_line_len(view->buffer, view->y + view->top_line);
    if (view->x == view->cols - 1 || view->x == n) return;
    view->x = n;
    show_cursor(view);
}

void cmd_page_up(View *view) {
    if (view->top_line > 0) {
        view->top_line -= view->rows - 1; 
        if (view->top_line < 0) view->top_line = 0;
    } else if (view->y > 0) {
        view->y = 0;
    } else {
        return;
    }
    show_view(view);
}

void cmd_page_down(View *view) {
    size_t buffer_len = buffer_get_len(view->buffer);
    if (buffer_len > view->top_line + view->rows) {
        view->top_line += view->rows - 1;
        if (view->top_line > buffer_len - view->rows) view->top_line = buffer_len - view->rows;
    } else if (view->y < view->rows - 1 && view->y < buffer_len - view->top_line) {
        view->y = view->rows - 1;
        if (view->y + view->top_line >= buffer_len) view->y = buffer_len - view->top_line - 1;
    } else {
        return;
    }
    show_view(view);
}

void cmd_insert_char(View *view, char ch) {
    if (!line_insert_char(buffer_get_line(view->buffer, view->y + view->top_line), view->x, ch))
    {
        return;
    }

    show_line(view, view->y);
    cmd_right(view);
}

void cmd_delete_char(View *view) {
    size_t buffer_len = buffer_get_len(view->buffer);
    int n = buffer_line_len(view->buffer, view->y + view->top_line);
    if (view->x == n) {
        if (view->y + view->top_line + 1 >= buffer_len) return;
        int new_x = buffer_join_lines(view->buffer, view->y + view->top_line);
        if (new_x == -1) return;

        show_view(view);
        return;
    }
    if (!line_delete_char(buffer_get_line(view->buffer, view->y + view->top_line), view->x)) return;
    
    clear_line(view->y);
    show_line(view, view->y);
    show_cursor(view);
}

void cmd_backspace_char(View *view) {
    if (view->x == 0) {
        if (view->y + view->top_line == 0) return;
        int new_x = buffer_join_lines(view->buffer, view->y + view->top_line - 1);
        if (new_x == -1) return;

        view->x = new_x;
        if (view->y > 0) view->y--; else view->top_line--;
        show_view(view);
        return;
    }
    if (!line_backspace_char(buffer_get_line(view->buffer, view->y + view->top_line), view->x)) return;

    clear_line(view->y);
    show_line(view, view->y);
    view->x--;
    show_cursor(view);
}

void cmd_enter(View *view) {
    if (!buffer_split_line(view->buffer, view->y + view->top_line, view->x)) return;

    if (view->y == view->rows - 1) view->top_line++; else view->y++;
    view->x = 0;  
    show_view(view);
    show_cursor(view);
}
