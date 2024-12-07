#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Util.h"


int is_equation_valid(int64_t target, int* arr, int last_element_index) {
	if (last_element_index == 0)
		return (target == arr[0]);
	if (target <= 0)
		return 0;
	int last_element = arr[last_element_index];
	if (target % last_element == 0) {
		int val = is_equation_valid(target / last_element, arr, last_element_index - 1);
		if (val)
			return 1;
	}
	return is_equation_valid(target - last_element, arr, last_element_index - 1);
}


int64_t solution_1(const string input) {
	char* str = input.content;

	int array_size = 1;
	int* arr = malloc(array_size * sizeof(int));
	
	int64_t sum = 0;

	int i = 0;
	while (i < input.size) {
		int last_element_index = -1;

		int64_t target = 0;
		while (str[i] != ':') {
			target = target * 10 + (str[i] - '0');
			i++;
		}
		i += 1; // skip colon
		while (str[i] != '\n') {
			int number = 0;
			i++;
			while (str[i] != ' ' && str[i] != '\n') {
				number = number * 10 + (str[i] - '0');
				i++;
			}
			if (last_element_index == array_size - 1) {
				array_size *= 2;
				arr = realloc(arr, array_size * sizeof(int));
			}
			last_element_index++;
			arr[last_element_index] = number;
			
		}
		
		if (is_equation_valid(target, arr, last_element_index))
			sum += target;
		i++;
	}
	free(arr);
	return sum;
}

int is_equation_valid_concat(int64_t target, int* arr, int last_element_index) {
	if (last_element_index == 0)
		return (target == arr[0]);
	if (target <= 0)
		return 0;
	int last_element = arr[last_element_index];

	if (target % last_element == 0) {
		int val = is_equation_valid_concat(target / last_element, arr, last_element_index - 1);
		if (val)
			return 1;
	}
	int val = is_equation_valid_concat(target - last_element, arr, last_element_index - 1);
	if (val)
		return 1;
	if ((target - last_element) % 10 != 0)
		return 0;
	while (last_element > 0) {
		last_element /= 10;
		target /= 10;
	}

	return is_equation_valid_concat(target, arr, last_element_index - 1);
}


int64_t solution_2(const string input) {
	char* str = input.content;

	int array_size = 1;
	int* arr = malloc(array_size * sizeof(int));
	
	int64_t sum = 0;

	int i = 0;
	while (i < input.size) {
		int last_element_index = -1;

		int64_t target = 0;
		while (str[i] != ':') {
			target = target * 10 + (str[i] - '0');
			i++;
		}
		i += 1; // skip colon
		while (str[i] != '\n') {
			int number = 0;
			i++;
			while (str[i] != ' ' && str[i] != '\n') {
				number = number * 10 + (str[i] - '0');
				i++;
			}
			if (last_element_index == array_size - 1) {
				array_size *= 2;
				arr = realloc(arr, array_size * sizeof(int));
			}
			last_element_index++;
			arr[last_element_index] = number;
			
		}
		
		if (is_equation_valid_concat(target, arr, last_element_index))
			sum += target;
		i++;
	}
	free(arr);
	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(7, argv);
	
	printf("Part 1: %lld\n", solution_1(input));
	time_function_and_print((SolutionFunction)solution_1, input, 1000);

	printf("Part 2: %lld\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 1000);

	free(input.content);
	return 0;
}


