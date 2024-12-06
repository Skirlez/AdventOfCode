#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Util.h"



int solution_1(const string input) {
	char* str = input.content;

	int count = 1;
	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int pos = first_occurrence_of_char(str, '^');
	int direction = 1;

	int cos_table[] = {1, 0, -1, 0};
	int sin_table[] = {0, -line_length, 0, line_length};
	
	while (1) {
		int next_pos = pos + cos_table[direction] + sin_table[direction];
		if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
			break;
		if (str[next_pos] == '.') {
			str[next_pos] = 'X';
			count++;
		}
		else if (str[next_pos] == '#') {
			direction = (direction - 1) & 3;
			continue;
		}
		pos = next_pos;
	}
	return count;
}

int solution_2(const string input) {
	char* str = input.content;

	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int pos = first_occurrence_of_char(str, '^');
	int direction = 1;

	// this map will store, for each position, whether or not we have already been there while facing a direction.
	// it will do this by setting the bit with the index of the direction to 1
	uint8_t* been_here_map = calloc(input.size, sizeof(uint8_t));

	// mark that we've been at the start
	been_here_map[pos] |= (1 << direction);

	// this map does the same thing but only while we're simulating an obstacle, and additions to it are temporary only to
	// that specific simulation of the obstacle at that position.
	uint8_t* simulate_been_here_map = malloc(input.size * sizeof(uint8_t));


	int cos_table[] = {1, 0, -1, 0};
	int sin_table[] = {0, -line_length, 0, line_length};
	int count = 0;
	while (1) {
		int next_pos = pos + cos_table[direction] + sin_table[direction];
		if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
			break;
		if (str[next_pos] == '#') {
			direction = (direction - 1) & 3;
			continue;
		}

		uint8_t been_here = been_here_map[next_pos];
		been_here_map[next_pos] |= (1 << direction);
		if (been_here != 0) {
			// we have been to this tile before - meaning we already simulated this obstacle
		}
		else {
			memcpy(simulate_been_here_map, been_here_map, input.size * sizeof(uint8_t));
			
			// simulate what would happen if an obstacle was here
			int keep_direction = direction;
			int keep_next_pos = next_pos;
			str[keep_next_pos] = '#';
	
			direction = (direction - 1) & 3;
			
			while (1) {
				next_pos = pos + cos_table[direction] + sin_table[direction];
				if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
					break; // we've made it out of the lab, so no loop
				if (str[next_pos] == '#') {
					direction = (direction - 1) & 3;
					continue;
				}
				if ((simulate_been_here_map[next_pos] >> direction) & 1) {
					// we made it into a square we've been to already, in a direction we already were - meaning a loop
					count++;
					break;
				}
				simulate_been_here_map[next_pos] |= (1 << direction);
				pos = next_pos;
			}
			
			str[keep_next_pos] = '.';
			next_pos = keep_next_pos;
			direction = keep_direction;
		}
		
		pos = next_pos;
	}
	free(been_here_map);
	free(simulate_been_here_map);
	return count;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(6, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 100);

	free(input.content);
	return 0;
}


