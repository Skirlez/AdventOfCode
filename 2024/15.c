#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "Util.h"

int solution_1(const string input) {
	char* str = input.content;
	int robot_pos = first_occurrence_of_char(str, '@');
	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int i = 0;
	while (str[i] != '\n') {
		i += line_length;
	}
	int warehouse_size = i;


	//'>' - 62
	//'^' - 94
	//'<' - 60
	//'v' - 118

	int move_arr[119];

	move_arr[62] = 1;
	move_arr[94] = -line_length;
	move_arr[60] = -1;
	move_arr[118] = line_length;

	char* copy = malloc(warehouse_size);
	memcpy(copy, str, warehouse_size);

	copy[robot_pos] = '.';
	while (i < input.size) {
		if (str[i] == '\n') {
			i++;
			continue;
		}
		int direction = move_arr[(uint8_t)str[i]];
		i++;

		// can't ever be out of bounds thanks to surrounding walls
		int new_pos = robot_pos + direction;
		if (copy[new_pos] == '#')
			continue; // can't move here
		if (copy[new_pos] == 'O') {
			int new_box_pos = new_pos;
			do {
				new_box_pos += direction;
			} while (copy[new_box_pos] == 'O');
			if (copy[new_box_pos] == '#')
				continue; // can't push the box
			else {
				// push the box
				copy[new_pos] = '.';
				copy[new_box_pos] = 'O';
			}
		}
		robot_pos = new_pos;
	}
	//copy[warehouse_size - 1] = '\0';
	//printf(copy);

	int sum = 0;
	for (int j = 0; j < warehouse_size; j++) {
		if (copy[j] == 'O') {
			int y = j / line_length;
			int x = j % line_length;
			sum += y * 100 + x;
		}
	}

	free(copy);
	return sum;
}

static inline bool is_wide_box(char c) {
	// both '[' and ']' when divided by 8 produce 11 (and none of the other characters we care about do)
	return (c >> 3 == 11);
}

bool is_vertical_push_valid(char* str, int box_pos, int push_direction) {
	int other_pos;
	if (str[box_pos] == '[')
		other_pos = box_pos + 1;
	else
		other_pos = box_pos - 1;
	
	if (str[box_pos + push_direction] == '#' || str[other_pos + push_direction] == '#')
		return false;
	if (str[box_pos + push_direction] == '.') {
		if (str[other_pos + push_direction] == '.')
			return true;
		return is_vertical_push_valid(str, other_pos + push_direction, push_direction);
	}
	// is this box directly above us
	if (str[box_pos + push_direction] == str[box_pos]) {
		return is_vertical_push_valid(str, box_pos + push_direction, push_direction);
	}
	// there is a box in our next position, above/below next_pos - check for other_pos
	if (str[other_pos + push_direction] == '.') {
		return is_vertical_push_valid(str, box_pos + push_direction, push_direction);
	}
	return is_vertical_push_valid(str, box_pos + push_direction, push_direction) 
		&& is_vertical_push_valid(str, other_pos + push_direction, push_direction);
	
}

void push_vertical(char* str, int box_pos, int push_direction) {
	int right_pos;
	int left_pos;
	if (str[box_pos] == ']') {
		right_pos = box_pos;
		left_pos = box_pos - 1;
	}
	else {
		left_pos = box_pos;
		right_pos = box_pos + 1;
	}

	int new_left_pos = left_pos + push_direction;

	// box right in front
	if (str[new_left_pos] == '[') {
		push_vertical(str, new_left_pos, push_direction);
		str[left_pos] = '.';
		str[right_pos] = '.';
		str[new_left_pos] = '[';
		str[new_left_pos + 1] = ']';
		return;
	}
	if (str[new_left_pos] == ']') {
		push_vertical(str, new_left_pos, push_direction);
	}
	if (str[new_left_pos + 1] == '[') {
		push_vertical(str, new_left_pos + 1, push_direction);
	}
	str[left_pos] = '.';
	str[right_pos] = '.';
	str[new_left_pos] = '[';
	str[new_left_pos + 1] = ']';
}


typedef struct {
	char a;
	char b;
} two_chars;

int solution_2(const string input) {
	char* str = input.content;
	int line_length = first_occurrence_of_char(str, '\n') + 1;
	
	int unpacked_warehouse_size = 0;
	int i = 0;
	while (str[i] != '\n') {
		i += line_length;
		unpacked_warehouse_size += line_length * 2 - 1;
	}
	int warehouse_size = i;


	char* copy = malloc(unpacked_warehouse_size);
	
	// '#' - 35;
	// 'O' - 79
	// '.' - 46;
	// '@' - 64
 
	two_chars widen_array[80];

	two_chars hash = {'#', '#'};
	widen_array[35] = hash;
	two_chars box = {'[', ']'};
	widen_array[79] = box;
	two_chars dot = {'.', '.'};
	widen_array[46] = dot;
	two_chars at = {'@', '.'};
	widen_array[64] = at;
	
	int j = 0;
	for (int k = 0; k < warehouse_size; k++) {
		if (str[k] == '\n') {
			copy[j] = '\n';
			j++;
			continue;
		}
		two_chars* chars = &widen_array[(uint8_t)str[k]];
		copy[j] = chars->a;
		copy[j + 1] = chars->b;
		j += 2;
	}

	int robot_pos = first_occurrence_of_char(copy, '@');
	int unpacked_line_length = first_occurrence_of_char(copy, '\n') + 1;
	copy[robot_pos] = '.';
	// above might seem slightly inefficient, but i measure only 2 microseconds up until now, so I think it's fine.

	//'>' - 62
	//'^' - 94
	//'<' - 60
	//'v' - 118

	int move_arr[119];

	move_arr[62] = 0;
	move_arr[94] = 1;
	move_arr[60] = 2;
	move_arr[118] = 3;

	int directions[] = {1, -unpacked_line_length, -1, unpacked_line_length};

	bool is_horizontal[] = {true, false, true, false};	
	
	// i is still at the inputs section
	while (i < input.size) {
		if (str[i] == '\n') {
			i++;
			continue;
		}
		int dir_index = move_arr[(uint8_t)str[i]];
		int direction = directions[dir_index];
		i++;
		// can't ever be out of bounds thanks to surrounding walls
		int new_pos = robot_pos + direction;
		if (copy[new_pos] == '#')
			continue; // can't move here
		if (is_wide_box(copy[new_pos])) {
			if (is_horizontal[dir_index]) {

				// this position represents the position of the edge *furthest* from the robot
				int new_box_pos = new_pos;
				int amount_pushed = 0;
				do {
					new_box_pos += 2 * direction;
					amount_pushed += 1;
				} while (is_wide_box(copy[new_box_pos]));
				
				if (copy[new_box_pos] == '#')
					continue;
				else {
					if (direction == 1)
						memmove(&copy[new_pos + direction], &copy[new_pos], amount_pushed * 2);
					else
						memmove(&copy[new_box_pos], &copy[new_box_pos + 1], amount_pushed * 2);
					copy[new_pos] = '.';
				}
			}
			else {
				// vertical pushing is more complex, since it's possible for the robot to push two boxes at once
				if (!is_vertical_push_valid(copy, new_pos, direction)) {
					continue;
				}
				push_vertical(copy, new_pos, direction);
			}
		}
		robot_pos = new_pos;
	}
	int sum = 0;
	for (int j = 0; j < unpacked_warehouse_size; j++) {
		if (copy[j] == '[') {
			int y = j / unpacked_line_length;
			int x = j % unpacked_line_length;
			sum += y * 100 + x;
		}
	}

	free(copy);
	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(15, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


