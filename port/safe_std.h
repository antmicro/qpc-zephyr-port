// SPDX-License-Identifier: Apache-2.0

#ifndef SAFE_STD_H
#define SAFE_STD_H

#include <stdio.h>
#include <string.h>

#define MEMMOVE_S(dest_, num_, src_, count_) \
    memmove(dest_, src_, count_)

#define STRNCPY_S(dest_, destsiz_, src_) do { \
    strncpy(dest_, src_, destsiz_);           \
    dest_[(destsiz_) - 1] = '\0';             \
} while (false)

#define STRCAT_S(dest_, destsiz_, src_) \
    strcat(dest_, src_)

#define SNPRINTF_S(buf_, bufsiz_, format_, ...) \
    snprintf(buf_, bufsiz_, format_, ##__VA_ARGS__)

#define PRINTF_S(format_, ...) \
    printf(format_, ##__VA_ARGS__)

#define FPRINTF_S(fp_, format_, ...) \
    fprintf(fp_, format_, ##__VA_ARGS__)

#define FREAD_S(buf_, bufsiz_, elsiz_, count_, fp_) \
    fread(buf_, elsiz_, count_, fp_)

#define FOPEN_S(fp_, fName_, mode_) \
    (fp_ = fopen(fName_, mode_))

#define LOCALTIME_S(tm_, time_) \
    memcpy(tm_, localtime(time_), sizeof(struct tm))

#endif /* SAFE_STD_H */
