#include "day_9.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

static void update_state_2(char c, long* biggest_area)
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
        size_t index_1 = 0;
        size_t index_2 = 0;
        long valid_squares = 0;
        grid_index = 0U;
        for(size_t i = 0U; i < line_index; i++)
        {
            for(size_t j = i + 1U; j < line_index; j++)
            {
                long x1 = grid[i][0];
                long x2 = grid[j][0];
                long y1 = grid[i][1];
                long y2 = grid[j][1];

                bool valid = true;
                if((x1 >= x2) && (y2 >= y1))
                {
                    if((grid[i + 1][1] > grid[i][1]) && ((grid[(i - 1) % line_index][0] > x1)) && (x1 > x2))
                    {
                        valid = false;
                    }
                    if((grid[(i - 1) % line_index][1] > grid[i][1]) && (grid[i + 1][0] > x1) && (x1 > x2))
                    {
                        valid = false;
                    }
                    for(size_t index = 0U; valid && (index < line_index); index++)
                    {
                        grid_index = (i + index) % line_index;
                        size_t next_grid_index = (i + index + 1U) % line_index;
                        bool invalid = ((grid[grid_index][0] > x2) && (grid[grid_index][0] < x1) && (grid[grid_index][1] <= y1) && (grid[next_grid_index][1] > y1));
                        invalid = invalid || ((grid[next_grid_index][0] > x2) && (grid[next_grid_index][0] < x1) && (grid[next_grid_index][1] <= y1) && (grid[grid_index][1] > y1));
                        invalid = invalid || ((grid[grid_index][1] > y1) && (grid[grid_index][1] < y2) && (grid[grid_index][0] <= x2) && (grid[next_grid_index][0] > x2));
                        invalid = invalid || ((grid[next_grid_index][1] > y1) && (grid[next_grid_index][1] < y2) && (grid[next_grid_index][0] <= x2) && (grid[grid_index][0] > x2));
                        
                        if(invalid)
                        {
                            valid = false;
                            break;
                        }
                    }
                }
                else if((x1 >= x2) && (y1 >= y2))
                {
                    if((grid[i + 1][1] < grid[i][1]) && (grid[(i - 1) % line_index][0] > x1) && (x1 > x2))
                    {
                        valid = false;
                    }
                    if((grid[(i - 1) % line_index][1] < grid[i][1]) && (grid[i + 1][0] > x1) && (x1 > x2))
                    {
                        valid = false;
                    }
                    for(size_t index = 0U; valid && (index <= line_index); index++)
                    {
                        grid_index = (i + index) % line_index;
                        size_t next_grid_index = (i + index + 1U) % line_index;
                        bool invalid = ((grid[grid_index][0] > x2) && (grid[grid_index][0] < x1) && (grid[grid_index][1] >= y1) && (grid[next_grid_index][1] < y1));
                        invalid = invalid || ((grid[grid_index][1] > y2) && (grid[grid_index][1] < y1) && (grid[grid_index][0] <= x2) && (grid[next_grid_index][0] > x2));
                        invalid = invalid || ((grid[next_grid_index][1] > y2) && (grid[next_grid_index][1] < y1) && (grid[next_grid_index][0] <= x2) && (grid[grid_index][0] > x2));
                        invalid = invalid || ((grid[next_grid_index][0] > x2) && (grid[next_grid_index][0] < x1) && (grid[next_grid_index][1] >= y1) && (grid[grid_index][1] < y1));

                        if(invalid)
                        {
                            valid = false;
                            break;
                        }
                    }
                }
                else if((x2 >= x1) && (y2 >= y1))
                {
                    if((grid[i + 1][0] > grid[i][0]) && (grid[(i - 1) % line_index][1] < y1) && (y1 < y2))
                    {
                        valid = false;
                    }
                    if((grid[(i - 1) % line_index][0] > grid[i][0]) && (grid[i + 1][1] < y1) && (y1 < y2))
                    {
                        valid = false;
                    }
                    for(size_t index = 0U; valid && (index < line_index); index++)
                    {
                        grid_index = (i + index) % line_index;
                        size_t next_grid_index = (i + index + 1U) % line_index;
                        bool invalid = ((grid[grid_index][1] > y1) && (grid[grid_index][1] < y2) && (grid[grid_index][0] <= x1) && (grid[next_grid_index][0] > x1));
                        invalid = invalid || ((grid[grid_index][0] > x1) && (grid[grid_index][0] < x2) && (grid[grid_index][1] >= y2) && (grid[next_grid_index][1] < y2));
                        invalid = invalid || ((grid[next_grid_index][1] > y1) && (grid[next_grid_index][1] < y2) && (grid[next_grid_index][0] <= x1) && (grid[grid_index][0] > x1));
                        invalid = invalid || ((grid[next_grid_index][0] > x1) && (grid[next_grid_index][0] < x2) && (grid[next_grid_index][1] >= y2) && (grid[grid_index][1] < y2));

                        if(invalid)
                        {
                            valid = false;
                            break;
                        }
                    }
                }
                else
                {
                    if((grid[i + 1][0] > grid[i][0]) && (grid[(i - 1) % line_index][1] > y1) && (y1 > y2))
                    {
                        valid = false;
                    }
                    if((grid[(i - 1) % line_index][0] > grid[i][0]) && (grid[i + 1][1] > y1) && (y1 > y2))
                    {
                        valid = false;
                    }
                    for(size_t index = 0U; index < line_index; index++)
                    {
                        grid_index = (i + index) % line_index;
                        size_t next_grid_index = (i + index + 1U) % line_index;
                        bool invalid = ((grid[grid_index][1] > y2) && (grid[grid_index][1] < y1) && (grid[grid_index][0] <= x1) && (grid[next_grid_index][0] > x1));
                        invalid = invalid || ((grid[grid_index][0] > x1) && (grid[grid_index][0] < x2) && (grid[grid_index][1] <= y2) && (grid[next_grid_index][1] > y2));
                        invalid = invalid || ((grid[next_grid_index][1] > y2) && (grid[next_grid_index][1] < y1) && (grid[next_grid_index][0] <= x1) && (grid[grid_index][0] > x1));
                        invalid = invalid || ((grid[next_grid_index][0] > x1) && (grid[next_grid_index][0] < x2) && (grid[next_grid_index][1] <= y2) && (grid[grid_index][1] > y2));

                        if(invalid)
                        {
                            valid = false;
                            break;
                        }
                    }
                }

                if(!valid)
                {
                    continue;
                }
                else
                {
                    valid_squares++;
                    // printf("Valid square (%li, %li) and (%li, %li)\n", grid[i][0], grid[i][1], grid[j][0], grid[j][1]);
                }

                if(x1 > x2)
                {
                    x1 = grid[j][0];
                    x2 = grid[i][0];
                }

                if(y1 > y2)
                {
                    y1 = grid[j][1];
                    y2 = grid[i][1];
                }
                area = (1 + x2 - x1) * (1 + y2 - y1);
                if(area > largest_area)
                {
                    largest_area = area;
                    index_1 = i;
                    index_2 = j;
                }
            }
        }
        // printf("Valid squares: %li\n", valid_squares);
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

static void find_biggest_area_1(const char* buffer, size_t length)
{
    long biggest_area = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &biggest_area);
    }

    printf("%li\n", biggest_area);
}

static void find_biggest_area_2(const char* buffer, size_t length)
{
    long biggest_area = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &biggest_area);
    }

    printf("%li\n", biggest_area);
}

void solve_day_9(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    if(part == 1)
    {
        find_biggest_area_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        // find_biggest_area_2(buffer, buffer_length);
    }
    else
    {
        find_biggest_area_1(buffer, buffer_length);
        // find_biggest_area_2(buffer, buffer_length);
    }
}