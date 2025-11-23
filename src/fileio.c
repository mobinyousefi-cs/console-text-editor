/*
 * Project: Console-Based Text Editor
 * File: fileio.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 1024

int file_load(const char *filename, TextBuffer *buffer)
{
    if (!filename || !buffer) {
        return -1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        /* Treat as non-fatal: caller may want to start with an empty buffer */
        return -1;
    }

    buffer_free(buffer);
    buffer_init(buffer);

    char line[LINE_BUFFER_SIZE];

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Strip trailing newlines */
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (buffer_append_line(buffer, line) != 0) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

int file_save(const char *filename, const TextBuffer *buffer)
{
    if (!filename || !buffer) {
        return -1;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        return -1;
    }

    for (size_t i = 0; i < buffer->count; ++i) {
        const char *line = buffer->lines[i] ? buffer->lines[i] : "";
        if (fputs(line, fp) == EOF || fputc('\n', fp) == EOF) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) == EOF) {
        return -1;
    }

    return 0;
}
