import time
import subprocess
import sys

def time_process():
    start = time.perf_counter()
    command = "./" + sys.argv[1]
    year = sys.argv[2]
    day = sys.argv[3]
    filename = sys.argv[4]
    part = sys.argv[5]
    process = [command, year, day, filename, part]
    subprocess.run(process)
    end = time.perf_counter()
    return end - start

def main():
    iterations = 10
    delta_time = 0
    for i in range(iterations):
        delta_time += time_process()

    print("Average runtime: ", delta_time / iterations)

if __name__ == '__main__':
    main()