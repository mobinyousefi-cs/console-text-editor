/*
 * Project: Console-Based Text Editor
 * File: buffer.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

static char *duplicate_string(const char *src)
{
    if (!src) {
        return NULL;
    }

    size_t len = strlen(src);
    char *copy = (char *)malloc(len + 1);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, src, len + 1);
    return copy;
}

static int ensure_capacity(TextBuffer *buffer, size_t min_capacity)
{
    if (buffer->capacity >= min_capacity) {
        return 0;
    }

    size_t new_capacity = buffer->capacity ? buffer->capacity : INITIAL_CAPACITY;
    while (new_capacity < min_capacity) {
        new_capacity *= 2;
    }

    char **new_lines = (char **)realloc(buffer->lines, new_capacity * sizeof(char *));
    if (!new_lines) {
        return -1;
    }

    buffer->lines = new_lines;
    buffer->capacity = new_capacity;
    return 0;
}

void buffer_init(TextBuffer *buffer)
{
    if (!buffer) {
        return;
    }
    buffer->lines = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

void buffer_free(TextBuffer *buffer)
{
    if (!buffer) {
        return;
    }

    for (size_t i = 0; i < buffer->count; ++i) {
        free(buffer->lines[i]);
    }
    free(buffer->lines);
    buffer->lines = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

int buffer_insert_line(TextBuffer *buffer, size_t index, const char *text)
{
    if (!buffer || index > buffer->count) {
        return -1;
    }

    if (ensure_capacity(buffer, buffer->count + 1) != 0) {
        return -1;
    }

    for (size_t i = buffer->count; i > index; --i) {
        buffer->lines[i] = buffer->lines[i - 1];
    }

    buffer->lines[index] = duplicate_string(text ? text : "");
    if (!buffer->lines[index]) {
        return -1;
    }

    buffer->count++;
    return 0;
}

int buffer_append_line(TextBuffer *buffer, const char *text)
{
    return buffer_insert_line(buffer, buffer->count, text);
}

int buffer_delete_line(TextBuffer *buffer, size_t index)
{
    if (!buffer || index >= buffer->count) {
        return -1;
    }

    free(buffer->lines[index]);

    for (size_t i = index; i + 1 < buffer->count; ++i) {
        buffer->lines[i] = buffer->lines[i + 1];
    }

    buffer->count--;
    return 0;
}

int buffer_replace_line(TextBuffer *buffer, size_t index, const char *text)
{
    if (!buffer || index >= buffer->count) {
        return -1;
    }

    char *new_line = duplicate_string(text ? text : "");
    if (!new_line) {
        return -1;
    }

    free(buffer->lines[index]);
    buffer->lines[index] = new_line;
    return 0;
}

const char *buffer_get_line(const TextBuffer *buffer, size_t index)
{
    if (!buffer || index >= buffer->count) {
        return NULL;
    }
    return buffer->lines[index];
}

void buffer_print(const TextBuffer *buffer)
{
    if (!buffer || buffer->count == 0) {
        printf("[Buffer is empty]\n");
        return;
    }

    for (size_t i = 0; i < buffer->count; ++i) {
        printf("%zu: %s\n", i + 1, buffer->lines[i] ? buffer->lines[i] : "");
    }
}

size_t buffer_find(const TextBuffer *buffer, const char *needle)
{
    if (!buffer || !needle || needle[0] == '\0') {
        return INVALID_INDEX;
    }

    for (size_t i = 0; i < buffer->count; ++i) {
        if (buffer->lines[i] && strstr(buffer->lines[i], needle) != NULL) {
            return i;
        }
    }

    return INVALID_INDEX;
}
