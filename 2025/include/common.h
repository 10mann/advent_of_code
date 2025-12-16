#ifndef COMMON_H_
#define COMMON_H_

#include <stddef.h>

long string_to_num(const char* string);
long string_to_num_n(const char* string, size_t length);
size_t num_to_string(char* string, size_t length, long num);
double get_time(void);
size_t read_file(const char* filename, char* buffer, size_t buffer_length);

#endif