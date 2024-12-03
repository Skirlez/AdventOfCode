#pragma once
#include <stdio.h>

typedef struct {
	char* content;
	const long size;
} string;

typedef int (*SolutionFunction)(string input);
double time_function(SolutionFunction func, string input, const int iterations);
void time_function_and_print(SolutionFunction func, string input, const int iterations); 
void init_timing();
const string read_input(int day, char* argv[]);

// returns first index of the char `ch`, starting from `index`. assumes `ch` exists
static inline size_t first_occurrence_of_char_offset(const char* str, size_t index, const char ch) {
	while (str[index] != ch)
		index++;
	return index;
}
// returns first index of the char `ch`. assumes `ch` exists
static inline size_t first_occurrence_of_char(const char* str, const char ch) {
	return first_occurrence_of_char_offset(str, 0, ch);
}
