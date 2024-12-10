#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Util.h"



// In this puzzle, we occassionally need to sum a sequence like 77777 by going over each digit and multiplying it by the position.
// You can do a bit of algebra to avoid the loop entirely.
// Let n be the number/id (7), p be the position, and the length of the sequence be l (5).

// sum = n * pos + n * (pos + 1) + n * (pos + 2)... + n * (pos + l - 1)
// = n * (pos + pos + 1 + pos + 2... + pos + l - 1)
// = n * (l * pos + 1 + 2 ... + l - 1)
// We can substitute the addition of numbers 1 - l - 1 with the formula for summing up an arithmetic sequence,
// with l - 1 elements, where a1 = 1, and an = (l - 1)
// = n * (l * pos + ((1 + l - 1) * (l - 1)) / 2)
// Simplified:
// sum = n * (l * pos + (l * (l - 1)) / 2)

// Note: Can also be simplified to:
// sum = n * l * (pos + (l - 1) / 2)
// But having (l - 1) / 2 is an issue with integers because l can be 2 which will make this 1 / 2 -> 0.
// The other form doesn't have this issue, since l * (l - 1) is guaranteed to be even.

// I hope this makes it easier to understand what exactly is being done here.

int64_t solution_1(const string input) {
	char* str = input.content;
	char* copy = malloc(input.size);
	memcpy(copy, str, input.size);

	int64_t last_id = input.size / 2;
	int64_t sum = 0;
	int64_t id = 0;

	int pos = 0;
	int end = input.size - 1;
	int i = 0;

	while (i <= end) {
		int num = (copy[i] - '0');
		sum += id * (num * pos + (num * (num - 1)) / 2);
		pos += num;
		id++;

		i++;

		num = (copy[i] - '0');
		int amount_needed = num;
		while (amount_needed > 0 && end > i) {
			int end_num = (copy[end] - '0');
			if (end_num <= amount_needed) {
				sum += last_id * (end_num * pos + (end_num * (end_num - 1)) / 2);
				end -= 2;
				amount_needed -= end_num;
				pos += end_num;
				last_id--;
			}
			else {
				copy[end] = (end_num - amount_needed + '0');
				sum += last_id * (amount_needed * pos + (amount_needed * (amount_needed - 1)) / 2);
				pos += amount_needed;
				amount_needed = 0;
			}
		}


		i++;

	}

	free(copy);
	return sum;
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
	
	printf("Part 1: %lld\n", solution_1(input));
	time_function_and_print((SolutionFunction)solution_1, input, 10000);

	printf("Part 2: %llu\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 1000);

	free(input.content);
	return 0;
}


