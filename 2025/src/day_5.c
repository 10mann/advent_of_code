#include "day_5.h"
#include "common.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static void update_state_1(char c, long* fresh_ingredients)
{
    static char number_buffer[256] = {0};
    static long ranges[2][256] = {0};
    static size_t index = 0U;
    static size_t range_index = 0U;
    static char prev_char = 0;
    static bool range_list_done = false;

    if(c == '-')
    {
        ranges[0][range_index] = string_to_num(number_buffer);
        memset(number_buffer, 0, sizeof(number_buffer));
        index = 0;
    }
    else if((c == '\n') || (c == '\0'))
    {
        index = 0;
        if((c == '\n') && (prev_char == '\n'))
        {
            range_list_done = true;
        }
        else if(!range_list_done)
        {
            ranges[1][range_index] = string_to_num(number_buffer);
            range_index++;
        }
        else
        {
            long ingredient = string_to_num(number_buffer);

            for(size_t i = 0U; i < range_index; i++)
            {
                if((ingredient >= ranges[0][i]) && (ingredient <= ranges[1][i]))
                {
                    (*fresh_ingredients)++;
                    break;
                }
            }
        }
        
        memset(number_buffer, 0, sizeof(number_buffer));
    }
    else if((c >= '0') && (0 <= '9'))
    {
        number_buffer[index] = c;
        index++;
    }

    prev_char = c;
}

static bool update_state_2(char c, long* fresh_ingredients)
{
    static char number_buffer[256] = {0};
    static long ranges[2][256] = {0};
    static size_t index = 0U;
    static size_t range_index = 0U;
    static char prev_char = 0;

    if(c == '-')
    {
        ranges[0][range_index] = string_to_num(number_buffer);
        memset(number_buffer, 0, sizeof(number_buffer));
        index = 0;
        for(size_t i = 0U; i < range_index; i++)
        {
            if((ranges[0][range_index] >= ranges[0][i]) && (ranges[0][range_index] <= ranges[1][i]))
            {
                ranges[0][range_index] = ranges[1][i] + 1U;
                break;
            }
        }
    }
    else if((c == '\n') || (c == '\0'))
    {
        index = 0;
        ranges[1][range_index] = string_to_num(number_buffer);
        for(size_t i = 0U; i < range_index; i++)
        {
            if((ranges[1][range_index] >= ranges[0][i]) && (ranges[1][range_index] <= ranges[1][i]))
            {
                ranges[1][range_index] = ranges[0][i] - 1U;
                if(ranges[1][range_index] < ranges[0][range_index])
                {
                    ranges[0][range_index] = 1;
                    ranges[1][range_index] = 0;
                }
                break;
            }
        }

        for(size_t i = 1U; i <= range_index; i++)
        {
            if((ranges[0][range_index - i] >= ranges[0][range_index]) && (ranges[1][range_index - i] <= ranges[1][range_index]))
            {
                ranges[0][range_index - i] = 1;
                ranges[1][range_index - i] = 0;
            }
        }

        if((c == '\n') && (prev_char == '\n'))
        {
            for(size_t i = 0U; i <= range_index; i++)
            {
                for(size_t j = 0U; j <= range_index; j++)
                {
                    if((i != j) && (ranges[0][i] >= ranges[0][j]) && (ranges[1][i] <= ranges[1][j]))
                    {
                        ranges[0][i] = 1;
                        ranges[1][i] = 0;
                    }
                }
            }

            for(size_t i = 0U; i < range_index; i++)
            {
                (*fresh_ingredients) += (ranges[1][i] + 1 - ranges[0][i]);
            }

            return true;
        }
        range_index++;
        memset(number_buffer, 0, sizeof(number_buffer));
    }
    else if((c >= '0') && (0 <= '9'))
    {
        number_buffer[index] = c;
        index++;
    }

    prev_char = c;
    return false;
}

static void find_fresh_ingredients_1(const char* buffer, size_t length)
{
    long fresh_ingredients = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &fresh_ingredients);
    }

    printf("%li\n", fresh_ingredients);
}

static void find_fresh_ingredients_2(const char* buffer, size_t length)
{
    long fresh_ingredients = 0;

    for(size_t i = 0U; i < length; i++)
    {
        if(update_state_2(buffer[i], &fresh_ingredients))
        {
            length = 0U;
            break;
        }
    }

    printf("%li\n", fresh_ingredients);
}

void solve_day_5(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        find_fresh_ingredients_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        find_fresh_ingredients_2(buffer, buffer_length);
    }
    else
    {
        find_fresh_ingredients_1(buffer, buffer_length);
        find_fresh_ingredients_2(buffer, buffer_length);
    }
}