#include "day_1.h"
#include <stdint.h>
#include <stdio.h>

static void update_state_1(char c, int* dial, int* password)
{
    static int sign = 1;
    static int num = 0;
    if(c == 'L')
    {
        sign = -1;
        num = 0;
    }
    else if(c == 'R')
    {
        sign = 1;
        num = 0;
    }
    else if((c >= '0') && (c <= '9'))
    {
        num *= 10;
        num += (c - '0');
    }
    else if(c == '\n')
    {
        int rest = num % 100;
        if(sign > 0)
        {
            *dial += rest;
        }
        else
        {
            *dial -= rest;
            if(*dial < 0)
            {
                *dial += 100;
            }
        }

        *dial %= 100;
        if(*dial == 0)
        {
            (*password)++;
        }
#ifdef DEBUG_LOG
        printf("Dial: %i, num: %i, pass: %i\n", *dial, num, *password);
#endif
    }
}

static void update_state_2(char c, int* dial, int* password)
{
    static int sign = 1;
    static int num = 0;
    if(c == 'L')
    {
        sign = -1;
        num = 0;
    }
    else if(c == 'R')
    {
        sign = 1;
        num = 0;
    }
    else if((c >= '0') && (c <= '9'))
    {
        num *= 10;
        num += (c - '0');
    }
    else if(c == '\n')
    {
        int turns = num / 100;
        (*password) += turns;
        int rest = num % 100;
        if(sign > 0)
        {
            if((*dial > 0) && ((rest + *dial) >= 100))
            {
                (*password)++;
            }

            *dial += rest;
        }
        else
        {
            if((*dial != 0) && (rest >= *dial))
            {
                (*password)++;
            }

            *dial -= rest;
            if(*dial < 0)
            {
                *dial += 100;
            }
        }

        *dial %= 100;
#ifdef DEBUG_LOG
        printf("Dial: %i, num: %i, pass: %i\n", *dial, num, *password);
#endif
    }
}

static void calculate_password_day_1(const char* filename)
{
    int password = 0;
    int dial = 50;
    FILE* input_text = fopen(filename, "r");
    
    char input[8 * 1024] = {0};
    size_t tot_read = 0U;
    while(input_text != NULL)
    {
        size_t length = fread(input, sizeof(input[0]), sizeof(input), input_text);
        tot_read += length;
        if(length < sizeof(input))
        {
            input[length] = '\n';
            length++;
        }

        for(size_t i = 0U; i < length; i++)
        {
            update_state_1(input[i], &dial, &password);
        }

        if(length < sizeof(input))
        {
            break;
        }
    }
#ifdef DEBUG_LOG
    printf("Tot read: %lu\n", tot_read);
#endif
    fclose(input_text);
    printf("%i\n", password);
}

static void calculate_password_day_2(const char* filename)
{
    int password = 0;
    int dial = 50;
    FILE* input_text = fopen(filename, "r");
    char input[8 * 1024] = {0};
    size_t tot_read = 0U;
    while(input_text != NULL)
    {
        size_t length = fread(input, sizeof(input[0]), sizeof(input), input_text);
        tot_read += length;
        if(length < sizeof(input))
        {
            input[length] = '\n';
            length++;
        }

        for(size_t i = 0U; i < length; i++)
        {
            update_state_2(input[i], &dial, &password);
        }

        if(length < sizeof(input))
        {
            break;
        }
    }
#ifdef DEBUG_LOG
    printf("Tot read: %lu\n", tot_read);
#endif
    fclose(input_text);
    printf("%i\n", password);
}

void solve_day_1(const char* filename, int part)
{
    if(part == 1)
    {
        calculate_password_day_1(filename);
    }
    else if(part == 2)
    {
        calculate_password_day_2(filename);
    }
    else
    {
        calculate_password_day_1(filename);
        calculate_password_day_2(filename);
    }
}