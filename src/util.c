/*
 * Project: Console-Based Text Editor
 * File: util.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#include "util.h"

#include <stdio.h>
#include <string.h>

void trim_newline(char *s)
{
    if (!s) {
        return;
    }

    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

int read_line(char *buffer, size_t size)
{
    if (!buffer || size == 0) {
        return -1;
    }

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return -1; /* EOF or error */
    }

    trim_newline(buffer);
    return 0;
}
