/*
 * Project: Console-Based Text Editor
 * File: main.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#include <stdio.h>
#include <string.h>

#include "editor.h"

#define FILENAME_MAX_LEN 260

static void print_usage(const char *prog_name)
{
    printf("Usage: %s [file]\n", prog_name);
}

int main(int argc, char *argv[])
{
    char filename[FILENAME_MAX_LEN] = {0};

    if (argc > 2) {
        fprintf(stderr, "Error: too many arguments.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (argc == 2) {
        strncpy(filename, argv[1], FILENAME_MAX_LEN - 1);
        filename[FILENAME_MAX_LEN - 1] = '\0';
    }

    EditorState editor;
    editor_init(&editor, (argc == 2) ? filename : NULL);
    editor_run(&editor);
    editor_free(&editor);

    return 0;
}
