#include "day_7.h"
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

static void calculate_splits_1(const char* filename)
{
    long splits = 0;
    FILE* input_text = fopen(filename, "r");
    while(input_text != NULL)
    {
        char buffer[8 * 1024] = {0};
        size_t length = fread(buffer, sizeof(buffer[0]), sizeof(buffer), input_text);
        if(length < sizeof(buffer))
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
            update_state_1(buffer[i], &splits);
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

    fclose(input_text);
    printf("%li\n", splits);
}

void solve_day_7(const char* filename, int part)
{
    if(part == 1)
    {
        calculate_splits_1(filename);
    }
    else if(part == 2)
    {

    }
    else
    {

    }
}