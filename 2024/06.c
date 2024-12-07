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

	// this map will store, for each position, whether or not we simulated an obstacle there or not.
	uint8_t* simulated_obstacle_map = calloc(input.size, sizeof(uint8_t));


	// we will iterate on every square the guard goes to, and for each square check what would happen
	// if we placed an obstacle there.

	// it is a good idea, then, to make sure that the process of simulating what would happen is as fast as possible.
	// to do this, we will store the obstacles in a format that allows us 
	// to go from one to the next in every iteration, rather than traversing square by square.
	
	int width = line_length - 1; // not counting the newline char
	int height = input.size / line_length;

	int** rows = malloc(height * sizeof(int*));
	for (int i = 0; i < height; i++) {
		rows[i] = malloc((width + 1) * sizeof(int));
		rows[i][0] = 0; // size
	}
	int** columns = malloc(width * sizeof(int*));
	for (int i = 0; i < height; i++) {
		columns[i] = malloc((height + 1) * sizeof(int));
		columns[i][0] = 0;
	}


	// this map will store, for each obstacle, whether or not we have encountered it from a certain direction.
	// it will store the incoming direction (before turning).
	// it will do this by setting the bit with the index of the direction to 1.
	uint8_t* been_here_map = calloc(width * height, sizeof(uint8_t));

	// this structure will store changes made to `been_here_map`. it will put the x position of the obstacle changed
	// in the last 8 bytes, then the y position in the next 8 bytes, and the original value in the first 8 bytes.
	// (xxxxxxxx yyyyyyyy 00000000 vvvvvvvv)
	uint32_t* changes_stack = malloc(width * height * sizeof(uint32_t));

	{
		int i = 0;
		int x = 0;
		int y = 0;
		while (i < input.size) {
			if (str[i] == '#') {
				rows[y][rows[y][0] + 1] = x;
				rows[y][0]++;
				columns[x][columns[x][0] + 1] = y;
				columns[x][0]++;
			}
			x++;
			i++;
			if (str[i] == '\n') {
				x = 0;
				y++;
				i++;
			}
		}
	}




	int cos_table[] = {1, 0, -1, 0};
	int sin_table[] = {0, -1, 0, 1};
	int sin_line_table[] = {0, -line_length, 0, line_length};
	int count = 0;

	int x = pos % line_length;
	int y = pos / line_length;

	while (1) {
		int next_pos = pos + cos_table[direction] + sin_line_table[direction];
		int next_x = x + cos_table[direction];
		int next_y = y + sin_table[direction];
		if (next_pos < 0 || next_pos >= input.size || str[next_pos] == '\n')
			break;
		if (str[next_pos] == '#') {
			int index = next_x + next_y * width;
			been_here_map[index] |= (1 << direction);
			direction = (direction - 1) & 3;
			continue;
		}
		if (simulated_obstacle_map[next_pos] == 1) {
			// already simulated this one
		}
		else {
			simulated_obstacle_map[next_pos] = 1;
			int keep_direction = direction;
			// simulate what would happen if an obstacle was here
			int changes_to_map = 0;

			int added_row_index;
			int added_column_index;
			// add the obstacle to the arrays
			{
				int* row = rows[next_y];
				int size = row[0];
				row = &row[1];
				int i = 0;
				while (i < size && row[i] < next_x) {
					i++;
				}
				added_row_index = i + 1; // + 1 to adjust for full array
				memmove(&row[i + 1], &row[i], (size - i) * sizeof(int));
				row[i] = next_x;
				row[-1]++;
			}
			{
				int* column = columns[next_x];
				int size = column[0];
				column = &column[1];
				int i = 0;
				while (i < size && column[i] < next_y) {
					i++;
				}
				added_column_index = i + 1;
				memmove(&column[i + 1], &column[i], (size - i) * sizeof(int));
				column[i] = next_y;
				column[-1]++;
			}

			direction = (direction - 1) & 3;
			
			while (1) {
				int** obstacles_for_direction[] = {rows, columns, rows, columns};
				int** obstacles_axis = obstacles_for_direction[direction];
				// downwards (direction 3) is considered forwards in this case
				int forward_or_backwards_for_direction[] = {1, -1, -1, 1};

				int forwards = forward_or_backwards_for_direction[direction];

				int* axis_per_direction[] = {&x, &y, &x, &y};
				int* axis = axis_per_direction[direction]; 
				int* other_axis = axis_per_direction[(direction + 1) & 3];
				
				int* obstacles = obstacles_axis[*other_axis];
				int size = obstacles[0];
				if (size == 0)
					break;
				// get only the rest of the array
				obstacles = &obstacles[1];

				if (forwards == 1) {
					int i = 0;
					while (i < size && obstacles[i] < *axis)
						i++;
					if (i == size) {
						break; // left the lab
					}
					*axis = obstacles[i];
				}
				else {
					int i = size - 1;
					while (i >= 0 && obstacles[i] > *axis)
						i--;
					if (i == -1) {
						break; // left the lab
					}
					*axis = obstacles[i];
				}
				int index = x + y * width;
				uint8_t been_here = been_here_map[index];
				if ((been_here_map[index] >> direction) & 1) {
					// we have already been to this obstacle, meaning a loop
					count++;
					break;
				}

				been_here_map[index] |= (1 << direction);
				uint32_t change = ((uint8_t)(x) << 24) | ((uint8_t)(y) << 16) | (been_here);
				changes_stack[changes_to_map] = change;
				changes_to_map++;

				// we are currently "inside" the obstacle - take a step back
				direction = (direction - 1) & 3;
				*axis -= forwards;
			}
			// revert all the changes we did to the data structures
			while (changes_to_map != 0) {
				changes_to_map--;
				uint32_t change = changes_stack[changes_to_map];
				uint8_t changed_x = (change >> 24) & 0xFF;
				uint8_t changed_y = (change >> 16) & 0xFF;
				uint8_t original_value = change & 0xFF;
				been_here_map[changed_x + changed_y * width] = original_value;
			}

			{
				int* row = rows[next_y];
				int size = row[0];
				memmove(&row[added_row_index], &row[added_row_index + 1], (size - added_row_index) * sizeof(int));
				row[0]--;
			}
			{
				int* column = columns[next_x];
				int size = column[0];
				memmove(&column[added_column_index], &column[added_column_index + 1], (size - added_column_index) * sizeof(int));
				column[0]--;
			}

			direction = keep_direction;
		}
		x = next_x;
		y = next_y;
		pos = next_pos;
	}
	free(been_here_map);
	free(changes_stack);
	for (int i = 0; i < height; i++) {
		free(rows[i]);
	}
	for (int i = 0; i < width; i++) {
		free(columns[i]);
	}
	free(rows);
	free(columns);
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


