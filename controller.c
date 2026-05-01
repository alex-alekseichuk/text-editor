#include <ncurses.h>
#include "buffer.h"
#include "view.h"

#define CTRL(c) ((c) & 0x1f)

int ctrl_run(View *view, Buffer *buffer) {
    keypad(stdscr, TRUE);
    set_escdelay(25);

    int key;
    while(key = getch()) {
        if (key == KEY_F(10) || key == 27) {
            break;
        }
        switch (key) {
            case KEY_F(2):
            case CTRL('s'): 
                buffer_save_file(buffer, NULL);
                break;
            case KEY_UP:
                cmd_up(view);
                break;
            case KEY_DOWN:
               cmd_down(view);
                break;
            case KEY_LEFT:
                cmd_left(view);
                break;
            case KEY_RIGHT:
                cmd_right(view);
                break;
            case KEY_HOME:
                cmd_begin_line(view);
                break;
            case KEY_END:
                cmd_end_line(view);
                break;
            case KEY_PPAGE:
                cmd_page_up(view);
                break;
            case KEY_NPAGE:
                cmd_page_down(view);
                break;
            case KEY_DC:
                cmd_delete_char(view);
                break;
            case KEY_BACKSPACE:
            case 127:
            case 8:
                cmd_backspace_char(view);
                break;
            case KEY_ENTER:
            case '\n':
            case '\r':
                cmd_enter(view);
                break;
        }
        if (key >= 32 && key <= 126) {
            cmd_insert_char(view, key);
        }
    }

    return 0;
}
