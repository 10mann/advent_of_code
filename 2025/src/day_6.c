#include "day_6.h"
#include "common.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static void update_state_1(char c, long* sum)
{
    static char prev_char = ' ';
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static long numbers[8][1024] = {0};
    static size_t number_index = 0U;
    static size_t number_count = 0U;
    static size_t line_index = 0U;
    static char operators[1024] = {};
    static size_t operator_index = 0U;

    if((c == ' ') && (prev_char >= '0') && (prev_char<= '9'))
    {
        numbers[line_index][number_index] = string_to_num(buffer);
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0U;
        number_index++;
        if(number_index > number_count)
        {
            number_count = number_index;
        }
    }
    else if((c >= '0') && (c <= '9'))
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
    else if((c == '+') || (c == '*'))
    {
        operators[operator_index] = c;
        operator_index++;
    }
    else if(c == '\n')
    {
        if(prev_char != ' ')
        {
            numbers[line_index][number_index] = string_to_num(buffer);
            memset(buffer, 0, sizeof(buffer));
            buffer_index = 0U;
        }

        line_index++;
        number_index = 0U;
    }
    else if(c == '\0')
    {
        number_count++;
        for(size_t i = 0U; i < number_count; i++)
        {
            long result = numbers[0][i];
            for(size_t j = 1U; j < line_index; j++)
            {
                if(operators[i] == '+')
                {
                    result += numbers[j][i];
                }
                else
                {
                    result *= numbers[j][i];
                }
            }
            (*sum) += result;
        }
    }

    prev_char = c;
}

static void update_state_2(char c, long* sum)
{
    static char lines[8][4096] = {0};
    static char operators[4096] = {0};
    static size_t operator_index = 0U;
    static size_t index = 0U;
    static size_t line_index = 0U;
    static size_t length = 0U;

    if(((c == ' ') || ((c >= '0') && (c <= '9'))) && (operator_index == 0U))
    {
        lines[line_index][index] = c;
        index++;
        if(index > length)
        {
            length = index;
        }
    }
    else if((c == '+') || (c == '*') || ((c == ' ') && (operator_index > 0U)))
    {
        operators[operator_index] = c;
        operator_index++;
    }
    else if(c == '\n')
    {
        line_index++;
        index = 0;
    }
    else if(c == '\0')
    {
        size_t operator_index = length;
        bool multiply = false;
        long result = 0;

        for(size_t j = operator_index - 1U; operator_index > 0U; j--)
        {
            if((operators[j] == '+') || (operators[j] == '*'))
            {
                multiply = operators[j] == '*';
                operator_index = j;
                break;
            }
        }
        for(size_t i = 0U; i < length; i++)
        {
            size_t char_index = length - i - 1U;

            long num = 0;
            bool number_found = false;
            for(size_t j = 0U; j < line_index; j++)
            {
                if((lines[j][char_index] >= '0') && (lines[j][char_index] <= '9'))
                {
                    num *= 10;
                    num += lines[j][char_index] - '0';
                    number_found = true;
                }
            }

            if(number_found)
            {
                if(multiply)
                {
                    if(result == 0)
                    {
                        result = num;
                    }
                    else
                    {
                        result *= num;
                    }
                }
                else
                {
                    result += num;
                }
            }
            else
            {
                for(size_t j = operator_index - 1U; operator_index > 0U; j--)
                {
                    if((operators[j] == '+') || (operators[j] == '*'))
                    {
                        multiply = operators[j] == '*';
                        operator_index = j;
                        break;
                    }
                }
            }

            if(operators[char_index] != ' ')
            {
                (*sum) += result;
                result = 0;
            }
        }
    }
}

static void solve_problems_1(const char* buffer, size_t length)
{
    long sum = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &sum);
    }

    printf("%li\n", sum);
}

static void solve_problems_2(const char* buffer, size_t length)
{
    long sum = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &sum);
    }

    printf("%li\n", sum);
}

void solve_day_6(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        solve_problems_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        solve_problems_2(buffer, buffer_length);
    }
    else
    {
        solve_problems_1(buffer, buffer_length);
        solve_problems_2(buffer, buffer_length);
    }
}