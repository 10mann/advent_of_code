#include "day_10.h"
#include "common.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    uint16_t indexes[32];
    uint16_t length;
} button_t;

static size_t get_max_presses(uint16_t* jolts, size_t jolt_length, button_t* button)
{
    size_t max_presses = SIZE_MAX;
    for(size_t i = 0U; (i < button->length) && (i < jolt_length); i++)
    {
        if(jolts[button->indexes[i]] < max_presses)
        {
            max_presses = jolts[button->indexes[i]];
        }
    }

    return max_presses;
}

static bool check_joltage(const uint16_t* desired_jolts, size_t jolt_length, const uint16_t* indexes, size_t index_length, const button_t* buttons, size_t button_length)
{
    bool equal = true;
    uint16_t current_jolts[32] = {0};
    for(size_t i = 0U; i < index_length; i++)
    {
        for(size_t j = 0U; j < buttons[i].length; j++)
        {
            current_jolts[buttons[i].indexes[j]] += indexes[i];
        }
    }

    for(size_t i = 0U; i < jolt_length; i++)
    {
        // printf("Desired: %u, current: %u\n", desired_jolts[i], current_jolts[i]);
        if(current_jolts[i] != desired_jolts[i])
        {
            equal = false;
            break;
        }
    }

    return equal;
}

static bool check_all_combinations(int N, int x, int idx, uint16_t arr[], const uint16_t* desired_jolts, size_t length, const button_t* buttons, size_t button_length) 
{
    if (idx == x - 1) 
    {
        arr[idx] = N;
        // for (int i = 0; i < x; i++) 
        // {
        //     printf("%d ", arr[i]);
        // }
        // printf("\n");
        if(check_joltage(desired_jolts, length, arr, button_length, buttons, button_length))
        {
            return true;
        }
        return false;
    }
    for (int i = 0; i <= N; i++) 
    {
        arr[idx] = i;
        if(check_all_combinations(N - i, x, idx + 1, arr, desired_jolts, length, buttons, button_length))
        {
            return true;
        }
    }
    return false;
}

// static bool check_all_combinations(const uint16_t* desired_jolts, size_t length, const button_t* buttons, size_t button_length, size_t presses, size_t index_count, size_t index, uint16_t indexes[]) 
// {
//     if (index == (index_count - 1)) 
//     {
//         printf("Presses: %lu\n", presses);
//         indexes[index] = presses;
//         for (int i = 0; i < presses; i++)
//         {
//             printf("%d ", indexes[i]);
//         } 
//         printf("\n");
//         if(check_joltage(desired_jolts, length, indexes, index_count, buttons, button_length))
//         {
//             return true;
//         }
//         return false;
//     }

//     for (int i = 0; i <= presses; i++) 
//     {
//         indexes[index] = i;
//         if(check_all_combinations(desired_jolts, length, buttons, button_length, presses - i, index_count, index + 1, indexes))
//         {
//             return true;
//         }
//     }

//     return false;
// }

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

static void update_state_2(char c, long* button_presses)
{
    static char buffer[512] = {0};
    static size_t buffer_index = 0U;

    if((c == '\n') || (c == '\0'))
    {
        size_t current_index = 1U;
        button_t buttons[32] = {0};
        size_t button_index = 0U;
        size_t button_subindex = 0U;
        uint16_t jolts[32] = {0};
        size_t jolt_index = 0U;
        uint16_t current_jolts[32] = {0};
        char string[8] = {0};
        size_t string_index = 0U;
        uint16_t indexes[32] = {0};
        uint16_t max_indexes[32] = {0};

        for(; (buffer[current_index - 1U] != ']') && (current_index < buffer_index); current_index++)
        {
        }

        for(; buffer[current_index] != '{'; current_index++)
        {
            if(buffer[current_index] == ')')
            {
                button_index++;
                button_subindex = 0U;
            }
            else if((buffer[current_index] >= '0') && (buffer[current_index] <= '9'))
            {
                buttons[button_index].indexes[button_subindex] = (buffer[current_index] - '0');
                buttons[button_index].length++;
                button_subindex++;
            }
        }

        for(; buffer[current_index] != '\0'; current_index++)
        {
            if((buffer[current_index] >= '0') && (buffer[current_index] <= '9'))
            {
                string[string_index] = buffer[current_index];
                string_index++;
            }
            else if(string_index > 0U)
            {
                jolts[jolt_index] = string_to_num(string);
                string_index = 0U;
                memset(string, 0, sizeof(string));
                jolt_index++;
            }
        }

        long minimum_presses = 0;
        for(size_t i = 0U; i < button_index; i++)
        {
            max_indexes[i] = get_max_presses(jolts, jolt_index, &buttons[i]);
            if(max_indexes[i] > minimum_presses)
            {
                minimum_presses = max_indexes[i];
            }
        }

        while(minimum_presses < 200)
        {
            memset(indexes, 0, sizeof(indexes));
            if(check_all_combinations(minimum_presses, button_index, 0U, indexes, jolts, jolt_index, buttons, button_index))
            {
                break;
            }
            minimum_presses++;
        }

        (*button_presses) += minimum_presses;

        memset(buffer, 0, sizeof(buffer));
        buffer_index = 0U;
    }
    else
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void find_button_presses_1(const char* buffer, size_t length)
{
    long button_presses = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1(buffer[i], &button_presses);
    }

    printf("%li\n", button_presses);
}

static void find_button_presses_2(const char* buffer, size_t length)
{
    long button_presses = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2(buffer[i], &button_presses);
    }

    printf("%li\n", button_presses);
}

void solve_day_10(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        find_button_presses_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        // find_button_presses_2(buffer, buffer_length);
    }
    else
    {
        find_button_presses_1(buffer, buffer_length);
    }
}