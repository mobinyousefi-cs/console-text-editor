# Console-Based Text Editor (C)

A lightweight, modular, console-based text editor implemented in C.  
It supports interactive text editing operations such as inserting, editing, deleting, searching, and saving lines.  
The project is structured professionally with reusable modules, a Makefile, and unit tests.

---

## Features

- Create and edit text files directly from the terminal
- Insert, append, replace, and delete lines
- Search within the buffer
- Load existing text files
- Save and Save-As functionality
- Detection of unsaved changes
- Modular architecture (buffer, IO, editor engine)
- Unit tests for buffer operations (C assert-based)
- MIT Licensed

---

## Project Structure

```
console-text-editor/
├── src/
│   ├── main.c
│   ├── editor.c
│   ├── buffer.c
│   ├── fileio.c
│   └── util.c
├── include/
│   ├── editor.h
│   ├── buffer.h
│   ├── fileio.h
│   └── util.h
├── tests/
│   └── test_buffer.c
├── Makefile
├── .gitignore
└── LICENSE
```

---

## Build Instructions

### Linux / macOS / WSL

```bash
make
```

The compiled binary will be placed in:

```
bin/text_editor
```

---

## Usage

### Start editor with an empty buffer:
```bash
./bin/text_editor
```

### Open or create a file:
```bash
./bin/text_editor notes.txt
```

### In-app commands (menu-driven):
- View buffer
- Insert line at position
- Append line
- Edit line
- Delete line
- Search text
- Save
- Save As
- Quit (warns if unsaved changes exist)

---

## Run Tests

```bash
make test
```

---

## License

This project is released under the **MIT License**.  
You are free to use, modify, and distribute the code.

---

## Author

**Mobin Yousefi**  
GitHub: https://github.com/mobinyousefi-cs
