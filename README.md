# OS-Assignment

## OS-Assignment-1 (Sudoku Solver Using Threads)

- Write a C program to solve an `NxN sudoku puzzle` by brute force. 
- The program should be compatible on an Ubuntu machine running `18.04` or `20.04`. 
- `0 < N <= 36` N is a perfect square. The value of N will be supplied as a command line argument. 
- The grid to be solved will be supplied as a text file as a command line argument. 
- Empty cells in the grid have `0`. Numbers greater than 9 will be represented by the number itself. 
- So, your program should run as : `./sudoku.out N input.txt`
- The program should print the solved grid on the standard output in a tab separated format, similar to the input.
***

- Example of a 9x9 grid
    - <https://drive.google.com/file/d/1zfhPVD80gyxCbGYnvhFfrr3j7X1LFcqu/view?usp=sharing>
- Example of a 25x25 grid: 
    - <https://drive.google.com/file/d/15rSuXJbEtJroHia0D1tNQVz8L-kkxN6O/view?usp=sharing> 
    - <https://drive.google.com/file/d/1592QBBbA1x_E5p8lxZk7hjtx_spfXyF3/view?usp=sharing> 

## OS-Assignment-2
Problem Statements:
- Write a multi-threaded C program (P1) that reads a file containing space-separated positive integers in `ASCII` format. The number of integers can be arbitrarily large (upto `1 million integers`). The name of the file and the number of integers in the file will be supplied in the command line.
    - Different threads should read different parts of the file. Vary the `number of threads` from `1… to arbitrarily large`
    - Record the time that it takes to read the entire file into memory with different number of threads (1, 2, … n)
    - Plot time against the number of threads for different numbers of integers in the input file. Analyse the findings from the plots.
---
- Write a C program (P2) which uses `IPC mechanisms` to receive the numbers read in the program in part (a). This program spawns multiple threads to sum the numbers passed by the program P1 in part (a). The program prints the sum on the terminal.
    - Vary the `number of threads` from `1… to arbitrarily large`. Different threads can calculate the partial sums of the series
    - Record the time it takes to sum the integers with different number of threads
    - Plot the time against the number of threads for different numbers of integers. Analyse the findings from the plots.
---
- Write a scheduler program S. S spawns 2 children processes which exec to become the processes P1 and P2 in part (a) and part (b). S uses some mechanism (e.g. sleep) to simulate a uniprocessor scheduler. That is, it puts Process 1 to sleep and lets Process 2 execute, and vice versa. Simulate the following scheduling algorithms in S:
    - `Round Robin` with time quantum 1 ms
    - `Pre-emptive priority scheduling`, where the priority is a function of the amount of processing time that a process has already received (lesser the processing time received, higher the priority). Priority is updated every 1 ms.
    - Plot the total `turnaround time vs workload size` and `waiting time vs workload size` for the different scheduling algorithms. How do the two algorithms compare for the same workload size? Analyse your findings.
    - What is the switching overhead in the different cases?
- **You might need to take care of race conditions which might arise at different parts of the assignment**
