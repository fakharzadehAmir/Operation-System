# Threads and Processes

This directory is dedicated to practicing with threads and processes in programming. Below are detailed descriptions of exercises that focus on utilizing multi-threading and multi-processing to solve various problems. Along with each exercise, appropriate input and output examples are provided to guide implementation and testing.

## Search File

Implement a multi-threaded search operation where each directory within a specified starting directory is searched in a separate thread. The program receives a search query and a starting directory as input and outputs the paths of files that match the search query. The search must be conducted using multiple threads, with each subdirectory being processed by a new thread, to qualify for points.

**Input:** `<search_query> <start_dir> search`

**Output:**
```
Found: [+] ./path/to/file
Found: [+] ./path2/to/file
...
```

## Word Counter

This task requires performing a search for multiple queries within a single file using multi-processing. Each query should be handled by a separate process, and the program should output the number of occurrences for each query as well as the total occurrences of all queries combined.

**Input:** `search <file_to_search> <query1> <query2> <query3> ....`

**Output:**
```
query1: <occurrences of query1>
query2: <occurrences of query2>
query3: <occurrences of query3>
...
Total occurrences: <total occurrences of all queries>
```

## Convolution Multi-Processing

The exercise involves performing a convolution operation using multi-processing. The base code is provided, and specific sections related to the core operation are commented out for you to implement. Additionally, you're asked to observe and report the execution time difference between the multi-processing approach and a single-process approach for varying matrix sizes, especially for larger dimensions, and explain why multi-processing may offer better performance in some cases.

## Building Shell

**Problem Statement:**

The goal is to implement a simple shell in C. Most of the code files are provided, and you are only required to implement parts of the `execute_args.c` and `own_cd.c` files. The shell should be able to read user input, parse it, execute entered commands, and handle built-in commands like `exit` and `cd`. Special attention should be paid to the execution of commands in separate processes to avoid terminating the shell after executing a single command.

**Input:** Commands entered by the user in the shell.

**Output:** The shell executes the commands and displays the results. Implementation of the `cd` command and handling of the `exit` command are specifically required.
