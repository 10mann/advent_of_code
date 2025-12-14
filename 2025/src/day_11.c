#include "day_11.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define NODE_COUNT 1024
#define NODE_NAME_LENGTH 4
#define OUTPUT_COUNT 32

typedef struct
{
    uint16_t outputs[OUTPUT_COUNT];
    uint16_t length;
} node_t;

static node_t nodes[NODE_COUNT] = {0};
static uint16_t name_hashes[NODE_COUNT] = {0};
static char names[NODE_COUNT][NODE_NAME_LENGTH] = {0};

static size_t get_name_index(const char* name)
{
    for(size_t i = 0U; (names[i][0] != '\0') && (i < NODE_COUNT); i++)
    {
        for(size_t j = 0U; j < NODE_NAME_LENGTH; j++)
        {
            if(name[j] != names[i][j])
            {
                break;
            }
            return i;
        }
    }

    return 0U;
}

static size_t get_hash_index(uint16_t hash)
{
    for(size_t i = 0U; name_hashes[i] != 0U; i++)
    {
        if(name_hashes[i] == hash)
        {
            return i;
        }
    }

    return 0;
}

static uint16_t get_hash(const char *str, size_t length)
{
    uint16_t hash = 5381;
    for(size_t i = 0U; i < length; i++)
    {
        hash = ((hash << 5) + hash) + str[i]; 
    }

    return hash;
}

static long find_path(size_t index, uint16_t end_hash, size_t depth, size_t max_depth)
{
    long paths = 0;
    size_t level = depth + 1U;
    if(level >= max_depth)
    {
        return 0U;
    }
    for(size_t i = 0U; i < nodes[index].length; i++)
    {
        if(nodes[index].outputs[i] == end_hash)
        {
            return 1U;
        }
        paths += find_path(get_hash_index(nodes[index].outputs[i]), end_hash, level, max_depth);
    }

    return paths;
}

static void update_state_1(char c, long* all_paths)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static size_t line_index = 0U;

    if((c == '\n') || (c == '\0'))
    {
        size_t index = 0U;
        for(; buffer[index] != ':'; index++)
        {
            names[line_index][index] = buffer[index];
        }

        name_hashes[line_index] = get_hash(names[line_index], NODE_NAME_LENGTH - 1U);
        index += 2U;
        char output_name[NODE_NAME_LENGTH] = {0};
        size_t output_name_index = 0U;
        for(; buffer[index - 1U] != '\0'; index++)
        {
            if((buffer[index] >= 'a') && (buffer[index] <= 'z'))
            {
                output_name[output_name_index] = buffer[index];
                output_name_index++;
            }
            else
            {
                nodes[line_index].outputs[nodes[line_index].length] = get_hash(output_name, NODE_NAME_LENGTH - 1U);
                nodes[line_index].length++;
                output_name_index = 0U;
                memset(output_name, 0, sizeof(output_name));
            }
        }

        line_index++;
        buffer_index = 0U;
        memset(buffer, 0, sizeof(buffer));

        if(c == '\0')
        {
            uint16_t start_hash = get_hash("you", NODE_NAME_LENGTH - 1U);
            uint16_t end_hash = get_hash("out", NODE_NAME_LENGTH - 1U);
            size_t start_index = 0U;
            for(size_t i = 0U; i < line_index; i++)
            {
                if(name_hashes[i] == start_hash)
                {
                    start_index = i;
                    break;
                }
            }

            (*all_paths) = find_path(start_index, end_hash, 0U, line_index);
        }
    }
    else
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void find_all_paths_1(const char* filename)
{
    long all_paths = 0;
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
            update_state_1(buffer[i], &all_paths);
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
    printf("%li\n", all_paths);
}

void solve_day_11(const char* filename, int part)
{
    if(part == 1)
    {
        find_all_paths_1(filename);
    }
    else if(part == 2)
    {

    }
    else
    {

    }
}