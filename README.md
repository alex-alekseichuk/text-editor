# Simple Text Editor

TUI text editor based on ncurses.

## Build

```bash
gcc main.c buffer/buffer.c buffer/line.c buffer/file.c view.c controller.c -lncurses -ltinfo -o text-editor
```

## Run

```bash
./text-editor example.txt
```

| Key     | Action |
|---------|--------|
| F2      | save   |
| F10/Esc | exit   |


The model is called a buffer, as in vi/vim.
The buffer is simply a dynamic array of lines, and each line is a dynamic array of characters.
For simplicity, the editor supports only ASCII.
There are two basic operations for persistence:
- read text from a file into the buffer
- write the buffer back to the file.

In the presentation layer, it’s a pure terminal application (TUI) built with the ncurses library.
The screen displays text line by line, without wrapping or horizontal scrolling.
Cursor navigation supports standard movements, home/end within a line, and page up/down scrolling.

There is only an insert mode. The editor supports splitting and inserting lines, deleting characters with delete/backspace, and joining lines.

Although it’s written in pure C,
both the model (buffer) and the view components encapsulate their internal data structures and expose a clean, minimal API.

## Absent features

- ascii only
- no horizontal scroll
- no wrapping
- no syntax highlighting
- no tabs
- no clipboard
- no search
- no replace
- no undo
- no redo
- no bookmarks
- no line numbers
- no column mode
- no block mode
