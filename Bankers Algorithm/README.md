# Banker's Algorithm in C

## Introduction
This directory contains a C program that implements the Banker's Algorithm, a classic resource allocation and deadlock avoidance algorithm used in operating systems. The algorithm simulates the allocation of resources to multiple processes, ensuring that the system remains in a safe state and avoiding potential deadlocks.

## Algorithm Overview

The Banker's Algorithm checks and grants resource requests by processes if doing so will leave the system in a safe state. A state is considered safe if there is at least one sequence of all processes wherein each process can request and obtain the maximum demand without causing a deadlock.

## Implementation Details

### Data Structures

- `available[]`: An array that keeps track of the number of available units of each resource.
- `maximum[][]`: A 2D array that represents the maximum demand of each process for each resource.
- `allocation[][]`: A 2D array that tracks the number of units of each resource currently allocated to each process.
- `need[][]`: A 2D array that represents the remaining needs of each process, calculated as the `maximum` minus `allocation`.

### Functions

- `isSafe()`: Determines if the system is in a safe state by trying to find a safe sequence of processes.
- `printMatrix()`: Utility function to print a resource matrix.
- `main()`: Drives the program, takes user input for resource allocation, and checks the system's state.

### User Interaction

The program prompts the user to enter the number of resources and processes. It then takes input for the available resources, the maximum resource requirements for each process, and the currently allocated resources for each process. The `need` matrix is calculated based on the input, and the `isSafe()` function checks if the resulting state is safe.

### Example Usage

To run the program, compile it using a C compiler and execute the resulting binary. Enter the required data as prompted.

```bash
gcc bankers_algorithm.c -o bankers_algorithm
./bankers_algorithm
```

## Safe and Unsafe States

The program accurately determines whether the system is in a safe or unsafe state after processing the user input. It provides immediate feedback on the state of the system based on the Banker's Algorithm safety criteria.

## Conclusion

Understanding the Banker's Algorithm is essential for anyone interested in operating systems or studying resource allocation in concurrent systems. This program provides a hands-on example of how such an algorithm can be implemented and used to ensure system stability and avoid deadlocks.