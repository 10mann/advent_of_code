#ifndef COMMON_H_
#define COMMON_H_

#include <stddef.h>

long string_to_num(const char* string);
size_t num_to_string(char* string, size_t length, long num);
double get_time(void);

#endif