#include "day_9.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

static void update_state_1(char c, long* biggest_area)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static size_t line_index = 0U;
    static long grid[512][2] = {0};
    static size_t grid_index = 0U;

    if(c == '\0')
    {
        grid[line_index][grid_index] = string_to_num(buffer);
        line_index++;

        long largest_area = 0;
        long area = 0;
        for(size_t i = 0U; i < line_index; i++)
        {
            for(size_t j = i + 1U; j < line_index; j++)
            {
                long delta_x = grid[i][0] - grid[j][0];
                long delta_y = grid[i][1] - grid[j][1];

                if(delta_x < 0)
                {
                    delta_x = grid[j][0] - grid[i][0];
                }

                if(delta_y < 0)
                {
                    delta_y = grid[j][1] - grid[i][1];
                }
                
                area = (delta_x + 1) * (delta_y + 1);
                if(area > largest_area)
                {
                    largest_area = area;
                }
            }
        }

        (*biggest_area) = largest_area;
    }
    else if(c == '\n')
    {
        grid[line_index][grid_index] = string_to_num(buffer);
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0U;
        line_index++;
        grid_index = 0U;
    }
    else if(c == ',')
    {
        grid[line_index][grid_index] = string_to_num(buffer);
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0U;
        grid_index++;
    }
    else if((c >= '0') && (c <= '9'))
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void find_biggest_area_1(const char* filename)
{
    long biggest_area = 0;
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
            update_state_1(buffer[i], &biggest_area);
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
    printf("%li\n", biggest_area);
}

void solve_day_9(const char* filename, int part)
{
    if(part == 1)
    {
        find_biggest_area_1(filename);
    }
}