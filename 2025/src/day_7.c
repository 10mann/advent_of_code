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

static void update_state_2(char c, long* splits)
{
    static long split_grid[256][256] = {0};
    static char buffer[256][256];
    static size_t index = 0U;
    static size_t line_index = 0U;
    static size_t origin = 0U;
    long single_paths = 0;
    // static size_t row_index = 0U;
    
    if(c == '\n')
    {
        index = 0U;
        line_index++;
    }
    else if(c == '\0')
    {
        long tot_paths = 1;
        // split_grid[0][origin] = 1;
        bool doubled = true;
        for(size_t i = 1U; i < line_index; i++)
        {
            printf("Line: %lu\n", i);
            // long should_double = 0;
            bool line_contains_split = false;
            // for(size_t j = 0U; j < index; j++)
            // {
            //     if(buffer[i][j] == '^')
            //     {
            //         should_double++;
            //     }
            // }
            for(size_t j = 0U; j < index; j++)
            {
                if((buffer[i - 1U][j] == 'S'))
                {
                    if(buffer[i][j] == '^')
                    {
                        line_contains_split = true;
                        bool new_split_1 = false;
                        bool new_split_2 = false;
                        (*splits)++;
                        // split_grid[i][j] = split_grid[i - 1U][j] + 1;
                        if(j > 0)
                        {
                            buffer[i][j - 1U] = 'S';
                            split_grid[i][j - 1U] = 1;
                            if((i < (line_index - 2)) && (buffer[i + 2U][j - 1U] == '^'))
                            {
                                new_split_1 = true;
                                split_grid[i][j - 1U]++;
                                // break;
                            }
                            // for(size_t k = i + 1U; k < line_index; k++)
                            // {
                            //     if(buffer[k][j - 1U] == '^')
                            //     {
                            //         new_split_1 = true;
                            //         split_grid[i][j - 1U]++;
                            //         break;
                            //     }
                            // }
                        }

                        if(j < (index - 1U))
                        {
                            // printf("Adding S\n");
                            buffer[i][j + 1U] = 'S';
                            split_grid[i][j + 1U] = 1;
                            // split_grid[i][j + 1U] = split_grid[i - 1U][j] + 1;
                            if((i < (line_index - 2)) && (buffer[i + 2U][j + 1U] == '^'))
                            {
                                new_split_2 = true;
                                split_grid[i][j + 1U]++;
                                // break;
                            }
                            // for(size_t k = i + 1U; k < line_index; k++)
                            // {
                            //     if(buffer[k][j + 1U] == '^')
                            //     {
                            //         new_split_2 = true;
                            //         split_grid[i][j + 1U]++;
                            //         break;
                            //     }
                            // }
                        }

                        // if(!new_split_1 || !new_split_2)
                        if(!(new_split_1 && new_split_2))
                        {
                            // printf("Stop doubling\n");
                            doubled = false;
                        }
                        // else if(new_split_1 || new_split_2)
                        // {
                        //     // tot_paths++;
                        // }
                        // if(new_split_1 && new_split_2)
                        // {
                        //     tot_paths *= 2;
                        // }
                        // else if(new_split_1 || new_split_2)
                        // {
                        //     tot_paths += 2;
                        // }
                        // else
                        // {
                        //     single_paths++;
                        //     tot_paths++;
                        // }
                    }
                    else
                    {
                        buffer[i][j] = 'S';
                        // split_grid[i][j] = split_grid[i - 1U][j];
                    }
                }
            }
            if(doubled && line_contains_split)
            {
                tot_paths *= 2;
                printf("Double: %li\n", tot_paths);
            }
            // else
            // {
            //     printf("Not doubled: %lu\n", i);
            // }
        }


        for(size_t i = 0U; i < line_index; i++)
        {
            printf("%s\n", buffer[i]);
            // for(size_t j = 0U; j < index; j++)
            // {
            //     // if(buffer[i][j] == '^')
            //     {
            //         printf("%li ", split_grid[i][j]);
            //     }
            //     // else
            //     // {
            //     //     printf("0 ");
            //     // }
            // }
            // printf("\n");
        }

        long tot = 0;
        for(size_t i = 0U; i < line_index; i++)
        {
            long sum = 0;
            for(size_t j = 0U; j < index; j++)
            {
                sum += split_grid[i][j];
                // printf("%li+", split_grid[i][j]);
            }
            if(sum > 1)
            {
                // printf("Sum: %li\n", sum);
                // tot += (1 << (sum - 2));
                tot += sum;
            }
        }
        printf("Tot: %li\n", tot);

        // long sum = 0;

        long double_paths = 0;
        for(size_t i = 0U; i < index; i++)
        {
            if(split_grid[line_index - 3U][i] == 2)
            {
                double_paths++;
            }
        }

        printf("Singles: %li, doubles: %li\n", single_paths, double_paths);
        printf("Tot paths: %li\n", tot_paths);
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

static void calculate_splits_2(const char* buffer, size_t length)
{
    long splits = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &splits);
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
        // calculate_splits_2(buffer, buffer_length);
    }
    else
    {

    }
}