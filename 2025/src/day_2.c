#include "day_2.h"
#include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

static bool is_valid_id_1(long id)
{
    bool valid = true;
    char string[64] = {0};
    size_t length = num_to_string(string, sizeof(string), id);
    
    return ((length % 2U) != 0U) || memcmp(string, &string[length / 2U], length / 2U) != 0;
}

static bool is_valid_id_2(long id)
{
    bool valid = true;
    char string[64] = {0};
    size_t length = num_to_string(string, sizeof(string), id);
    
    for(size_t i = 1U; i <= (length / 2); i++)
    {
        if((length % i) != 0)
        {
            continue;
        }

        size_t parts = length / i;
        bool match = true;
        for(size_t j = 0; j < (parts - 1U); j++)
        {
            if(memcmp(&string[j * i], &string[(j + 1U) * i], i) != 0)
            {
                match = false;
                break;
            }
        }

        if(match)
        {
            valid = false;
            break;
        }
    }

    return valid;
}

static void update_state_1(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        num_2 = string_to_num(numbers[1]);
        buffer = 0;
        index = 0;
#if DEBUG_LOG == 1
        printf("Number1: %s, number2: %s\n", numbers[0], numbers[1]);
        printf("Num1: %li, num2: %li\n", num_1, num_2);
#endif
        memset(numbers, 0, sizeof(numbers));

        for(long i = num_1; i <= num_2; i++)
        {
            if(!is_valid_id_1(i))
            {
#if DEBUG_LOG == 1
                printf("invalid: %li\n", i);
#endif
                (*invalid_ids) += i;
            }
        }
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void update_state_2(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        num_2 = string_to_num(numbers[1]);
        buffer = 0;
        index = 0;
#if DEBUG_LOG == 1
        printf("Number1: %s, number2: %s\n", numbers[0], numbers[1]);
        printf("Num1: %li, num2: %li\n", num_1, num_2);
#endif
        memset(numbers, 0, sizeof(numbers));
        for(long i = num_1; i <= num_2; i++)
        {
            if(!is_valid_id_2(i))
            {
#if DEBUG_LOG == 1
                printf("invalid: %li\n", i);
#endif
                (*invalid_ids) += i;
            }
        }
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void find_valid_ids_1(const char* filename)
{
    long invalid_ids = 0;
    FILE* input_text = fopen(filename, "r");
    while(input_text != NULL)
    {
        char buffer[8 * 1024] = {0};
        size_t length = fread(buffer, sizeof(buffer[0]), sizeof(buffer), input_text);
        if(length < sizeof(buffer))
        {
            buffer[length] = '\0';
            length++;
        }
        for(size_t i = 0U; i < length; i++)
        {
            update_state_1(buffer[i], &invalid_ids);
        }

        if(length < sizeof(buffer))
        {
            break;
        }
    }

    if(input_text == NULL)
    {
        printf("Unable to open file %s\n", filename);
    }

    printf("%li\n", invalid_ids);
}

static void find_valid_ids_2(const char* filename)
{
    long invalid_ids = 0;
    FILE* input_text = fopen(filename, "r");
    while(input_text != NULL)
    {
        char buffer[8 * 1024] = {0};
        size_t length = fread(buffer, sizeof(buffer[0]), sizeof(buffer), input_text);
        if(length < sizeof(buffer))
        {
            buffer[length] = '\0';
            length++;
        }
        for(size_t i = 0U; i < length; i++)
        {
            update_state_2(buffer[i], &invalid_ids);
        }

        if(length < sizeof(buffer))
        {
            break;
        }
    }

    if(input_text == NULL)
    {
        printf("Unable to open file %s\n", filename);
    }

    printf("%li\n", invalid_ids);
}

void solve_day_2(const char* filename, int part)
{
    if(part == 1)
    {
        find_valid_ids_1(filename);
    }
    else if(part == 2)
    {
        find_valid_ids_2(filename);   
    }
    else
    {
        find_valid_ids_1(filename);
        find_valid_ids_2(filename);
    }
}