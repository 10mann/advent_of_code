#include "day_2.h"
#include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
    
static bool is_valid_id_2(long id)
{
    bool valid = true;
    char string[64] = {0};
    size_t length = num_to_string(string, sizeof(string), id);
    
    for(size_t i = 1U; i <= (length / 2); i++)
    {
        if((length % i) != 0)
        {
            continue;
        }

        size_t parts = length / i;
        bool match = true;
        for(size_t j = 0; j < (parts - 1U); j++)
        {
            if(memcmp(&string[j * i], &string[(j + 1U) * i], i) != 0)
            {
                match = false;
                break;
            }
        }

        if(match)
        {
            valid = false;
            break;
        }
    }

    return valid;
}

static bool is_valid_id_2_v2(long id)
{
    bool valid = true;
    char string[64] = {0};
    size_t length = num_to_string(string, sizeof(string), id);
    
    for(size_t i = 1U; i <= (length / 2); i++)
    {
        if(length & 1)
        {
            continue;
        }

        size_t parts = length / i;
        bool match = true;
        for(size_t j = 0; j < (parts - 1U); j++)
        {
            if(memcmp(&string[j * i], &string[(j + 1U) * i], i) != 0)
            {
                match = false;
                break;
            }
        }

        if(match)
        {
            valid = false;
            break;
        }
    }

    return valid;
}

