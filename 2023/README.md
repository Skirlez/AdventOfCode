## Performant Advent of Code 2023 solutions written in C++

Each file expects to be given a directory which holds "n.txt", the input file for day n, as a console argument.

Each file will print the answer and the average time to solve for both part 1 and 2. Each year folder contains a table with the results.

Timing the solutions works by measuring how long the solution function takes to run some amount of times, divided by that amount (usually 10,000).
The functions will always recieve the input as a string, and how they parse it will add to the total time.
Reading the file from disk is done outside of the solution functions on purpose, and the time it takes is not counted.

Some rules I'm setting up for myself:
Solution functions cannot do input file specific optimizations, like storing the position of a character without reading the string,
or storing the width of the input (distance to newline) without parsing the string. They are made to be general, and should work with everyone's inputs and example inputs 
(unless supporting the example would hurt performance, see Day 20).

They also cannot use any information/properties that aren't explicity given, unless the property being exploited is intentionally and sneakily inserted into all input files (see 2023 Day 8 Part 2).

To run: I usually build with GCC, C++23, compiling both the .cpp of the day and Util.cpp together. When timing the solutions, I use -Ofast as a compiler argument.

Timings on my CPU (Ryzen 7 3700X): 
|Day|Part 1 (μs)|Part 2 (μs)|Notes|
|-|-|-|-|
|01|91|137||
|02|14|15||
|03|33|18||
|04|117|119||
|05|37|43||
|06|0.041|0.033||
|07|375|376||
|08|26|183||
|09|280|280||
|10|68|265||
|11|61|61||
|12|950|56125|Part 2 solution relies on __int128_t|
|13|25|27||
|14|21|24103|Part 2 time varies a LOT depending on input|
|15|25|120||
|16|31|7873||
|17|47137|961989||
|18|1.8|3.6||
|19|192|190||
|20|2546|9658||

Give or take 10 μs for results above 200 μs.