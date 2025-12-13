#include "day_10.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static void update_state_1(char c, long* button_presses)
{
    static char buffer[512] = {0};
    static size_t buffer_index = 0U;

    if((c == '\n') || (c == '\0'))
    {
        size_t light_index = 0U;
        size_t current_index = 1U;
        size_t button_index = 0U;
        uint16_t desired_pattern = 0U;
        uint16_t current_pattern = 0U;
        uint16_t buttons[32] = {0};
        light_index = 0U;
        for(; (buffer[current_index] != ']') && (current_index < buffer_index); current_index++)
        {
            if(buffer[current_index] == '#')
            {
                desired_pattern |= (1U << light_index);
            }
            light_index++;
        }

        for(; buffer[current_index] != '{'; current_index++)
        {
            if(buffer[current_index] == ')')
            {
                button_index++;
            }
            else if((buffer[current_index] >= '0') && (buffer[current_index] <= '9'))
            {
                buttons[button_index] |= (1U << (buffer[current_index] - '0'));
            }
        }

        size_t fewest_presses = 0U;
        current_pattern = 0U;
        size_t pattern_index = 0U;
        size_t start_index = 0U;
        size_t start_buttons = 0U;
        size_t button_indexes[32] = {0};
        size_t indexes_index = 0U;
        while((current_pattern != desired_pattern) && (indexes_index < button_index) && (start_buttons < button_index))
        {
            current_pattern = 0U;
            for(size_t i = 0U; i <= indexes_index; i++)
            {
                current_pattern ^= buttons[button_indexes[i]];
            }

            if(current_pattern == desired_pattern)
            {
                fewest_presses = indexes_index + 1U;
                break;
            }

            button_indexes[indexes_index]++;
            if(button_indexes[indexes_index] >= button_index)
            {
                size_t max_index = button_index;
                for(size_t i = indexes_index; i > 0U; i--)
                {
                    if(button_indexes[i] >= max_index)
                    {
                        button_indexes[i - 1U]++;
                        if(i == 1U)
                        {
                            for(size_t j = 1U; j <= indexes_index; j++)
                            {
                                button_indexes[j] = button_indexes[j - 1U] + 1U;
                            }
                        }
                        if((i == 1) && (button_indexes[0] >= max_index))
                        {
                            button_indexes[0] = button_index;
                        }
                        button_indexes[i] = button_indexes[i - 1U] + 1U;
                        if(button_indexes[i] >= max_index)
                        {
                            if(i > 1)
                            {
                                button_indexes[i] = button_indexes[i - 2U] + 1U;
                            }
                            else
                            {
                                button_indexes[0] = button_index;
                            }
                        }
                    }
                    max_index--;
                }
                if(button_indexes[0] >= button_index)
                {
                    indexes_index++;
                    for(size_t i = 0U; i <= indexes_index; i++)
                    {
                        button_indexes[i] = i;
                    }
                }
            }
        }

        (*button_presses) += fewest_presses;
        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0U;
    }
    else
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void find_button_presses_1(const char* filename)
{
    long button_presses = 0;
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
            update_state_1(buffer[i], &button_presses);
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
    printf("%li\n", button_presses);
}

void solve_day_10(const char* filename, int part)
{
    if(part == 1)
    {
        find_button_presses_1(filename);
    }
    else if(part == 2)
    {

    }
    else
    {
        find_button_presses_1(filename);
    }
}