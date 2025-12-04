from os import listdir
from os.path import isdir, isfile, join
import sys
import subprocess

def find_includes():
    pass

def main():
    path = "."
    
    build_flags = []
    for i in range(1, len(sys.argv)):
        if sys.argv[i] == "--d":
            build_flags.append("-DDEBUG_LOG")
        elif sys.argv[i] == "--p":
            path = sys.argv[i + 1]
        elif sys.argv[i] == "--t":
            build_flags.append("-DBENCHMARK")
    topdirs = [f for f in listdir(path) if isdir(join(path, f))]
    includes = []
    source_files = []
    for dir in topdirs:
        subdirs = [f for f in listdir(dir) if isdir(join(dir, f))]
        for subdir in subdirs:
            if subdir == "include":
                includes.append("-I" + dir + "/" + subdir)
            current_path = dir + "/" + subdir
            files = [f for f in listdir(current_path) if isfile(join(current_path, f))]
            for file in files:
                if file.endswith(".c"):
                    source_files.append(current_path + "/" + file)
            
    print(includes)
    print(source_files)
    command = []
    command.append("gcc")
    command.append("main.c")
    for include in includes:
        command.append(include)
    for file in source_files:
        command.append(file)
    for flag in build_flags:
        command.append(flag)
    command.append("-O3")
    command.append("-o")
    command.append("build/advent_of_code.out")
    print(command)
    # print(onlydirs)
    subprocess.run(command)

if __name__ == "__main__":
    main()