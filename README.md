# 'Disarium Numbers' - parallel solutions

'Disarium Numbers' - a program that uses a prime number search algorithm to investigate the dependence of program execution time on the number of processes used.
The program was implemented in C ++ with the use of:
- OpenMP
- MPI 

The algorithm is to find a prime number, which is a natural number greater than 1, which has exactly two natural divisors: one and itself.

The program generates two arrays with 100,000 numbers, the first one contains numbers from 1 to 100,000, while the second table is marked with one-zero, it checks which of the numbers in the array is prime (the value 1 for the index of the number is prime) . On its basis, the program saves the numbers.
