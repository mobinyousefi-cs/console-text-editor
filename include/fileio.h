/*
 * Project: Console-Based Text Editor
 * File: fileio.h
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-23
 * Updated: 2025-11-23
 * License: MIT License (see LICENSE file for details)
 */

#ifndef FILEIO_H
#define FILEIO_H

#include "buffer.h"

/*
 * Loads the contents of `filename` into `buffer`.
 * Returns 0 on success, non-zero on error.
 * On failure, the buffer is left in a valid (possibly empty) state.
 */
int file_load(const char *filename, TextBuffer *buffer);

/*
 * Saves the contents of `buffer` into `filename`.
 * Returns 0 on success, non-zero on error.
 */
int file_save(const char *filename, const TextBuffer *buffer);

#endif /* FILEIO_H */
