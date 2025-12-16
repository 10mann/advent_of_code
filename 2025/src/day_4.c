#include "day_4.h"
#include "common.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static long find_accessible_first_or_last_1(const char* row_1, const char* row_2, size_t length)
{
    long num_accessible = 0;
    for(size_t i = 0U; i < length; i++)
    {
        long count = 0;
        if(row_1[i] == '@')
        {
            if((i > 0) && (row_1[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_1[i + 1U] == '@'))
            {
                count++;
            }
            if(row_2[i] == '@')
            {
                count++;
            }
            if((i > 0) && (row_2[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_2[i + 1U] == '@'))
            {
                count++;
            }
            if(count < 4)
            {
                num_accessible++;
            }
        }
    }

    return num_accessible;
}

static long find_accessible_first_or_last_2(char* row_1, const char* row_2, size_t length)
{
    long num_accessible = 0;
    for(size_t i = 0U; i < length; i++)
    {
        long count = 0;
        if(row_1[i] == '@')
        {
            if((i > 0) && (row_1[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_1[i + 1U] == '@'))
            {
                count++;
            }
            if(row_2[i] == '@')
            {
                count++;
            }
            if((i > 0) && (row_2[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_2[i + 1U] == '@'))
            {
                count++;
            }
            if(count < 4)
            {
                num_accessible++;
                row_1[i] = '.';
            }
        }
    }

    return num_accessible;
}

static long find_accessible_1(const char* row_1, const char* row_2, const char* row_3, size_t length)
{
    long num_accessible = 0;
    for(size_t i = 0U; i < length; i++)
    {
        long count = 0;
        if(row_2[i] == '@')
        {
            if(row_1[i] == '@')
            {
                count++;
            }
            if((i > 0) && (row_1[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_1[i + 1U] == '@'))
            {
                count++;
            }
            if((i > 0) && (row_2[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_2[i + 1U] == '@'))
            {
                count++;
            }
            if(row_3[i] == '@')
            {
                count++;
            }
            if((i > 0) && (row_3[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_3[i + 1U] == '@'))
            {
                count++;
            }
            if(count < 4)
            {
                num_accessible++;
            }
        }
    }

    return num_accessible;
}

static long find_accessible_2(const char* row_1, char* row_2, const char* row_3, size_t length)
{
    long num_accessible = 0;
    for(size_t i = 0U; i < length; i++)
    {
        long count = 0;
        if(row_2[i] == '@')
        {
            if(row_1[i] == '@')
            {
                count++;
            }
            if((i > 0) && (row_1[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_1[i + 1U] == '@'))
            {
                count++;
            }
            if((i > 0) && (row_2[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_2[i + 1U] == '@'))
            {
                count++;
            }
            if(row_3[i] == '@')
            {
                count++;
            }
            if((i > 0) && (row_3[i - 1] == '@'))
            {
                count++;
            }
            if((i < (length - 1U)) && (row_3[i + 1U] == '@'))
            {
                count++;
            }
            if(count < 4)
            {
                num_accessible++;
                row_2[i] = '.';
            }
        }
    }

    return num_accessible;
}

static void update_state_1(char c, long* accessible_rolls)
{
    static size_t index = 0U;
    static size_t row_index = 0U;
    static char row_1[256]= {0};
    static char row_2[256]= {0};
    static char row_3[256]= {0};

    if((c == '\n') || (c == '\0'))
    {
        if(row_index == 1U)
        {
            (*accessible_rolls) += find_accessible_first_or_last_1(row_2, row_3, index);
        }

        if(row_index > 1U)
        {
            (*accessible_rolls) += find_accessible_1(row_1, row_2, row_3, index);
        }

        if(c == '\0')
        {
            (*accessible_rolls) += find_accessible_first_or_last_1(row_3, row_2, index);
        }

        row_index++;
        index = 0U;
        memcpy(row_1, row_2, sizeof(row_1));
        memcpy(row_2, row_3, sizeof(row_1));
        memset(row_3, 0, sizeof(row_3));
    }
    else
    {
        row_3[index] = c;
        index++;
    }
}

static void update_state_2(char c, long* accessible_rolls)
{
    static size_t row = 0U;
    static size_t column = 0U;
    static char grid[256][256] = {0};

    if(c == '\n')
    {
        row++;
        column = 0U;
    }
    else if(c == '\0')
    {
        long accessible = -1;
        size_t index = 0U;
        while((accessible != 0))
        {
            accessible = find_accessible_first_or_last_2(grid[0], grid[1], column);
            for(size_t i = 1U; i < row; i++)
            {
                accessible += find_accessible_2(grid[i - 1U], grid[i], grid[i + 1U], column);
            }
            accessible += find_accessible_first_or_last_2(grid[row], grid[row - 1], column);
            (*accessible_rolls) += accessible;
            index++;
        }
    }
    else
    {
        grid[row][column] = c;
        column++;
    }
}

static void find_accessible_rolls_1(const char* buffer, size_t length)
{
    long accessible_rolls = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &accessible_rolls);
    }

    printf("%li\n", accessible_rolls);
}

static void find_accessible_rolls_2(const char* buffer, size_t length)
{
    long accessible_rolls = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &accessible_rolls);
    }

    printf("%li\n", accessible_rolls);
}

void solve_day_4(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        find_accessible_rolls_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        find_accessible_rolls_2(buffer, buffer_length);
    }
    else
    {
        find_accessible_rolls_1(buffer, buffer_length);
        find_accessible_rolls_2(buffer, buffer_length);
    }
}