#include "day_8.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define GROUP_SIZE 1000

static void update_state_1(char c, long* circuits)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static long grid[1000][3] = {0};
    static size_t line_index = 0U;
    static size_t coord_index = 0U;
    static long index_groups[GROUP_SIZE] = {0};
    static size_t index_groups_sizes[GROUP_SIZE] = {0};
    static long shortest_paths[GROUP_SIZE][2] = {0};

    if(c == '\0')
    {
        for(size_t i = 0U; i < GROUP_SIZE; i++)
        {
            index_groups[i] = -1;
        }

        grid[line_index][coord_index] = string_to_num(buffer);
        line_index++;

        size_t group_index = 0U;
        long prev_dist = 0;
        long closes_dist = __LONG_MAX__;
        long connections = 0;
        long prev_first_index = 0;
        long prev_second_index = 0;
        for(size_t iterations = 0U; iterations < 1000; iterations++)
        {
            closes_dist = __LONG_MAX__;
            size_t first_index = 0U;
            size_t second_index = 0U;
            long group_1 = -1;
            long group_2 = -1;
            long curr_group_1 = -1;
            long curr_group_2 = -1;
            for(size_t i = 0U; i < line_index; i++)
            {
                for(size_t j = 0U; j < line_index; j++)
                {
                    bool indexes_equal = ((prev_first_index == i) && (prev_second_index == j)) || ((prev_first_index == j) && (prev_second_index == i));
                    if((i != j) && !indexes_equal)
                    {
                        curr_group_1 = index_groups[i];
                        curr_group_2 = index_groups[j];
                        long x = grid[i][0] - grid[j][0];
                        long y = grid[i][1] - grid[j][1];
                        long z = grid[i][2] - grid[j][2];
                        long dist = (x * x) + (y * y) + (z * z);
                        bool indexes_equal = ((prev_first_index == i) && (prev_second_index == j)) || ((prev_first_index == j) && (prev_second_index == i));
                        // && ((curr_group_1 == -1) || (curr_group_1 != curr_group_2)))
                        if((dist < closes_dist) && (dist >= prev_dist) && (!indexes_equal))
                        {
                            group_1 = curr_group_1;
                            group_2 = curr_group_2;
                            closes_dist = dist;
                            first_index = i;
                            second_index = j;
                        }
                    }
                }
            }
            shortest_paths[iterations][0] = first_index;
            shortest_paths[iterations][1] = second_index;
            prev_dist = closes_dist;
            prev_first_index = first_index;
            prev_second_index = second_index;

            if(group_1 == group_2)
            {
                if(group_1 == -1)
                {
                    index_groups[first_index] = group_index;
                    index_groups[second_index] = group_index;
                    index_groups_sizes[group_index] = 2U;
                    group_index++;
                }
            }
            else
            {
                if(group_1 == -1)
                {
                    index_groups[first_index] = group_2;
                    index_groups_sizes[group_2]++;
                    group_2 = -1;
                }
                else if(group_2 == -1)
                {
                    index_groups[second_index] = group_1;
                    index_groups_sizes[group_1]++;
                    group_1 = -1;
                }
                else
                {
                    long first_group = group_1;
                    long second_group = group_2;
                    if(index_groups_sizes[group_2] > index_groups_sizes[group_1])
                    {
                        first_group = group_2;
                        second_group = group_1;
                    }

                    index_groups[second_index] = first_group;
                    index_groups_sizes[first_group] += index_groups_sizes[second_group];
                    index_groups_sizes[second_group] = 0U;
                    group_2 = first_group;
                    
                    for(size_t index = 0U; index < line_index; index++)
                    {
                        if(index_groups[index] == second_group)
                        {
                            index_groups[index] = first_group;
                        }
                    }
                }
            }
        }

        (*circuits) = 1;
        for(size_t i = 0U; i < 3U; i++)
        {
            size_t largest_group = 0U;
            size_t largest_group_index = 0U;
            for(size_t i = 0U; i < group_index; i++)
            {
                if(index_groups_sizes[i] > largest_group)
                {
                    largest_group = index_groups_sizes[i];
                    largest_group_index = i;
                }
            }
            (*circuits) *= largest_group;
            index_groups_sizes[largest_group_index] = 0U;
        }
    }
    else if(c == '\n')
    {
        grid[line_index][coord_index] = string_to_num(buffer);
        coord_index = 0U;
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0;
        line_index++;
    }
    else if(c == ',')
    {
        grid[line_index][coord_index] = string_to_num(buffer);
        coord_index++;
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0;
    }
    else if((c >= '0') && (c <= '9'))
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void update_state_2(char c, long* circuits)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static long grid[1000][3] = {0};
    static size_t line_index = 0U;
    static size_t coord_index = 0U;
    static long index_groups[GROUP_SIZE] = {0};
    static size_t index_groups_sizes[GROUP_SIZE] = {0};
    static long shortest_paths[GROUP_SIZE][2] = {0};

    if(c == '\0')
    {
        for(size_t i = 0U; i < GROUP_SIZE; i++)
        {
            index_groups[i] = -1;
        }

        grid[line_index][coord_index] = string_to_num(buffer);
        line_index++;

        size_t group_index = 0U;
        long prev_dist = 0;
        long closes_dist = __LONG_MAX__;
        long connections = 0;
        long prev_first_index = 0;
        long prev_second_index = 0;
        for(size_t iterations = 0U; iterations < 1400; iterations++)
        {
            closes_dist = __LONG_MAX__;
            size_t first_index = 0U;
            size_t second_index = 0U;
            long group_1 = -1;
            long group_2 = -1;
            long curr_group_1 = -1;
            long curr_group_2 = -1;
            for(size_t i = 0U; i < line_index; i++)
            {
                for(size_t j = 0U; j < line_index; j++)
                {
                    bool indexes_equal = ((prev_first_index == i) && (prev_second_index == j)) || ((prev_first_index == j) && (prev_second_index == i));
                    if((i != j) && !indexes_equal)
                    {
                        curr_group_1 = index_groups[i];
                        curr_group_2 = index_groups[j];
                        long x = grid[i][0] - grid[j][0];
                        long y = grid[i][1] - grid[j][1];
                        long z = grid[i][2] - grid[j][2];
                        long dist = (x * x) + (y * y) + (z * z);
                        bool indexes_equal = ((prev_first_index == i) && (prev_second_index == j)) || ((prev_first_index == j) && (prev_second_index == i));
                        // && ((curr_group_1 == -1) || (curr_group_1 != curr_group_2)))
                        if((dist < closes_dist) && (dist >= prev_dist) && (!indexes_equal)&& ((curr_group_1 == -1) || (curr_group_1 != curr_group_2)))
                        {
                            group_1 = curr_group_1;
                            group_2 = curr_group_2;
                            closes_dist = dist;
                            first_index = i;
                            second_index = j;
                        }
                    }
                }
            }
            shortest_paths[iterations][0] = first_index;
            shortest_paths[iterations][1] = second_index;
            prev_dist = closes_dist;
            prev_first_index = first_index;
            prev_second_index = second_index;

            if(group_1 == group_2)
            {
                if(group_1 == -1)
                {
                    index_groups[first_index] = group_index;
                    index_groups[second_index] = group_index;
                    index_groups_sizes[group_index] = 2U;
                    group_index++;
                }
            }
            else
            {
                if(group_1 == -1)
                {
                    index_groups[first_index] = group_2;
                    index_groups_sizes[group_2]++;
                    group_2 = -1;
                }
                else if(group_2 == -1)
                {
                    index_groups[second_index] = group_1;
                    index_groups_sizes[group_1]++;
                    group_1 = -1;
                }
                else
                {
                    long first_group = group_1;
                    long second_group = group_2;
                    if(index_groups_sizes[group_2] > index_groups_sizes[group_1])
                    {
                        first_group = group_2;
                        second_group = group_1;
                    }

                    index_groups[second_index] = first_group;
                    index_groups_sizes[first_group] += index_groups_sizes[second_group];
                    index_groups_sizes[second_group] = 0U;
                    group_2 = first_group;
                    
                    for(size_t index = 0U; index < line_index; index++)
                    {
                        if(index_groups[index] == second_group)
                        {
                            index_groups[index] = first_group;
                        }
                    }
                }
            }

            for(size_t i = 0U; i < group_index; i++)
            {
                if(index_groups_sizes[i] == 1000)
                {
                    (*circuits) = grid[first_index][0] * grid[second_index][0];
                    return;
                }
            }
        }

        (*circuits) = 1;
        for(size_t i = 0U; i < 3U; i++)
        {
            size_t largest_group = 0U;
            size_t largest_group_index = 0U;
            for(size_t i = 0U; i < group_index; i++)
            {
                if(index_groups_sizes[i] > largest_group)
                {
                    largest_group = index_groups_sizes[i];
                    largest_group_index = i;
                }
            }
            (*circuits) *= largest_group;
            index_groups_sizes[largest_group_index] = 0U;
        }
    }
    else if(c == '\n')
    {
        grid[line_index][coord_index] = string_to_num(buffer);
        coord_index = 0U;
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0;
        line_index++;
    }
    else if(c == ',')
    {
        grid[line_index][coord_index] = string_to_num(buffer);
        coord_index++;
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0;
    }
    else if((c >= '0') && (c <= '9'))
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void find_circuits_1(const char* filename)
{
    long circuits = 0;
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
            update_state_1(buffer[i], &circuits);
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
    printf("%li\n", circuits);
}

static void find_circuits_2(const char* filename)
{
    long circuits = 0;
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
            update_state_2(buffer[i], &circuits);
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
    printf("%li\n", circuits);
}

void solve_day_8(const char* filename, int part)
{
    if(part == 1)
    {
        find_circuits_1(filename);
    }
    else if(part == 2)
    {
        find_circuits_2(filename);
    }
    else
    {
        find_circuits_1(filename);
        find_circuits_2(filename);
    }
}