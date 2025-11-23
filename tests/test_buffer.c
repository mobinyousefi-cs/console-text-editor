/*
 * Project: Console-Based Text Editor
 * File: test_buffer.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#include <assert.h>
#include <stdio.h>

#include "buffer.h"

int main(void)
{
    TextBuffer buf;
    buffer_init(&buf);

    /* Append */
    assert(buffer_append_line(&buf, "Hello") == 0);
    assert(buffer_append_line(&buf, "World") == 0);
    assert(buf.count == 2);

    /* Insert */
    assert(buffer_insert_line(&buf, 1, "Inserted") == 0);
    assert(buf.count == 3);

    /* Replace */
    assert(buffer_replace_line(&buf, 0, "Hi") == 0);

    /* Find */
    size_t idx = buffer_find(&buf, "World");
    assert(idx != INVALID_INDEX);

    /* Delete */
    assert(buffer_delete_line(&buf, idx) == 0);
    assert(buf.count == 2);

    printf("All buffer tests passed.\n");

    buffer_free(&buf);
    return 0;
}
