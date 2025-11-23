/*
 * Project: Console-Based Text Editor
 * File: editor.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#include "editor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "util.h"

#define INPUT_BUFFER_SIZE 1024

static void editor_print_header(const EditorState *editor)
{
    printf("\n================ Console Text Editor ================\n");
    printf("File    : %s\n", editor->current_filename[0] ? editor->current_filename : "<unnamed>");
    printf("Status  : %s\n", editor->is_modified ? "modified" : "saved");
    printf("Lines   : %zu\n", editor->buffer.count);
    printf("====================================================\n\n");
}

static void editor_print_menu(void)
{
    printf("Commands:\n");
    printf(" 1) View buffer\n");
    printf(" 2) Insert line at position\n");
    printf(" 3) Append line\n");
    printf(" 4) Edit existing line\n");
    printf(" 5) Delete line\n");
    printf(" 6) Search text\n");
    printf(" 7) Save\n");
    printf(" 8) Save As\n");
    printf(" 9) Quit\n");
    printf("----------------------------------------------------\n");
}

static int prompt_for_index(size_t *out_index, const char *label, size_t max_value)
{
    char input[INPUT_BUFFER_SIZE];

    printf("%s (1-%zu): ", label, max_value);
    if (read_line(input, sizeof(input)) != 0) {
        return -1;
    }

    char *endptr = NULL;
    unsigned long value = strtoul(input, &endptr, 10);
    if (endptr == input || *endptr != '\0' || value == 0 || value > max_value) {
        printf("Invalid number.\n");
        return -1;
    }

    *out_index = (size_t)(value - 1); /* convert to 0-based */
    return 0;
}

static void command_view(EditorState *editor)
{
    (void)editor; /* unused warning guard if needed */
    buffer_print(&editor->buffer);
}

static void command_insert(EditorState *editor)
{
    if (editor->buffer.count == 0) {
        printf("Buffer is empty; inserting as first line.\n");
    }

    size_t max_pos = editor->buffer.count + 1;
    size_t index = 0;

    char line[INPUT_BUFFER_SIZE];

    printf("Enter position to insert at (1-%zu): ", max_pos);
    if (read_line(line, sizeof(line)) != 0) {
        return;
    }

    char *endptr = NULL;
    unsigned long pos = strtoul(line, &endptr, 10);
    if (endptr == line || *endptr != '\0' || pos == 0 || pos > max_pos) {
        printf("Invalid position.\n");
        return;
    }

    index = (size_t)(pos - 1);

    printf("Enter text: ");
    if (read_line(line, sizeof(line)) != 0) {
        return;
    }

    if (buffer_insert_line(&editor->buffer, index, line) != 0) {
        printf("Failed to insert line (out of memory?).\n");
        return;
    }

    editor->is_modified = 1;
}

static void command_append(EditorState *editor)
{
    char line[INPUT_BUFFER_SIZE];

    printf("Enter text to append: ");
    if (read_line(line, sizeof(line)) != 0) {
        return;
    }

    if (buffer_append_line(&editor->buffer, line) != 0) {
        printf("Failed to append line (out of memory?).\n");
        return;
    }

    editor->is_modified = 1;
}

static void command_edit(EditorState *editor)
{
    if (editor->buffer.count == 0) {
        printf("Buffer is empty. Nothing to edit.\n");
        return;
    }

    size_t index = 0;
    if (prompt_for_index(&index, "Enter line number to edit", editor->buffer.count) != 0) {
        return;
    }

    const char *old_line = buffer_get_line(&editor->buffer, index);
    printf("Current text: %s\n", old_line ? old_line : "");

    char line[INPUT_BUFFER_SIZE];
    printf("Enter new text: ");
    if (read_line(line, sizeof(line)) != 0) {
        return;
    }

    if (buffer_replace_line(&editor->buffer, index, line) != 0) {
        printf("Failed to replace line (out of memory?).\n");
        return;
    }

    editor->is_modified = 1;
}

static void command_delete(EditorState *editor)
{
    if (editor->buffer.count == 0) {
        printf("Buffer is empty. Nothing to delete.\n");
        return;
    }

    size_t index = 0;
    if (prompt_for_index(&index, "Enter line number to delete", editor->buffer.count) != 0) {
        return;
    }

    if (buffer_delete_line(&editor->buffer, index) != 0) {
        printf("Failed to delete line.\n");
        return;
    }

    editor->is_modified = 1;
}

