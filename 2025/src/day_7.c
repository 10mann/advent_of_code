#include "day_7.h"
#include "common.h"

#include <stdio.h>

static void update_state_1(char c, long* splits)
{
    static char buffer[256][256];
    static size_t index = 0U;
    static size_t line_index = 0U;
    static size_t origin = 0U;
    
    if(c == '\n')
    {
        index = 0U;
        line_index++;
    }
    else if(c == '\0')
    {
        for(size_t i = 1U; i < line_index; i++)
        {
            for(size_t j = 0U; j < index; j++)
            {
                if((buffer[i - 1U][j] == 'S'))
                {
                    if(buffer[i][j] == '^')
                    {
                        (*splits)++;
                        if(j > 0)
                        {
                            buffer[i][j - 1U] = 'S';
                        }

                        if(j < (index - 1U))
                        {
                            buffer[i][j + 1U] = 'S';
                        }
                    }
                    else
                    {
                        buffer[i][j] = 'S';
                    }
                }
            }
        }
    }
    else if(c == 'S')
    {
        origin = index;
        buffer[line_index][index] = c;
        index++;
    }
    else
    {
        buffer[line_index][index] = c;
        index++;
    }
}

static void calculate_splits_1(const char* buffer, size_t length)
{
    long splits = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &splits);
    }

    printf("%li\n", splits);
}

void solve_day_7(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        calculate_splits_1(buffer, buffer_length);
    }
    else if(part == 2)
    {

    }
    else
    {

    }
}