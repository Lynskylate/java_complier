//
// Created by lynskylate on 2018/6/26.
//

#ifndef JCC_UTILS_H
#define JCC_UTILS_H

#include <ctype.h>
#include <stdio.h>

size_t file_size(FILE *stream);

char *file_read(const char *filename);

char *read_until_eof(FILE *stream);

#endif //JCC_UTILS_H
