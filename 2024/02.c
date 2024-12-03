#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Util.h"

// Solution 1 works out the solution as it parses without an intermediary structure -
// This may be possible with solution 2, but it would be really, really messy. So we use arrays there.

int read_next_number(char* str, int* indexPtr) {
	int multiplier = 1;
	int num = 0;

	int index = *indexPtr;
	while (str[index] != ' ' && str[index] != '\n' && index >= 0) {
		num += (str[index] - '0') * multiplier;
		multiplier *= 10;
		index--;
	}
	*indexPtr = index;
	return num;
}


static inline int are_numbers_valid_neighbors(int number, int previousNumber, int direction) {
	return number != previousNumber && abs(previousNumber - number) <= 3 && ((((previousNumber - number) > 0) ? 1 : -1) == direction);
}

static inline int is_line_valid(char* str, int start, int end) {
	int i = end;
	int previousNumber = read_next_number(str, &i);
	i--;
	int number = read_next_number(str, &i);
	i--;
	int direction = ((previousNumber - number) > 0) ? 1 : -1;
	if (!are_numbers_valid_neighbors(number, previousNumber, direction))
		return 0;

	while (i >= start) {
		previousNumber = number;
		number = read_next_number(str, &i);
		i--;
		if (!are_numbers_valid_neighbors(number, previousNumber, direction)) {
			return 0;
		}
	}
	return 1;
}


int solution_1(string input) {
	char* str = input.content;
	int i = 0;
	int count = 0;
	while (i < input.size) {
		int start = i;
		i = first_occurrence_of_char_offset(str, start, '\n') + 1;
		int end = i - 2;
		count += is_line_valid(str, start, end);
	}
	return count;
}



static inline int* convert_line_to_array(char* str, int start, int* out_size) {
	int i = start;
	int number_amount = 1;
	while (str[i] != '\n') {
		number_amount += (str[i] == ' ');
		i++;
	}
	int* arr = (int*)malloc(number_amount * sizeof(int));
	i = start;
	int number_index = 0;
	while (number_index < number_amount) {
		int number = 0;
		while (str[i] != ' ' && str[i] != '\n') {
			number = number * 10 + (str[i] - '0');
			i++;
		}
		i++;
		arr[number_index] = number;
		number_index++;
	}
	*out_size = number_amount;
	return arr;
}


static inline int is_line_valid_array(int* arr, int size) {
	int direction = (arr[0] - arr[1] > 0) ? 1 : -1;
	for (int i = 0; i < size - 1; i++) {
		int new_direction = (arr[i] - arr[i + 1] > 0) ? 1 : -1;
		if (new_direction != direction || arr[i] == arr[i + 1] || abs(arr[i] - arr[i + 1]) > 3)
			return 0;
	}
	return 1;
}


static inline void copy_array_from_array_except(int* dest, int* src, int source_size, int index_except) {
	memcpy(dest, src, index_except * sizeof(int));
	memcpy(dest + index_except, src + index_except + 1, (source_size - index_except - 1) * sizeof(int));
}

static inline int is_line_valid_tolerant(char* str, int start) {
	int size;
	int* arr = convert_line_to_array(str, start, &size);

	int direction = (arr[0] - arr[1] > 0) ? 1 : -1;
	for (int i = 0; i < size - 1; i++) {
		int new_direction = (arr[i] - arr[i + 1] > 0) ? 1 : -1;
		if (new_direction != direction || arr[i] == arr[i + 1] || abs(arr[i] - arr[i + 1]) > 3) {
			// This sequence is unsafe. Try removing this index or the index after.

			int* arr_1 = (int*)malloc((size - 1) * sizeof(int));
			copy_array_from_array_except(arr_1, arr, size, i);
			int value_1 = is_line_valid_array(arr_1, size - 1);
			free(arr_1);
			if (value_1) {
				free(arr);
				return 1;
			}
			int* arr_2 = (int*)malloc((size - 1) * sizeof(int));
			copy_array_from_array_except(arr_2, arr, size, i + 1);
			int value_2 = is_line_valid_array(arr_2, size - 1);
			free(arr_2);

			free(arr);
			return value_2;

			/* 
			Seems like they made sure a sequence like:
			37 40 37 36 33 32 29 27
			cannot appear. If it existed, we would have wrongly counted it as unsafe, when it
			can be made safe by removing 37. Didn't consider if you read the lines backwards though!

			It's also possible they made a sequence like this appear on purpose, but backwards,
			I'm only guessing that because this is the first line of my input.

			I'm leaving this here in case other input files do contain such a sequence.
		
			if (value_2) {
				free(arr);
				return 1;
			}

			int* arr_3 = (int*)malloc((size - 1) * sizeof(int));
			copy_array_from_array_except(arr_3, arr, size, 0);
			int value_3 = is_line_valid_array(arr_3, size - 1);
			free(arr_3);
			free(arr);
			return value_3;
			*/
		}
	}
	free(arr);
	return 1;
}


int solution_2(string input) {
	char* str = input.content;
	int i = 0;
	int count = 0;
	while (i < input.size) {
		count += is_line_valid_tolerant(str, i);
		i = first_occurrence_of_char_offset(str, i, '\n') + 1;
	}
	return count;
}



int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(2, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


