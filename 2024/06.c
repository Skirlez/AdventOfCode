#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Util.h"


int cos_table[] = {1, 0, -1, 0};
int round_cos(int direction) {
	return cos_table[direction];
}
int sin_table[] = {0, -1, 0, 1};
int round_sin(int direction) {
	return sin_table[direction];
}

int solution_1(const string input) {
	char* str = input.content;

	int count = 1;
	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int pos = first_occurrence_of_char(str, '^');
	int direction = 1;

	while (1) {
		int next_pos = pos + round_cos(direction) + round_sin(direction) * line_length;
		if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
			break;
		if (str[next_pos] == '.') {
			str[next_pos] = 'X';
			count++;
		}
		else if (str[next_pos] == '#') {
			direction--;
			if (direction == -1)
				direction = 3;
			continue;
		}
		pos = next_pos;
	}
	return count;
}




int solution_2(const string input) {
	char* str = input.content;

	uint8_t* placed_obstacles_map = calloc(input.size + 1, sizeof(uint8_t));

	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int pos = first_occurrence_of_char(str, '^');
	int direction = 1;
	int start = pos;


	str[pos] = '.';

	pos = start;
	direction = 1;

	uint8_t* been_here_map = calloc(input.size, sizeof(uint8_t));


	int count = 0;
	//time = 1;
	while (1) {
		int next_pos = pos + round_cos(direction) + round_sin(direction) * line_length;
		if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
			break;
		//time++;
		if (str[next_pos] == '#') {
			direction--;
			if (direction == -1)
				direction = 3;
			continue;
		}
		else {
			// if we placed an obstacle in front of us
			int keep_direction = direction;
			int keep_next_pos = next_pos;
			str[keep_next_pos] = '#';
			
			direction = 1;
			pos = start;

			while (1) {
				next_pos = pos + round_cos(direction) + round_sin(direction) * line_length;
				if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
					break;
				if (str[next_pos] == '#') {
					direction--;
					if (direction == -1)
						direction = 3;
					continue;
				}
				else {
					uint8_t been_here = been_here_map[next_pos];
					if ((been_here >> direction) & 1) {
						if (placed_obstacles_map[keep_next_pos] != 1) {
							count++;
							placed_obstacles_map[keep_next_pos] = 1;
						}
						break;
					}
					been_here_map[next_pos] |= (1 << direction);
				}
				pos = next_pos;
			}
			
			str[keep_next_pos] = '.';
			
			//time = keep_time;

			next_pos = keep_next_pos;
			direction = keep_direction;
			
			memset(been_here_map, 0, input.size * sizeof(uint8_t));
		}
		pos = next_pos;
	}

	str[start] = '^';
	free(been_here_map);
	return count;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(6, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10);

	free(input.content);
	return 0;
}


