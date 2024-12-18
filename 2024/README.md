## Performant Advent of Code 2024 solutions written in C

Each code file expects to be given a directory which holds "n.txt", the input file for day n, as a console argument.

Each code file will print the answer and the time to compute the answers for both part 1 and 2. Each year folder contains a table with the results.

Timing the solutions works by running the solution function some amount of times (usually 10,000) and dividing the time elapsed by that amount.
The functions will always recieve the input as a string, and how they parse it will add to the total time.
Reading the file from disk is done outside of the solution functions on purpose, and the time it takes is not counted.
[Epoch](https://github.com/seanjackson6325/Epoch) is used to measure the time. I had to add a few casts to its source to make 
it work for some reason, but it seems to work now, and will likely work on Linux as well.

Some rules I'm setting up for myself:
Solution functions must work with the example input.
In general, properties of the input should be calculated when it makes sense, rather than embedded in the solution function.
For example, the width/height of the input, if it's rectangular, should be computed in real time.

To run: I usually build with GCC, C17, compiling both the .c of the day, Util.c and epoch.c. When timing the solutions, I use -Ofast as a compiler argument.

Timings on my CPU (Ryzen 7 3700X): 
|Day|Part 1 (μs)|Part 2 (μs)|Notes|
|-|-|-|-|
|01|118|95||
|02|26|120| Makes an assumption that might not be true for all inputs - Part 2 potentially 140μs |
|03|12|12||
|04|200|70||
|05|9|39||
|06|7|1220||
|07|154|253||
|08|13|14||
|09|24|12161|Part 2 could likely be optimized...|
|10|22|23||
|11|545|490588|NOT final! Could be greatly improved with a hash map|
|12|400|TBD||
|13|4|5||
|14|4|120|TODO: Verify correctness of part 2 for other inputs|
|15|86|97||
|16|635|654||

Solutions with longer runtimes tend to deviate more, so give or take a few microseconds for large numbers in this table.