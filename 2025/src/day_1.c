#include "day_1.h"
#include "common.h"

#include <stdbool.h>
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
        num = 0;
    }
}

static void calculate_password_day_1(const char* buffer, size_t length)
{
    int password = 0;
    int dial = 50;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &dial, &password);
    }

    printf("%i\n", password);
}

static void calculate_password_day_2(const char* buffer, size_t length)
{
    int password = 0;
    int dial = 50;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &dial, &password);
    }

    printf("%i\n", password);
}

void solve_day_1(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));
    if(dryrun)
    {
        
    }
    else if(part == 1)
    {
        calculate_password_day_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        calculate_password_day_2(buffer, buffer_length);
    }
    else
    {
        calculate_password_day_1(buffer, buffer_length);
        calculate_password_day_2(buffer, buffer_length);
    }
}