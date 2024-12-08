#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Util.h"


static inline int is_antinode_created(int8_t char_1_x, int8_t char_1_y, int8_t char_2_x, int8_t char_2_y, char* str, int width, int height) {
	int antinode_x = char_2_x + char_2_x - char_1_x;
	if (antinode_x < 0 || antinode_x >= width) 
		return 0;
	int antinode_y = char_2_y + char_2_y - char_1_y;
	if (antinode_y < 0 || antinode_y >= height) 
		return 0;
	if (str[antinode_x + antinode_y * (width + 1)] == '#')
		return 0;
	str[antinode_x + antinode_y * (width + 1)] = '#';
	return 1;
	
}

static inline int8_t** map_out_characters(char* str, int size) {
	// this array is used like a map; indexing it with a character will give you an array with all the positions of this character in the input.
	int8_t** character_arr_map = malloc(256 * sizeof(int8_t*)); 
	for (int i = 0; i < 256; i++) {
		character_arr_map[i] = malloc(size * sizeof(int8_t));
		character_arr_map[i][0] = 0;
	}
	int i = 0;
	int8_t x = 0;
	int8_t y = 0;
	while (i < size) {
		char c = str[i];
		if (c == '.') {
			x++;
			i++;
			continue;
		}
		if (c == '\n') {
			x = 0;
			y++;
			i++;
			continue;
		}
		int8_t* character_arr = character_arr_map[(unsigned int)c];

		int size = character_arr[0];
		character_arr[size * 2 + 1] = x;
		character_arr[size * 2 + 2] = y;
		character_arr[0]++;

		i++;
		x++;
	}
	return character_arr_map;
}
static inline void free_character_array_map(int8_t** character_arr_map) {
	for (int i = 0; i < 256; i++) {
		free(character_arr_map[i]);
	}
	free(character_arr_map);
}

int solution_1(const string input) {
	char* str = input.content;
	char* copy = malloc(input.size + 1);
	strcpy(copy, str);

	int line_length = first_occurrence_of_char(str, '\n') + 1;
	
	int width = line_length - 1;
	int height = input.size / line_length;

	int8_t** character_arr_map = map_out_characters(str, input.size);
	
	int count = 0;
	for (int i = 0; i < 256; i++) {
		int8_t* character_arr = character_arr_map[i];
		int size = character_arr[0];
		if (size == 0)
			continue;
		character_arr++; // make array start from first element
		// iterate on all the pairs
		for (int j = 0; j < size - 1; j++) {
			int8_t char_1_x = character_arr[j * 2];
			int8_t char_1_y = character_arr[j * 2 + 1];
			for (int k = j + 1; k < size; k++) {
				int8_t char_2_x = character_arr[k * 2];
				int8_t char_2_y = character_arr[k * 2 + 1];
				count += is_antinode_created(char_1_x, char_1_y, char_2_x, char_2_y, copy, width, height);
				count += is_antinode_created(char_2_x, char_2_y, char_1_x, char_1_y, copy, width, height);
			}
		} 
	}
	free_character_array_map(character_arr_map);
	free(copy);
	return count;
}

// returns how many antinodes are created from antenna 1 to antenna 2
static inline int create_pair_antinodes_p2(int8_t char_1_x, int8_t char_1_y, int8_t char_2_x, int8_t char_2_y, char* str, int width, int height) {

	int count = 0;

	int antinode_x = char_2_x;
	int antinode_y = char_2_y;
	int diff_x = char_2_x - char_1_x;
	int diff_y = char_2_y - char_1_y;
	do {
		antinode_x += diff_x;
		antinode_y += diff_y;
		if (antinode_x < 0 || antinode_x >= width) 
			break;
		if (antinode_y < 0 || antinode_y >= height) 
			break;
		if (str[antinode_x + antinode_y * (width + 1)] == '#')
			continue;
		str[antinode_x + antinode_y * (width + 1)] = '#';
		count++;
	} while (1);

	return count;
}


int solution_2(const string input) {
	char* str = input.content;
	char* copy = malloc(input.size + 1);
	strcpy(copy, str);

	int line_length = first_occurrence_of_char(str, '\n') + 1;
	
	int width = line_length - 1;
	int height = input.size / line_length;

	int8_t** character_arr_map = map_out_characters(str, input.size);
	
	int count = 0;
	for (int i = 0; i < 256; i++) {
		int8_t* character_arr = character_arr_map[i];
		int size = character_arr[0];
		if (size == 0)
			continue;
		character_arr++; // make array start from first element
		// iterate on all the pairs
		for (int j = 0; j < size - 1; j++) {
			int8_t char_1_x = character_arr[j * 2];
			int8_t char_1_y = character_arr[j * 2 + 1];

			// I'm really not sure why, but it seems like in part 2, every single antenna also counts as an antinode?
			// The puzzle is a bit confusing, but they seem to count as in line with two antennas (I assume themselves?)
			if (copy[char_1_x + char_1_y * (width + 1)] != '#') {
				copy[char_1_x + char_1_y * (width + 1)] = '#';
				count++;
			}

			for (int k = j + 1; k < size; k++) {
				int8_t char_2_x = character_arr[k * 2];
				int8_t char_2_y = character_arr[k * 2 + 1];

				// This is here for the same reason as above
				if (copy[char_2_x + char_2_y * (width + 1)] != '#') {
					copy[char_2_x + char_2_y * (width + 1)] = '#';
					count++;
				}

				count += create_pair_antinodes_p2(char_1_x, char_1_y, char_2_x, char_2_y, copy, width, height);
				count += create_pair_antinodes_p2(char_2_x, char_2_y, char_1_x, char_1_y, copy, width, height);
			}
		} 
	}

	free_character_array_map(character_arr_map);
	free(copy);
	return count;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(8, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