static void command_search(EditorState *editor)
{
    (void)editor;

    char query[INPUT_BUFFER_SIZE];

    printf("Enter search text: ");
    if (read_line(query, sizeof(query)) != 0) {
        return;
    }

    if (query[0] == '\0') {
        printf("Empty search string.\n");
        return;
    }

    size_t index = buffer_find(&editor->buffer, query);
    if (index == INVALID_INDEX) {
        printf("No match found for '%s'.\n", query);
    } else {
        const char *line = buffer_get_line(&editor->buffer, index);
        printf("First match at line %zu: %s\n", index + 1, line ? line : "");
    }
}

static int perform_save(EditorState *editor, const char *filename)
{
    if (file_save(filename, &editor->buffer) != 0) {
        printf("Failed to save file '%s'.\n", filename);
        return -1;
    }

    strncpy(editor->current_filename, filename, EDITOR_FILENAME_MAX - 1);
    editor->current_filename[EDITOR_FILENAME_MAX - 1] = '\0';
    editor->is_modified = 0;

    printf("Saved to '%s'.\n", editor->current_filename);
    return 0;
}

static void command_save(EditorState *editor)
{
    if (editor->current_filename[0] == '\0') {
        /* No current filename, fall back to Save As */
        char filename[INPUT_BUFFER_SIZE];
        printf("Enter filename to save as: ");
        if (read_line(filename, sizeof(filename)) != 0 || filename[0] == '\0') {
            printf("Save cancelled.\n");
            return;
        }
        perform_save(editor, filename);
    } else {
        perform_save(editor, editor->current_filename);
    }
}

static void command_save_as(EditorState *editor)
{
    char filename[INPUT_BUFFER_SIZE];

    printf("Enter new filename: ");
    if (read_line(filename, sizeof(filename)) != 0 || filename[0] == '\0') {
        printf("Save As cancelled.\n");
        return;
    }

    perform_save(editor, filename);
}

static int confirm_discard_changes(void)
{
    char input[INPUT_BUFFER_SIZE];

    printf("You have unsaved changes. Quit anyway? (y/n): ");
    if (read_line(input, sizeof(input)) != 0) {
        return 0;
    }

    return (input[0] == 'y' || input[0] == 'Y');
}

void editor_init(EditorState *editor, const char *filename)
{
    if (!editor) {
        return;
    }

    buffer_init(&editor->buffer);
    editor->current_filename[0] = '\0';
    editor->is_modified = 0;

    if (filename && filename[0] != '\0') {
        if (file_load(filename, &editor->buffer) == 0) {
            printf("Opened existing file '%s'.\n", filename);
        } else {
            printf("Starting new file '%s'.\n", filename);
        }
        strncpy(editor->current_filename, filename, EDITOR_FILENAME_MAX - 1);
        editor->current_filename[EDITOR_FILENAME_MAX - 1] = '\0';
    } else {
        printf("Starting new unnamed buffer.\n");
    }
}

void editor_run(EditorState *editor)
{
    if (!editor) {
        return;
    }

    char input[INPUT_BUFFER_SIZE];

    for (;;) {
        editor_print_header(editor);
        editor_print_menu();

        printf("Enter choice: ");
        if (read_line(input, sizeof(input)) != 0) {
            printf("\nEnd of input detected. Exiting.\n");
            break;
        }

        if (input[0] == '\0') {
            continue;
        }

        int choice = atoi(input);

        switch (choice) {
        case 1:
            command_view(editor);
            break;
        case 2:
            command_insert(editor);
            break;
        case 3:
            command_append(editor);
            break;
        case 4:
            command_edit(editor);
            break;
        case 5:
            command_delete(editor);
            break;
        case 6:
            command_search(editor);
            break;
        case 7:
            command_save(editor);
            break;
        case 8:
            command_save_as(editor);
            break;
        case 9:
            if (editor->is_modified) {
                if (!confirm_discard_changes()) {
                    printf("Quit cancelled.\n");
                    break;
                }
            }
            printf("Goodbye.\n");
            return;
        default:
            printf("Unknown command: %d\n", choice);
            break;
        }
    }
}

void editor_free(EditorState *editor)
{
    if (!editor) {
        return;
    }

    buffer_free(&editor->buffer);
}
