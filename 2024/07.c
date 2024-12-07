#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Util.h"


int is_equation_valid(uint64_t target, uint64_t first_element, int* arr, int size) {
	if (size == 0)
		return (target == first_element);
	if (target < first_element)
		return 0;
	int val = is_equation_valid(target, first_element * (uint64_t)arr[0], &arr[1], size - 1);
	if (val)
		return 1;
	return is_equation_valid(target, first_element + (uint64_t)arr[0], &arr[1], size - 1);
}


uint64_t solution_1(const string input) {
	char* str = input.content;

	int array_size = 1;
	int* arr = malloc(array_size * sizeof(int));
	
	uint64_t sum = 0;

	int i = 0;
	while (i < input.size) {
		int last_element_index = -1;

		uint64_t target = 0;
		while (str[i] != ':') {
			target = target * 10 + (str[i] - '0');
			i++;
		}
		i += 1; // skip colon
		while (str[i] != '\n') {
			int number = 0;
			i++;
			while (str[i] != ' ' && str[i] != '\n') {
				number = number * 10 +(str[i] - '0');
				i++;
			}
			if (last_element_index == array_size - 1) {
				array_size *= 2;
				arr = realloc(arr, array_size * sizeof(int));
			}
			last_element_index++;
			arr[last_element_index] = number;
			
		}
		if (is_equation_valid(target, arr[0], &arr[1], last_element_index + 1 - 1)) // + 1 because we want the size, but -1 because it's the size starting from the second index
			sum += target;
		i++;
	}
	free(arr);
	return sum;
}

int is_equation_valid_concat(uint64_t target, uint64_t first_element, int* arr, int size) {
	if (size == 0)
		return (target == first_element);
	if (target < first_element)
		return 0;
	int val = is_equation_valid_concat(target, first_element * (uint64_t)arr[0], &arr[1], size - 1);
	if (val)
		return 1;
	val = is_equation_valid_concat(target, first_element + (uint64_t)arr[0], &arr[1], size - 1);
	if (val)
		return 1;
	int digits = 0;
	int second_element = arr[0];
	while (second_element > 0) {
		second_element /= 10;
		first_element *= 10;
	}
	return is_equation_valid_concat(target, first_element + (uint64_t)arr[0], &arr[1], size - 1);
}


uint64_t solution_2(const string input) {
	char* str = input.content;

	int array_size = 1;
	int* arr = malloc(array_size * sizeof(int));
	
	uint64_t sum = 0;

	int i = 0;
	while (i < input.size) {
		int last_element_index = -1;

		uint64_t target = 0;
		while (str[i] != ':') {
			target = target * 10 + (str[i] - '0');
			i++;
		}
		i += 1; // skip colon
		while (str[i] != '\n') {
			int number = 0;
			i++;
			while (str[i] != ' ' && str[i] != '\n') {
				number = number * 10 +(str[i] - '0');
				i++;
			}
			if (last_element_index == array_size - 1) {
				array_size *= 2;
				arr = realloc(arr, array_size * sizeof(int));
			}
			last_element_index++;
			arr[last_element_index] = number;
			
		}
		if (is_equation_valid_concat(target, arr[0], &arr[1], last_element_index + 1 - 1)) // + 1 because we want the size, but -1 because it's the size starting from the second index
			sum += target;
		i++;
	}
	free(arr);
	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(7, argv);
	
	printf("Part 1: %llu\n", solution_1(input));
	time_function_and_print((SolutionFunction)solution_1, input, 10000);

	printf("Part 2: %llu\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 100);

	free(input.content);
	return 0;
}


