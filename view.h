#pragma once

typedef struct View View;

View *make_view();
void view_init(View *view);
void view_buffer(View *view, Buffer *_buffer);
int view_close(View *view);

void cmd_up(View *view);
void cmd_down(View *view);
void cmd_left(View *view);
void cmd_right(View *view);
void cmd_begin_line(View *view);
void cmd_end_line(View *view);
void cmd_page_up(View *view);
void cmd_page_down(View *view);
void cmd_insert_char(View *view, char ch);
void cmd_delete_char(View *view);
void cmd_backspace_char(View *view);
void cmd_enter(View *view);
