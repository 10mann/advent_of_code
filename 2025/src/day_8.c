#include "day_8.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define GROUP_SIZE 1000
#define GROUP_SIZE_V2 6400

static void shuffle_array(long* arr, size_t length)
{
    for(size_t i = 1; i < length; i++)
    {
        arr[length - i] = arr[length - i - 1];
    }
}

static size_t get_prev_index(const long* arr, size_t length, long num)
{
    size_t start = 0U;
    size_t end = length;
    size_t mid = (end - start) / 2U;

    if(num > arr[length - 1U])
    {
        return length - 1U;
    }
    if(num < arr[0U])
    {
        return 0U;
    }

    while(mid > 0U)
    {
        if((arr[mid] >= num) && (arr[mid - 1U] < num))
        {
            return mid;
        }
        else if(arr[mid] < num)
        {
            start = mid;
            mid = (end + start) / 2U;
        }
        else 
        {
            end = mid;
            mid = (end + start) / 2U;
        }
    }

    return 0U;
}

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
    static long closest_dists[GROUP_SIZE] = {0};

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
                for(size_t j = i + 1U; j < line_index; j++)
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

static void update_state_1_v2(char c, long* circuits)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static long grid[1000][3] = {0};
    static size_t line_index = 0U;
    static size_t coord_index = 0U;
    static long index_groups[GROUP_SIZE] = {0};
    static size_t index_groups_sizes[GROUP_SIZE] = {0};
    static long closest_dists[GROUP_SIZE + 1U] = {0};
    static long closest_first_indexes[GROUP_SIZE + 1U] = {0};
    static long closest_second_indexes[GROUP_SIZE + 1U] = {0};
    static size_t closest_dists_index = 1U;

    if(c == '\0')
    {
        for(size_t i = 0U; i < GROUP_SIZE; i++)
        {
            index_groups[i] = -1;
        }

        grid[line_index][coord_index] = string_to_num(buffer);
        line_index++;
        size_t group_index = 0U;

        for(size_t i = 0U; i < GROUP_SIZE; i++)
        {
            closest_dists[i] = __LONG_MAX__;
        }

        for(size_t i = 0U; i < line_index; i++)
        {
            for(size_t j = i + 1U; j < line_index; j++)
            {
                long x = grid[i][0] - grid[j][0];
                long y = grid[i][1] - grid[j][1];
                long z = grid[i][2] - grid[j][2];
                long dist = (x * x) + (y * y) + (z * z);
                if(dist < closest_dists[closest_dists_index - 1U])
                {
                    size_t index = get_prev_index(closest_dists, closest_dists_index, dist);
                    if(closest_dists_index < GROUP_SIZE)
                    {
                        closest_dists_index++;
                    }
                    shuffle_array(&closest_dists[index], closest_dists_index - index);
                    shuffle_array(&closest_first_indexes[index], closest_dists_index - index);
                    shuffle_array(&closest_second_indexes[index], closest_dists_index - index);
                    closest_dists[index] = dist;
                    closest_first_indexes[index] = i;
                    closest_second_indexes[index] = j;
                }
            }
        }

        for(size_t iterations = 0U; iterations < 1000; iterations++)
        {
            size_t first_index = closest_first_indexes[iterations];
            size_t second_index = closest_second_indexes[iterations];
            long group_1 = index_groups[first_index];
            long group_2 = index_groups[second_index];

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
                for(size_t j = i + 1U; j < line_index; j++)
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

static void update_state_2_v2(char c, long* circuits)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static long grid[1000][3] = {0};
    static size_t line_index = 0U;
    static size_t coord_index = 0U;
    static long index_groups[GROUP_SIZE] = {0};
    static size_t index_groups_sizes[GROUP_SIZE] = {0};
    static long closest_dists[GROUP_SIZE_V2 + 1U] = {0};
    static long closest_first_indexes[GROUP_SIZE_V2 + 1U] = {0};
    static long closest_second_indexes[GROUP_SIZE_V2 + 1U] = {0};
    static size_t closest_dists_index = 1U;

    if(c == '\0')
    {
        for(size_t i = 0U; i < GROUP_SIZE; i++)
        {
            index_groups[i] = -1;
        }
        for(size_t i = 0U; i < GROUP_SIZE_V2; i++)
        {
            closest_dists[i] = __LONG_MAX__;
        }

        grid[line_index][coord_index] = string_to_num(buffer);
        line_index++;

        for(size_t i = 0U; i < line_index; i++)
        {
            for(size_t j = i + 1U; j < line_index; j++)
            {
                long x = grid[i][0] - grid[j][0];
                long y = grid[i][1] - grid[j][1];
                long z = grid[i][2] - grid[j][2];
                long dist = (x * x) + (y * y) + (z * z);
                if(dist < closest_dists[closest_dists_index - 1U])
                {
                    // bool first_found = false;
                    // bool second_found = false;
                    // for(size_t idx = 0U; idx < closest_dists_index; idx++)
                    // {
                    //     if((closest_first_indexes[idx] == i) || (closest_second_indexes[idx] == i))
                    //     {
                    //         first_found = true;
                    //     }
                    //     if((closest_first_indexes[idx] == j) || (closest_second_indexes[idx] == j))
                    //     {
                    //         second_found = true;
                    //     }

                    //     if(first_found && second_found)
                    //     {
                    //         break;
                    //     }
                    // }

                    // if(first_found && second_found)
                    // {
                    //     continue;
                    // }

                    size_t index = get_prev_index(closest_dists, closest_dists_index, dist);
                    if(closest_dists_index < GROUP_SIZE_V2)
                    {
                        closest_dists_index++;
                    }
                    shuffle_array(&closest_dists[index], closest_dists_index - index);
                    shuffle_array(&closest_first_indexes[index], closest_dists_index - index);
                    shuffle_array(&closest_second_indexes[index], closest_dists_index - index);
                    closest_dists[index] = dist;
                    closest_first_indexes[index] = i;
                    closest_second_indexes[index] = j;
                }
            }
        }

        size_t group_index = 0U;
        for(size_t iterations = 0U; iterations < GROUP_SIZE_V2; iterations++)
        {
            size_t first_index = closest_first_indexes[iterations];
            size_t second_index = closest_second_indexes[iterations];
            long group_1 = index_groups[first_index];
            long group_2 = index_groups[second_index];
            bool indexes_equal = (iterations > 0U) && ((closest_first_indexes[iterations - 1U] == first_index) && (closest_second_indexes[iterations - 1U] == second_index)) || ((closest_first_indexes[iterations - 1U] == second_index) && (closest_second_indexes[iterations - 1U] == first_index));
            if((indexes_equal) || ((group_1 != -1) && (group_1 == group_2)))
            {
                continue;
            }

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
                    if(index_groups_sizes[group_2] >= 1000)
                    {
                        (*circuits) = grid[first_index][0] * grid[second_index][0];
                        return;
                    }
                    group_2 = -1;
                }
                else if(group_2 == -1)
                {
                    index_groups[second_index] = group_1;
                    index_groups_sizes[group_1]++;
                    if(index_groups_sizes[group_1] >= 1000)
                    {
                        (*circuits) = grid[first_index][0] * grid[second_index][0];
                        return;
                    }
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
                    if(index_groups_sizes[first_group] >= 1000)
                    {
                        (*circuits) = grid[first_index][0] * grid[second_index][0];
                        return;
                    }
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

static void find_circuits_1(const char* buffer, size_t length)
{
    long circuits = 0;

    for(size_t i = 0U; i < length; i++)
    {
        // update_state_1(buffer[i], &circuits);
        update_state_1_v2(buffer[i], &circuits);
    }

    printf("%li\n", circuits);
}

static void find_circuits_2(const char* buffer, size_t length)
{
    long circuits = 0;

    for(size_t i = 0U; i < length; i++)
    {
        // update_state_2(buffer[i], &circuits);
        update_state_2_v2(buffer[i], &circuits);
    }

    printf("%li\n", circuits);
}

void solve_day_8(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        find_circuits_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        find_circuits_2(buffer, buffer_length);
    }
    else
    {
        find_circuits_1(buffer, buffer_length);
        find_circuits_2(buffer, buffer_length);
    }
}