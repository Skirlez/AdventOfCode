My solutions for Advent of Code. Contains 2023 as of now, the entirety of which is written in C++.
As my timezone does not allow me to participate in the leaderboards, I have taken upon a different challenge: All solutions are written to be as performant as possible, at all costs. 
(They are then awkwardly modified to be less ugly, sacrificing like 1 or 2 microseconds for readability and size)

Each file expects to be given a directory which holds "n.txt", the input file for day n, as a console argument.

Each file will print the answer and the average time to solve for both part 1 and 2. Timing the solutions works by checking how long the function takes to run some amount of times, divided by that amount (usually 10,000).

To run: I usually build with GCC, compiling the .cpp of the day with Util.cpp included. When timing the solutions, I use -Ofast as a compiler argument.