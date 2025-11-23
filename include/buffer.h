/*
 * Project: Console-Based Text Editor
 * File: buffer.h
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#define INVALID_INDEX ((size_t)-1)

typedef struct {
    char **lines;
    size_t count;
    size_t capacity;
} TextBuffer;

void buffer_init(TextBuffer *buffer);
void buffer_free(TextBuffer *buffer);

int buffer_insert_line(TextBuffer *buffer, size_t index, const char *text);
int buffer_append_line(TextBuffer *buffer, const char *text);
int buffer_delete_line(TextBuffer *buffer, size_t index);
int buffer_replace_line(TextBuffer *buffer, size_t index, const char *text);

const char *buffer_get_line(const TextBuffer *buffer, size_t index);
void buffer_print(const TextBuffer *buffer);

/* Returns index of the first matching line, or INVALID_INDEX if not found */
size_t buffer_find(const TextBuffer *buffer, const char *needle);

#endif /* BUFFER_H */
