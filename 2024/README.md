## Performant Advent of Code 2024 solutions written in C

Each file expects to be given a directory which holds "n.txt", the input file for day n, as a console argument.

Each file will print the answer and the average time to solve for both part 1 and 2. Each year folder contains a table with the results.

Timing the solutions works by measuring how long the solution function takes to run some amount of times, divided by that amount (usually 10,000).
The functions will always recieve the input as a string, and how they parse it will add to the total time.
Reading the file from disk is done outside of the solution functions on purpose, and the time it takes is not counted.
[Epoch](https://github.com/seanjackson6325/Epoch) is used to measure the time. I had to add a few casts to its source to make it work for some reason, but it seems to work, and will likely work on Linux as well.

Some rules I'm setting up for myself:
Solution functions must work with the example input.
In general, properties of the input should be calculated when it makes sense.
For example, the width/height of it, if it's rectangular, should be computed in real time.

To run: I usually build with GCC, C17, compiling both the .c of the day, Util.c and epoch.c. When timing the solutions, I use -Ofast as a compiler argument.

Timings on my CPU (Ryzen 7 3700X): 
|Day|Part 1 (μs)|Part 2 (μs)|Notes|
|-|-|-|-|
|01|112|93||
|02|26|150| Makes an assumption that might not be true for all inputs - potentially 180μs |
|03|12|12||
|04|199|67||
|05|9|39||

Give or take 10 μs for results above 200 μs.

