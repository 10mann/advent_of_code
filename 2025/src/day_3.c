#include "day_3.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

static void update_state_1(char c, long* joltage)
{
    static char battery[256] = {0};
    static size_t index = 0U;

    if((c >= '0') && (c <= '9'))
    {
        battery[index] = c;
        index++;
    }
    else
    {
        int first_num = 0;
        int second_num = 0;
        size_t current_index = 0U;
        for(size_t i = 0U; (i < (index - 1)) && (index > 0); i++)
        {
            if(battery[i] > first_num)
            {
                current_index = i + 1U;
                first_num = battery[i];
            }
        }

        for(size_t i = current_index; i < index; i++)
        {
            if(battery[i] > second_num)
            {
                second_num = battery[i];
            }
        }

        long jolt = ((first_num - '0') * 10) + (second_num - '0');
        (*joltage) += jolt;
        memset(battery, 0, sizeof(battery));
        index = 0U;
    }
}

static void update_state_2(char c, long* joltage)
{
    static char battery[256] = {0};
    static size_t index = 0U;

    if((c >= '0') && (c <= '9'))
    {
        battery[index] = c;
        index++;
    }
    else
    {
        char numbers[13] = {0};
        size_t current_index = 0U;
        long jolt = 0;
        long power = 100000000000;

        for(size_t num_index = 12U; num_index > 0U; num_index--)
        {
            for(size_t i = current_index; i <= (index - num_index); i++)
            {
                if(battery[i] > numbers[num_index - 1U])
                {
                    current_index = i + 1U;
                    numbers[num_index - 1U] = battery[i];
                }
            }

            jolt += (numbers[num_index - 1U] - '0') * power;
            power /= 10;
        }

        (*joltage) += jolt;
        memset(battery, 0, sizeof(battery));
        index = 0U;
    }
}

static void find_joltage_1(const char* buffer, size_t length)
{
    long joltage = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &joltage);
    }

    printf("%li\n", joltage);
}

static void find_joltage_2(const char* buffer, size_t length)
{
    long joltage = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &joltage);
    }

    printf("%li\n", joltage);
}

void solve_day_3(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        find_joltage_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        find_joltage_2(buffer, buffer_length);
    }
    else
    {
        find_joltage_1(buffer, buffer_length);
        find_joltage_2(buffer, buffer_length);
    }
}