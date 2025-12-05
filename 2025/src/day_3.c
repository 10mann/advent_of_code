#include "day_3.h"
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
        for(size_t i = 0U; i < (index - 1); i++)
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

static void find_joltage_1(const char* filename)
{
    long joltage = 0;
    FILE* input_text = fopen(filename, "r");
    while(input_text != NULL)
    {
        char buffer[8 * 1024] = {0};
        size_t length = fread(buffer, sizeof(buffer[0]), sizeof(buffer), input_text);
        if(length < sizeof(buffer))
        {
            buffer[length] = '\0';
            length++;
        }

        for(size_t i = 0U; i < length; i++)
        {
            update_state_1(buffer[i], &joltage);
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

    printf("%li\n", joltage);
}

void solve_day_3(const char* filename, int part)
{
    find_joltage_1(filename);
}