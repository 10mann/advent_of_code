#include "common.h"
#include <sys/time.h>
#include <stdio.h>

long string_to_num(const char* string)
{
    long num = 0;
    for(int i = 0; (string[i] >= '0') && (string[i] <= '9'); i++)
    {
        num *= 10;
        num += string[i] - '0';
    }
    
    return num;
}

long string_to_num_n(const char* string, size_t length)
{
    long num = 0;
    for(size_t i = 0U; i < length; i++)
    {
        num *= 10;
        num += string[i] - '0';
    }
    
    return num;
}

size_t num_to_string(char* string, size_t length, long num)
{
    size_t index = 0;
    long temp = num;
    while((temp > 0) && (index < length))
    {
        string[index] = (temp % 10) + '0';
        temp /= 10;
        index++;
    }

    for(size_t i = 0; i < (index / 2); i++)
    {
        char c = string[i];
        string[i] = string[index - i - 1];
        string[index - i - 1] = c;
    }

    return index;
}

double get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

size_t read_file(const char* filename, char* buffer, size_t buffer_length)
{
    size_t length = 0U;
    FILE* input_text = fopen(filename, "r");
    while(input_text != NULL)
    {
        length = fread(buffer, sizeof(buffer[0]), buffer_length, input_text);
        if(length < buffer_length)
        {
            if(buffer[length - 1U] == '\n')
            {
                buffer[length - 1U] = '\0';
            }
            else
            {
                buffer[length] = '\0';
                length++;
            }
        }

        for(size_t i = 0U; i < length; i++)
        {
            (void)buffer[i];
        }

        if(length < buffer_length)
        {
            break;
        }
    }

    if(input_text == NULL)
    {
        printf("Unable to open file %s\n", filename);
    }

    return length;
}
