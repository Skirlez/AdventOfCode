#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

// returns the multiplication of the two numbers in the mul instruction if it exists, 0 otherwise
static inline int try_parse_suspected_mul(char* str, int size, int* indexPtr) {
	// parse first integer
	int left = 0;
	int i = *indexPtr;
	while (i < size) {
		char ch = str[i];
		if (ch == ',')
			break;
		if (ch < '0' || ch > '9') {
			*indexPtr = i;
			return 0;
		}
		left = left * 10 + (ch - '0');
		i++;
	}
	if (i == size)
		return 0;
	// now on the comma, go to next number
	i++;
	int right = 0;
	while (i < size) {
		char ch = str[i];
		if (ch == ')')
			break;
		if (ch < '0' || ch > '9') {
			*indexPtr = i;
			return 0;
		}
		right = right * 10 + (ch - '0');
		i++;
	}
	if (i == size) {
		*indexPtr = i;
		return 0;
	}

	*indexPtr = i;
	return left * right;
}

char mul_chars[] = {'m', 'u', 'l', '('};

int solution_1(const string input) {
	char* str = input.content;
	int sum = 0;
	int mul_progress = 0;
	for (int i = 0; i < input.size; i++) {
		if (mul_progress != 4) {
			// trying to find "mul("
			if (str[i] == mul_chars[mul_progress])
				mul_progress++;
			else
				mul_progress = 0;
		}
		else {
			sum += try_parse_suspected_mul(str, input.size, &i);
			mul_progress = 0;
		}
	}

	return sum;
}


char do_chars[] = {'d', 'o', '(', ')'};
char dont_chars[] = {'d', 'o', 'n', '\'', 't', '(', ')'};

int solution_2(const string input) {
	char* str = input.content;
	int sum = 0;
	int mul_progress = 0;
	int dont_progress = 0;
	int do_progress = 0;
	for (int i = 0; i < input.size; i++) {
		if (dont_progress != 7) {
			// we are enabled - looking for donts and muls
			if (str[i] == dont_chars[dont_progress]) {
				dont_progress++;
			}
			else {
				dont_progress = 0;
				if (mul_progress != 4) {
					// trying to find "mul("
					if (str[i] == mul_chars[mul_progress])
						mul_progress++;
					else
						mul_progress = 0;
				}
				else {
					sum += try_parse_suspected_mul(str, input.size, &i);
					mul_progress = 0;
				}
			}
		}
		else {
			// looking for dos
			if (str[i] == do_chars[do_progress]) {
				do_progress++;
				if (do_progress == 4) {
					// reset (enabled again)
					dont_progress = 0;
					do_progress = 0;
				}
			}
			else
				do_progress = 0;
		}
	}

	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(3, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


