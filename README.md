My solutions for Advent of Code. 2023 is unfinished, and solutions are written in C++. 2024 solutions written in C.
As my timezone does not allow me to participate in the leaderboards, I have taken upon a different challenge: All solutions are written to be as performant as possible, at all costs. 
(They are then awkwardly modified to be less ugly, sacrificing like 1 or 2 microseconds for readability and size)

Each file expects to be given a directory which holds "n.txt", the input file for day n, as a console argument.

Each file will print the answer and the average time to solve for both part 1 and 2. Each year folder contains a table with the results.

Timing the solutions works by measuring how long the solution function takes to run some amount of times, divided by that amount (usually 10,000).
The functions will always recieve the input as a string, and how they parse it will add to the total time.
Reading the file from disk is done outside of the solution functions on purpose, and the time it takes is not counted.

Solution functions cannot do input file specific optimizations, like storing the position of a character without reading the string,
or storing the width of the input (distance to newline) without parsing the string. They are made to be general, and should work with everyone's inputs and example inputs (unless supporting the example would hurt performance, see 2023 Day 20).

They also cannot use any information/properties that aren't explicity given, unless the property being exploited is intentionally and sneakily inserted into all input files (see 2023 Day 8 Part 2).

To run: I usually build with GCC, C++23, compiling both the .cpp of the day and Util.cpp together. When timing the solutions, I use -Ofast as a compiler argument.

