#include <stdio.h>
#include <stdlib.h>
#include "Util.h"


// This day was cool. I think there's barely any room for improvement here.

char mas[3] = {'M', 'A', 'S'};
static inline int find_mas_in_direction(char* str, int size, int line_length, int i, int direction_x, int direction_y) {
	for (int j = 0; j < 3; j++) {
		i = i + direction_x + direction_y * line_length;
		if (i < 0 || i >= size)
			return 0;
		if (str[i] != mas[j])
			return 0;
	}
	return 1;
}
int direction_combinations[16] = {1, 0,  1, 1,  0, 1,  -1, 1,  -1, 0,  -1, -1,  0, -1,  1, -1};

int solution_1(const string input) {
	char* str = input.content;
	int count = 0;
	int line_length = first_occurrence_of_char(str, '\n') + 1;

	for (int i = 0; i < input.size; i++) {
		if (str[i] == 'X') {
			for (int j = 0; j < 8; j++)
				count += find_mas_in_direction(str, input.size, line_length, i, direction_combinations[2 * j], direction_combinations[2 * j + 1]);
		}
	}

	return count;
}


static inline int find_ms_in_direction_and_opposite(char* str, int size, int line_length, int i, int direction_x, int direction_y) {
	i = i + direction_x + direction_y * line_length;
	if (i < 0 || i >= size)
		return 0;
	char ch = str[i];
	if (ch != 'S' && ch != 'M')
		return 0;
	i = i - direction_x * 2 - direction_y * line_length * 2;
	char ch2 = str[i];
	return (i >= 0 && i < size) && ((ch2 == 'S' || ch2 == 'M') && ch2 != ch);
}

int solution_2(const string input) {
	char* str = input.content;
	int count = 0;
	int line_length = first_occurrence_of_char(str, '\n') + 1;

	for (int i = 0; i < input.size; i++) {
		if (str[i] == 'A') {
			int right = find_ms_in_direction_and_opposite(str, input.size, line_length, i, 1, 1);
			int left = find_ms_in_direction_and_opposite(str, input.size, line_length, i, -1, 1);
			count += (right && left);
		}
	}

	return count;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(4, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


