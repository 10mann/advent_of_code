#include "common.h"
#include <sys/time.h>
#include <stddef.h>

long int string_to_num(const char* string)
{
    long int num = 0;
    for(int i = 0; (string[i] >= '0') && (string[i] <= '9'); i++)
    {
        num *= 10;
        num += string[i] - '0';
    }
    
    return num;
}

double get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}