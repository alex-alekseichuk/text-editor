# Simple Text Editor

TUI text editor based on ncurses.

## Build

```bash
mkdir build
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Or by hand:
```bash
gcc main.c buffer/buffer.c buffer/line.c buffer/file.c view.c controller.c \
  -lncurses -ltinfo -o text-editor
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

## Issues

- Save failures are completely silent — buffer/file.c:51 + controller.c:19
   buffer_save_file ignores fopen/fputs/fclose failures, and the caller ignores its return value with no status message. (Reproduced): open a read-only file, type a character, press Ctrl+S — no feedback at all, the file is unchanged, but the screen looks normal. That's a quiet data-loss trap.
- CRLF lines are mangled — buffer/file.c:39
   str[read-1] == '\n' ? read-1 : read strips the \n but leaves the \r. (Reproduced): a Windows file a\r\nb\r\n loads as a^M, b^M and is saved back with mixed endings.

### Code-quality / latent issues

- No SIGWINCH/resize handling — rows/cols are captured once in view_init; resizing the terminal leaves the view stale.
- ESC quits without confirmation (controller.c:13), discarding unsaved changes; and since a bare 27 also results from any unrecognized escape sequence, a key with bad terminfo support can accidentally quit the editor.
- fprintf(stderr, ...) inside buffer functions (OOM paths) prints directly to the terminal while ncurses owns it, corrupting the screen.
