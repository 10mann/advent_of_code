#include "common.h"
#include "day_1.h"
#include "day_2.h"
#include <stdio.h>
#include <time.h>

typedef void (*solver_function_t)(const char* filename, int part);

solver_function_t solver_functions_2025[] =
{
    solve_day_1,
    solve_day_2,
};

int main(int argc, char *argv[])
{
#if BENCHMARK == 1
    double start = get_time();
#endif
    const char* filename = "input.txt";
    int year = 0;
    int day = 0;
    int part = 0;

    if(argc > 4)
    {
        year = string_to_num(argv[1]);
        day = string_to_num(argv[2]);
        filename = argv[3];
        part = string_to_num(argv[4]);
        switch (year)
        {
            case 2025:
            {
                solver_functions_2025[day - 1](filename, part);
                break;
            }

            default:
            {
                printf("No solutions for year %i\n", year);
            }
        }
    }

#if BENCHMARK == 1
    double end = get_time();
    printf("Time: %f\n", end - start);
#endif
    return 0;
}