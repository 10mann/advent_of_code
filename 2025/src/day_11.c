#include "day_11.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define NODE_COUNT 1024
#define NODE_NAME_LENGTH 4
#define OUTPUT_COUNT 32
#define NODE_HISTORY_COUNT 32
#define GOOD_PATHS_COUNT 1024

typedef struct
{
    uint16_t outputs[OUTPUT_COUNT];
    uint16_t length;
} node_t;

static node_t nodes[NODE_COUNT] = {0};
static node_t index_nodes[NODE_COUNT] = {0};
static uint16_t name_hashes[NODE_COUNT] = {0};
static char names[NODE_COUNT][NODE_NAME_LENGTH] = {0};
static size_t searches = 0U;
static uint16_t dead_paths[GOOD_PATHS_COUNT] = {0};
static size_t dead_paths_count = 0U;
static uint16_t good_paths[GOOD_PATHS_COUNT] = {0};
static size_t good_paths_count = 0U;

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

    return UINT16_MAX;
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
    if(index < GOOD_PATHS_COUNT)
    {
        if(good_paths[index] > 0)
        {
            return good_paths[index];
        }

        if(dead_paths[index] > 0)
        {
            return 0;
        }
    }
    for(size_t i = 0U; i < nodes[index].length; i++)
    {
        if(nodes[index].outputs[i] == end_hash)
        {
            return 1U;
        }
        paths += find_path(get_hash_index(nodes[index].outputs[i]), end_hash, level, max_depth);
        if(index < GOOD_PATHS_COUNT)
        {
            if(paths > 0)
            {
                good_paths[index] = paths;
                good_paths_count++;
            }
            else
            {
                dead_paths[index] = 1;
                dead_paths_count++;
            }
        }
    }

    return paths;
}

static long find_path_2(size_t index, uint16_t end_hash, bool fft_found, uint16_t fft_hash, bool dac_found, uint16_t dac_hash, size_t depth, size_t max_depth)
{
    searches++;
    long paths = 0;
    bool fft = fft_found;
    bool dac = dac_found;
    size_t level = depth + 1U;
    if(index < GOOD_PATHS_COUNT)
    {
        if(good_paths[index] > 0)
        {
            return good_paths[index];
        }

        if(dead_paths[index] > 0)
        {
            return 0;
        }
    }
    if(level >= max_depth)
    {
        return 0;
    }
    for(size_t i = 0U; i < nodes[index].length; i++)
    {
        if(nodes[index].outputs[i] == fft_hash)
        {
            fft = true;
        }
        else if(nodes[index].outputs[i] == dac_hash)
        {
            dac = true;
        }
        else if(nodes[index].outputs[i] == end_hash)
        {
            if(dac && fft)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        
        size_t next_index = get_hash_index(nodes[index].outputs[i]);
        paths += find_path_2(next_index, end_hash, fft, fft_hash, dac, dac_hash, level, max_depth);
        if(index < GOOD_PATHS_COUNT)
        {
            if(paths > 0)
            {
                good_paths[index] = paths;
                good_paths_count++;
            }
            else
            {
                dead_paths[index] = 1;
                dead_paths_count++;
            }
        }
    }

    return paths;
}

static long find_path_2_v2(size_t index, uint16_t end_index, bool fft_found, uint16_t fft_index, bool dac_found, uint16_t dac_index, size_t depth, size_t max_depth)
{
    long paths = 0;
    bool fft = fft_found;
    bool dac = dac_found;
    size_t level = depth + 1U;
    if(index > NODE_COUNT)
    {
        printf("Index: %lu\n", index);
    }
    for(size_t i = 0U; i < dead_paths_count; i++)
    {
        if(index == dead_paths[i])
        {
            return 0;
        }
    }
    if(index < GOOD_PATHS_COUNT)
    {
        if(good_paths[index] > 0)
        {
            // printf("Good paths: %u\n", good_paths[index]);
            return good_paths[index];
            // return 1;
        }
    }
    // for(size_t i = 0U; i < good_paths_count; i++)
    // {
    //     if(index == good_paths[i])
    //     {
    //         return 0;
    //     }
    // }
    if(level >= max_depth)
    {
        for(size_t i = 0U; i < dead_paths_count; i++)
        {
            if(index == dead_paths[i])
            {
                return 0;
            }
        }
        dead_paths[dead_paths_count] = index;
        dead_paths_count++;
        return 0;
    }
    // printf("Index: %lu\n", index);
    for(size_t i = 0U; i < index_nodes[index].length; i++)
    {
        if(index_nodes[index].outputs[i] == fft_index)
        {
            fft = true;
        }
        else if(index_nodes[index].outputs[i] == dac_index)
        {
            dac = true;
        }
        else if(index_nodes[index].outputs[i] == end_index)
        {
            if(dac && fft)
            {
                return 1;
            }
            else
            {
                if(dead_paths_count < GOOD_PATHS_COUNT)
                {
                    for(size_t i = 0U; i < dead_paths_count; i++)
                    {
                        if(index == dead_paths[i])
                        {
                            return 0;
                        }
                    }
                    dead_paths[dead_paths_count] = index;
                    dead_paths_count++;
                }
                return 0;
            }
        }
        
        paths += find_path_2_v2(index_nodes[index].outputs[i], end_index, fft, fft_index, dac, dac_index, level, max_depth);
        if(index < GOOD_PATHS_COUNT)
        {
            good_paths[index] = paths;
            good_paths_count++;
        }
    }

    searches++;
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

static void update_state_2(char c, long* all_paths)
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
            uint16_t start_hash = get_hash("svr", NODE_NAME_LENGTH - 1U);
            uint16_t end_hash = get_hash("out", NODE_NAME_LENGTH - 1U);
            uint16_t fft_hash = get_hash("fft", NODE_NAME_LENGTH - 1U);
            uint16_t dac_hash = get_hash("dac", NODE_NAME_LENGTH - 1U);
            size_t start_index = 0U;
            for(size_t i = 0U; i < line_index; i++)
            {
                if(name_hashes[i] == start_hash)
                {
                    start_index = i;
                    break;
                }
            }

            // (*all_paths) = find_path(start_index, end_hash, 0U, line_index);
            // (*all_paths) = find_path_2(start_index, end_hash, true, fft_hash, true , dac_hash, 0U, 15);
            (*all_paths) = find_path_2(start_index, end_hash, false, fft_hash, false, dac_hash, 0U, 400);
            printf("Searches: %lu\n", searches);
            printf("Dead paths: %lu\n", dead_paths_count);
        }
    }
    else
    {
        buffer[buffer_index] = c;
        buffer_index++;
    }
}

