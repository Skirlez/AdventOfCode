#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Util.h"

typedef struct {
	int area;
	int perimeter;
} shape_data;


shape_data empty_data = { 0, 0 };
shape_data perimeter = { 0, 1 };
static inline shape_data explore_area(char* str, int size, int pos, char c, int line_length, uint16_t area_index, uint16_t* area_id_map) {
	if (area_id_map[pos] == area_index)
		return empty_data;
	if (str[pos] != c)
		return perimeter;
	if (area_id_map[pos] == 0) {
		area_id_map[pos] = area_index;
	}

	int directions[] = {1, line_length, -1, -line_length};
	shape_data data = {1, 0};
	for (int dir_index = 0; dir_index < 4; dir_index++) {
		int new_pos = pos + directions[dir_index];
		if (new_pos < 0 || new_pos >= size || str[new_pos] == '\n') {
			data.perimeter++;
			continue;
		}
		shape_data new_data = explore_area(str, size, new_pos, c, line_length, area_index, area_id_map);
		data.area += new_data.area;
		data.perimeter += new_data.perimeter;
	}
	return data;
}

int solution_1(const string input) {
	char* str = input.content;
	uint16_t* area_id_map = calloc(input.size, sizeof(uint16_t));
	uint16_t last_area_id = 0;

	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int sum = 0;
	for (int pos = 0; pos < input.size; pos++) {
		if (str[pos] == '\n')
			continue;
		if (area_id_map[pos] == 0) {
			last_area_id++;
			shape_data data = explore_area(str, input.size, pos, str[pos], line_length, last_area_id, area_id_map);
			sum += data.perimeter * data.area;
		}
	}

	free(area_id_map);
	return sum;
}



static inline int explore_area_only_area(char* str, int size, int pos, char c, int line_length, uint16_t area_index, uint16_t* area_id_map) {
	area_id_map[pos] = area_index;
	int directions[] = {1, line_length, -1, -line_length};
	int area = 1;
	for (int dir_index = 0; dir_index < 4; dir_index++) {
		int new_pos = pos + directions[dir_index];
		if (new_pos < 0 || new_pos >= size)
			continue;
		if (str[new_pos] != c)
			continue;
		if (area_id_map[new_pos] == area_index)
			continue;
		area += explore_area_only_area(str, size, new_pos, c, line_length, area_index, area_id_map);
	}
	return area;
}


static inline int is_area_in_position(int pos, uint16_t area_index, uint16_t* area_id_map,  int size) {
	if (pos < 0 || pos >= size)
		return 0;
	return (area_id_map[pos] == area_index);
}

static inline int explore_area_sides(char* str, int size, int pos, char c, int line_length, uint16_t area_index, uint16_t* area_id_map) {
	int initial_pos = pos;

	int directions[] = {1, line_length, -1, -line_length};
	int dir_index = 0;
	int initial_dir_index = dir_index;
	int turns = 0;

	do {
		int new_pos = pos + directions[dir_index];
		if (is_area_in_position(new_pos, area_index, area_id_map, size)) {
			dir_index = (dir_index - 1) & 3;
			turns++;
			continue;
		}

		int look_into = new_pos + directions[(dir_index + 1) & 3];
		if (!is_area_in_position(look_into, area_index, area_id_map, size)) {
			dir_index = (dir_index + 1) & 3;
			turns++;
			pos = look_into;
		}
		else
			pos = new_pos;
	} while (pos != initial_pos || dir_index != initial_dir_index);

	return turns;
}

/*
static inline shape_data explore_area_2(char* str, int size, int pos, int* allowed_directions, char c, int line_length, uint16_t area_index, uint16_t* area_id_map) {
	area_id_map[pos] = area_index;

	int directions[] = {1, line_length, -1, -line_length};
	int allowed[] = {1, 1, 1, 1};
	shape_data data = {1, 0};

	int new_allowed_directions[] = {1, 1, 1, 1};

	for (int dir_index = 0; dir_index < 4; dir_index++) {
		int new_pos = pos + directions[dir_index];
		if (new_pos < 0 || new_pos >= size || str[new_pos] == '\n' || str[new_pos] != c) {
			if (allowed_directions[dir_index]) {
				data.perimeter++;
			}
			new_allowed_directions[dir_index] = 0;
			allowed[dir_index] = 0;
		}

	}

	for (int dir_index = 0; dir_index < 4; dir_index++) {
		if (!allowed[dir_index])
			continue;
		int new_pos = pos + directions[dir_index];
		if (area_id_map[new_pos] == area_index)
			continue;
		shape_data new_data = explore_area_2(str, size, new_pos, new_allowed_directions, c, line_length, area_index, area_id_map);
		data.area += new_data.area;
		data.perimeter += new_data.perimeter;
	}
	return data;
}
*/


// UNFINISHED!
int solution_2(const string input) {
	char* str = input.content;
	uint16_t* area_id_map = calloc(input.size, sizeof(uint16_t));
	uint16_t last_area_id = 0;

	

	int line_length = first_occurrence_of_char(str, '\n') + 1;

	int sum = 0;
	for (int pos = 0; pos < input.size; pos++) {
		if (str[pos] == '\n')
			continue;
		if (area_id_map[pos] == 0) {
			last_area_id++;
			int area = explore_area_only_area(str, input.size, pos, str[pos], line_length, last_area_id, area_id_map);
			//int sides = explore_area_all_sides(str, input.size, pos, str[pos], line_length, last_area_id, area_id_map);
			//sum += sides * area;
			//printf("Char: %c, area: %d, sides: %d\n", str[pos], area, sides);
		}
	}

	free(area_id_map);
	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(12, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	//printf("Part 2: %d\n", solution_2(input));
	//time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


