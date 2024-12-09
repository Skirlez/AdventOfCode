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
		if (unpacked_str[i] == -1)
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


	int last_contiguous_index = 0;
	int last_index = unpacked_length - 1;

	while (last_index > last_contiguous_index) {
		int16_t number = unpacked_str[last_index];
		int count = 1;
		last_index--;
		while (unpacked_str[last_index] == number) {
			count++;
			last_index--;
		}
		// found block we want to move
		// start of the block we want to move (on the right)
		int start_right_side = last_index + 1;

		while (unpacked_str[last_contiguous_index + 1] != -1)
			last_contiguous_index++;
		int i = last_contiguous_index + 1;
		while (i < last_index) {
			int empty_count = 0;
			// start of the empty space we potentially move to (on the left)
			int start_left_side = i;
			while (empty_count < count && unpacked_str[i] == -1) {
				i++;
				empty_count++;
			}
			if (empty_count == count) {
				for (int j = 0; j < empty_count; j++) {
					unpacked_str[start_left_side + j] = number;
					unpacked_str[start_right_side + j] = -1;
				}
				break;
			}
			while (unpacked_str[i] != -1)
				i++;
		}
		while (unpacked_str[last_index] == -1)
			last_index--;
	}

	
	int pos = 0;
	int i = 0;
	while (i < unpacked_length) {
		if (unpacked_str[i] != -1) {
			checksum += i * unpacked_str[i];
			pos++;
		}
		i++;
	}
	//print_unpacked_str(unpacked_str, unpacked_length);
	

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