static void update_state_2_v2(char c, long* all_paths)
{
    static char buffer[256] = {0};
    static size_t buffer_index = 0U;
    static size_t line_index = 0U;
    // static uint16_t outputs[NODE_COUNT][OUTPUT_COUNT] = {0};

    if((c == '\n') || (c == '\0'))
    {
        // printf("%s\n", buffer);
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
            uint16_t start_index = get_hash_index(get_hash("svr", NODE_NAME_LENGTH - 1U));
            // uint16_t start_index = get_hash_index(get_hash("you", NODE_NAME_LENGTH - 1U));
            // uint16_t start_index = get_hash_index(get_hash("fok", NODE_NAME_LENGTH - 1U));
            uint16_t end_index = UINT16_MAX;
            uint16_t fft_index = get_hash_index(get_hash("fft", NODE_NAME_LENGTH - 1U));
            uint16_t dac_index = get_hash_index(get_hash("dac", NODE_NAME_LENGTH - 1U));
            for(size_t i = 0U; i < line_index; i++)
            {
                dead_paths[i] = UINT16_MAX - 1U;
                index_nodes[i].length = nodes[i].length;
                for(size_t j = 0U; j < nodes[i].length; j++)
                {
                    index_nodes[i].outputs[j] = get_hash_index(nodes[i].outputs[j]);
                }
            }

            // uint16_t end_index = get_hash_index(get_hash("out", NODE_NAME_LENGTH - 1U));
            // printf("Start index: %u\n", start_index);
            // printf("End index: %u\n", end_index);
            // printf("FFT index: %u\n", fft_index);
            // printf("Dac index: %u\n", dac_index);

            (*all_paths) = find_path_2_v2(start_index, end_index, false, fft_index, false, dac_index, 0U, 2);
            printf("Searches: %lu\n", searches);
            printf("Dead paths: %lu\n", dead_paths_count);
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

static void find_all_paths_2(const char* filename)
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
            update_state_2(buffer[i], &all_paths);
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

static void find_all_paths_2_v2(const char* filename)
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
            update_state_2_v2(buffer[i], &all_paths);
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
        // find_all_paths_2(filename);
        // find_all_paths_2_v2(filename);
    }
    else
    {

    }
}