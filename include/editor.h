/*
 * Project: Console-Based Text Editor
 * File: editor.h
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#ifndef EDITOR_H
#define EDITOR_H

#include "buffer.h"

#define EDITOR_FILENAME_MAX 260

typedef struct {
    TextBuffer buffer;
    char current_filename[EDITOR_FILENAME_MAX];
    int is_modified;
} EditorState;

void editor_init(EditorState *editor, const char *filename);
void editor_run(EditorState *editor);
void editor_free(EditorState *editor);

#endif /* EDITOR_H */
