# Simple Text Editor

TUI text editor based on ncurses.

## Build

```bash
gcc main.c buffer.c line.c file.c view.c controller.c -lncurses -ltinfo -o text-editor
```

## Run

```bash
./text-editor example.txt
```

| Key     | Action |
|---------|--------|
| F2      | save   |
| F10/Esc | exit   |

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
