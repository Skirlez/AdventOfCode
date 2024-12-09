#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Util.h"


uint64_t solution_1(const string input) {
	char* str = input.content;
	int unpacked_length = 0;
	for (int i = 0; i < input.size; i++) {
		unpacked_length += (str[i] - '0');
	}

	int16_t* unpacked_str = malloc(unpacked_length * sizeof(int16_t));

	int is_block = 1;
	int16_t id = 0;
	unpacked_length = 0;
	for (int i = 0; i < input.size; i++) {
		int num = (str[i] - '0');
		if (is_block) {
			
			for (int j = 0; j < num; j++) {
				unpacked_str[unpacked_length] = id;
				unpacked_length++;
			}
			id++;
		}
		else {
			for (int j = 0; j < num; j++) {
				unpacked_str[unpacked_length] = -1;
				unpacked_length++;
			}
		}
		is_block = !is_block;
	}

	uint64_t checksum = 0;

	// instead of modifying the strings to move the file blocks just read from the start and when you get to a '.' read from the end
	int start = 0;
	int end = unpacked_length - 1;
	while (start <= end) {
		if (unpacked_str[start] != -1) {
			checksum += start * unpacked_str[start];
		}
		else {
			while (unpacked_str[end] == -1)
				end--;
			if (start > end)
				break;
			checksum += start * unpacked_str[end];
			end--;
		}
		start++;
	}

	return checksum;
}

void print_unpacked_str(int16_t* unpacked_str, int length) {
	for (int i = 0; i < length; i++) {
		if (unpacked_str[i] < 0)
			printf(".");
		else
			printf("%d", unpacked_str[i]);
		
	}
	printf("\n");
}


uint64_t solution_2(const string input) {
	char* str = input.content;
	int unpacked_length = 0;
	for (int i = 0; i < input.size; i++) {
		unpacked_length += (str[i] - '0');
	}

	int16_t* unpacked_str = malloc(unpacked_length * sizeof(int16_t));

	int is_block = 1;
	int16_t id = 0;
	unpacked_length = 0;
	for (int i = 0; i < input.size; i++) {
		int num = (str[i] - '0');
		if (is_block) {
			
			for (int j = 0; j < num; j++) {
				unpacked_str[unpacked_length] = id;
				unpacked_length++;
			}
			id++;
		}
		else {
			for (int j = 0; j < num; j++) {
				unpacked_str[unpacked_length] = -1;
				unpacked_length++;
			}
		}
		is_block = !is_block;
	}

	uint64_t checksum = 0;

	int i = 0;
	int last_index = unpacked_length - 1;
	while (i <= last_index) {
		while (unpacked_str[i] < -1) {
			i++;
		}
		if (unpacked_str[i] >= 0) {
			checksum += i * unpacked_str[i];
			i++;
		}
		else {
			int empty_start = i;
			int empty_count = 1;
			i++;
			while (unpacked_str[i] == -1) {
				empty_count++;
				i++;
			}
			int empty_end = empty_start + empty_count - 1;
			while (unpacked_str[last_index] < 0)
				last_index--;

			int j = last_index;
			while (empty_count > 0 && j > empty_end) {
				int end_number = unpacked_str[j];
				int end_count = 1;
				j--;
				while (unpacked_str[j] == end_number) {
					end_count++;
					j--;
				}
				int number_start = j + 1;
				if (end_count <= empty_count) {
					for (int k = 0; k < end_count; k++) {
						checksum += (empty_start + k) * end_number;
						unpacked_str[number_start + k] = -end_number;
					}
					empty_count -= end_count;
					empty_start += end_count;
					while (unpacked_str[last_index] < 0)
						last_index--;
					j = last_index;
					continue;
				}
				while (unpacked_str[j] < 0)
					j--;
			}
		}
	}
	while (i <= last_index) {
		if (unpacked_str[i] > 0) {
			checksum += i * unpacked_str[i];
		}
		i++;
	}
	return checksum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input_no_trailing_newline(9, argv);
	
	printf("Part 1: %llu\n", solution_1(input));
	time_function_and_print((SolutionFunction)solution_1, input, 1000);

	printf("Part 2: %llu\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 1000);

	free(input.content);
	return 0;
}