static void update_state_1(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        num_2 = string_to_num(numbers[1]);
        buffer = 0;
        index = 0;

        size_t num_1_length = 0U;
        for(num_1_length = 0U; numbers[0][num_1_length] != '\0'; num_1_length++){}
        size_t num_2_length = 0U;
        for(num_2_length = 0U; numbers[1][num_2_length] != '\0'; num_2_length++){}

        bool length_1_odd = ((num_1_length % 2U) != 0U);
        bool skip = ((num_1_length == num_2_length) && length_1_odd);
        size_t next_power = 1U;
        if((num_2_length > num_1_length) && length_1_odd)
        {
            for(size_t i = 0U; i < num_1_length; i++)
            {
                next_power *= 10U;
            }
            num_1 = next_power;
        }

        memset(numbers, 0, sizeof(numbers));
        for(long i = num_1; (i <= num_2) && !skip; i++)
        {
            char string[64] = {0};
            size_t length = num_to_string(string, sizeof(string), i);
            size_t half_index = length / 2U;
            if(length & 1)
            {
                continue;
            }

            bool invalid = true;
            for(size_t j = 0U; j < half_index; j++)
            {
                if(string[j] != string[half_index + j])
                {
                    invalid = false;
                    break;
                }
            }

            size_t power = 1;
            for(size_t j = 0U; j < (half_index); j++)
            {
                power *= 10U;
            }

            if(invalid)
            {
                (*invalid_ids) += i;
                i += power;
            }
            else
            {
                long upper = i / power;
                long lower = i % power;
                if(lower > upper)
                {
                    next_power = 1U;
                    for(size_t i = 0U; i < (half_index); i++)
                    {
                        next_power *= 10U;
                    }
                    i += next_power - 1U - (lower - upper);
                    continue;
                }
                else
                {
                    i += (upper - lower) - 1U;
                }
            }
        }
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void update_state_1_v2(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;
    static size_t num_1_length = 0U;
    static size_t num_2_length = 0U;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        num_1_length = index;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        buffer = 0;
        num_2_length = index;
        index = 0;

        bool length_1_odd = ((num_1_length & 1));
        size_t next_power = 1U;
        if((num_2_length > num_1_length) && length_1_odd)
        {
            for(size_t i = 0U; i < num_1_length; i++)
            {
                next_power *= 10U;
            }
            num_1 = next_power;
            num_1_length++;
            num_to_string(numbers[0], sizeof(numbers[0]), num_1);
            length_1_odd = false;
        }

        if(((num_2_length - num_1_length) < 2U) && !length_1_odd)
        {
            num_2 = string_to_num(numbers[1]);
            size_t half_length = num_1_length / 2U;
            for(size_t i = 0U; i < half_length; i++)
            {
                numbers[0][half_length + i] = numbers[0][i];
            }
            char str_max_index[12] = {0};
            char str_factor[12] = {0};
            str_factor[0] = '1';
            for(size_t i = 1U; i < half_length; i++)
            {
                str_factor[i] = '0';
            }
            for(size_t i = 0U; i < (num_2_length / 2U); i++)
            {
                str_max_index[i] = '9';
            }
            str_factor[half_length] = '1';
            long num = string_to_num(str_factor);
            long max_index = string_to_num(str_max_index);
            long start_num = string_to_num(numbers[0]);
            if(start_num < num_1)
            {
                start_num += num;   
            }
            num_1 = start_num;
            long start = num_1 / num;
            long end = num_2 / num;
            if(end > max_index)
            {
                end = max_index;
            }

            long indexes = 0;
            for(long i = start; i <= end; i++)
            {
                indexes += i;
            }
            (*invalid_ids) += (num * (indexes));
            num_1 = num_2 + 1;
        }

        memset(numbers, 0, sizeof(numbers));
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void update_state_2(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        num_2 = string_to_num(numbers[1]);
        buffer = 0;
        index = 0;
        memset(numbers, 0, sizeof(numbers));
        for(long i = num_1; i <= num_2; i++)
        {
            if(!is_valid_id_2_v2(i))
            {
                // printf("invalid: %li\n", i);
                (*invalid_ids) += i;
            }
        }
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void update_state_2_v2(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        static size_t add_table[12] = {0};
        add_table[2] = 10;
        add_table[3] = 110;
        add_table[4] = 100;
        add_table[5] = 11110;
        add_table[6] = 90;
        add_table[7] = 111110;
        add_table[8] = 10000;
        add_table[9] = 100100;

        size_t num_1_length = 0U;
        for(num_1_length = 0U; numbers[0][num_1_length] != '\0'; num_1_length++){}
        size_t num_2_length = 0U;
        for(num_2_length = 0U; numbers[1][num_2_length] != '\0'; num_2_length++){}
        num_2 = string_to_num(numbers[1]);
        buffer = 0;
        index = 0;
        if(num_1 < 10)
        {
            num_1 = 10;
            num_1_length++;
        }
        if((num_1_length & 1) && (num_1_length < 8) && (num_1_length == num_2_length))
        {
            char temp[16] = {0};
            for(size_t i = 0U; i < num_1_length; i++)
            {
                temp[i] = '1';
            }
            long num = string_to_num(temp);
            long start = num_1 / num;
            long end = num_2 / num;
            size_t num_index = start + 1U;
            for(long i = start; i < end; i++)
            {
                long tmp = (num_index * num);
                (*invalid_ids) += tmp;
                num_index++;
            }
            num_1 = num_2 + 1;
        }
        if((num_1_length & 1) && (num_1_length < 8) && (num_1 < num_2))
        {
            char c = numbers[0][0];
            for(size_t i = 0U; i < num_1_length; i++)
            {
                if(numbers[0][i] < numbers[0][0])
                {
                    break;
                }
                if(numbers[0][i] > numbers[0][0])
                {
                    c++;
                    break;
                }
            }

            for(size_t i = 0U; i < num_1_length; i++)
            {
                numbers[0][i] = c;
            }
            num_1 = string_to_num(numbers[0]);
        }

        memset(numbers, 0, sizeof(numbers));
    
        for(long i = num_1; i <= num_2; i++)
        {
            char string[64] = {0};
            size_t length = num_to_string(string, sizeof(string), i);
            bool valid = true;
            bool odd = length & 1;

            if((length == 2U) && (string[0] == string[1]))
            {
                valid = false;
            }

            if(valid)
            {
                // if(odd)
                // {

                    size_t half_length = length / 2U;
                    for(size_t index = 1U; valid && (index <= half_length); index++)
                    {
                        if((length % index) != 0U)
                        {
                            continue;
                        }
                        size_t parts = length / index;
                        bool match = true;
                        for(size_t j = 0; j < (parts - 1U); j++)
                        {
                            if(memcmp(&string[j * index], &string[(j + 1U) * index], index) != 0)
                            {
                                match = false;
                                break;
                            }
                        }
                        
                        if(match)
                        {
                            valid = false;
                            break;
                        }
                    }
                // }
                // else
                // {
                //     size_t xor = 0U;
                //     size_t half_length = length / 2U;
                //     for(size_t index = 0U; index < half_length; index++)
                //     {
                //         xor ^= (string[index] << (index + 2));
                //     }
                //     for(size_t index = 0U; index < half_length; index++)
                //     {
                //         xor ^= (string[index + half_length] << (index + 2));
                //     }

                //     valid = xor;
                // }
            }

            if(!valid)
            {
                // printf("Invalid: %lu\n", i);
                (*invalid_ids) += i;
                if(string[0] < '9')
                {
                    i += add_table[length];
                }
            }
        }
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void update_state_2_v3(char c, long* invalid_ids)
{
    static char numbers[2][256] = {0};
    static long num_1 = 0;
    static long num_2 = 0;
    static long buffer = 0;
    static int index = 0;
    static size_t checks = 0U;

    if(c == '-')
    {
        num_1 = string_to_num(numbers[0]);
        buffer = 1;
        index = 0;
    }
    else if((c == ',') || (c == '\0') || (c == '\n'))
    {
        // size_t num_1_length = 0U;
        // for(num_1_length = 0U; numbers[0][num_1_length] != '\0'; num_1_length++){}
        size_t num_2_length = 0U;
        for(num_2_length = 0U; numbers[1][num_2_length] != '\0'; num_2_length++){}
        num_2 = string_to_num(numbers[1]);
        buffer = 0;
        index = 0;
        // memset(numbers, 0, sizeof(numbers));
        char string[64] = {0};
        size_t length = num_to_string(string, sizeof(string), num_1);
        // printf("Length: %lu\n", length);
        if(length == 1U)
        {
            string[0] = '1';
            string[1] = '1';
            length = 2U;
            num_1 = 11;
        }

        for(long i = num_1; i <= num_2; i++)
        {
            // printf("%s, length: %lu\n", string, length);
            bool valid = true;
            bool odd = length & 1;
            size_t match_length = 0U;

            // if(length == 1U)
            // {
            //     checks++;
            //     continue;
            // }
            // else 
            if((length == 2U) && (string[0] == string[1]))
            {
                checks++;
                valid = false;
            }

            if(valid && odd)
            {
                valid = false;
                for(size_t j = 1U; j < length; j++)
                {
                    if(string[j - 1] != string[j])
                    {
                        checks++;
                        valid = true;
                        break;
                    }
                }
            }

            if(valid)
            {
                size_t half_length = length / 2U;
                for(size_t index = 1U; valid && (index <= half_length); index++)
                {
                    if((length % index) != 0U)
                    {
                        checks++;
                        continue;
                    }
                    size_t parts = length / index;
                    bool match = true;
                    for(size_t j = 0; j < (parts - 1U); j++)
                    {
                        if(memcmp(&string[j * index], &string[(j + 1U) * index], index) != 0)
                        {
                            checks++;
                            match = false;
                            break;
                        }
                    }
                    
                    if(match)
                    {
                        match_length = index;
                        checks++;
                        valid = false;
                        break;
                    }
                }
            }

            if(!valid)
            {
                printf("Invalid: %s\n", string);
                (*invalid_ids) += string_to_num(string);
                // (*invalid_ids) += i;
                // if(length == 2U)
                
                // size_t half_length = length / 2U;
                // // if(match_length == half_length)
                // {
                //     string[half_length]++;
                //     long num = string_to_num(string);
                //     if(num > num_2)
                //     {
                //         break;
                //     }
                //     if((string[0] > numbers[1][0]) && (length == num_2_length))
                //     {
                //         break;
                //     }
                // }
                // size_t power = 1;
                // for(size_t len = 0U; len < (length / 2U); len++)
                // {
                //     power *= 10U;
                // }
                // i += power;
                // if(length == 2U)
                // {
                //     i += 10;
                // }
                // else if(length == 4U)
                // {
                //     i += 100;
                // }
            }

            bool match = true;
            for(size_t str_len = 0U; str_len < num_2_length; str_len++)
            {
                if(string[str_len] != numbers[1][str_len])
                {
                    match = false;
                    break;
                }
            }
            if(match)
            {
                break;
            }

            string[length - 1U]++;
            for(size_t str_len = 0U; str_len < (length - 1U); str_len++)
            {
                if(string[length - str_len - 1U] > '9')
                {
                    string[length - str_len - 1U] = '0';
                    string[length - str_len - 2U]++;
                }
            }
            if(string[0] > '9')
            {
                length++;
                string[0] = '1';
                for(size_t str_len = 1U; str_len < length; str_len++)
                {
                    string[str_len] = '0';
                }
                if((length & 1) == 0)
                {
                    string[length / 2U] = '1';
                }
                long num = string_to_num(string);
                if(num > num_2)
                {
                    break;
                }
                // string[length - 1U] = '0';
            }
        }
        memset(numbers, 0, sizeof(numbers));

        if(c == '\0')
        {
            printf("Checks: %lu\n", checks);
        }
    }
    else if((c >= '0') && (0 <= '9'))
    {
        numbers[buffer][index] = c;
        index++;
    }
}

static void find_valid_ids_1(const char* buffer, size_t length)
{
    long invalid_ids = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_1_v2(buffer[i], &invalid_ids);
    }

    printf("%li\n", invalid_ids);
}

static void find_valid_ids_2(const char* buffer, size_t length)
{
    long invalid_ids = 0;

    for(size_t i = 0U; i < length; i++)
    {
        update_state_2_v2(buffer[i], &invalid_ids);
    }

    printf("%li\n", invalid_ids);
}

void solve_day_2(const char* filename, int part, bool dryrun)
{
    char buffer[32 * 1024] = {0};
    size_t buffer_length = read_file(filename, buffer, sizeof(buffer));

    if(dryrun)
    {

    }
    else if(part == 1)
    {
        find_valid_ids_1(buffer, buffer_length);
    }
    else if(part == 2)
    {
        find_valid_ids_2(buffer, buffer_length);
    }
    else
    {
        find_valid_ids_1(buffer, buffer_length);
        find_valid_ids_2(buffer, buffer_length);
    }
}