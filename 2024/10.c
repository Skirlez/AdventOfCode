#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Util.h"

int get_trailhead_score(char* str, uint8_t* visited_nines_map, int pos, int line_length, int size) {
	char here = str[pos];
	if (here == '9') {
		if (visited_nines_map[pos] == 1)
			return 0;
		visited_nines_map[pos] = 1;
		return 1;
	}
	int directions[] = {1, -line_length, -1, line_length};
	int sum = 0;
	for (int i = 0; i < 4; i++) {
		int new_pos = pos + directions[i];
		if (new_pos < 0 || new_pos >= size)
			continue;
		if (str[new_pos] == here + 1) {
			sum += get_trailhead_score(str, visited_nines_map, new_pos, line_length, size);
		}
	}

	return sum;
}


int solution_1(const string input) {
	char* str = input.content;
	int line_length = first_occurrence_of_char(str, '\n') + 1;

	uint8_t* visited_nines_map = malloc(input.size * sizeof(uint8_t));
	int sum = 0;
	int i = 0;
	while (i < input.size) {
		if (str[i] == '0') {
			memset(visited_nines_map, 0, input.size);
			sum += get_trailhead_score(str, visited_nines_map, i, line_length, input.size);
		}
		i++;
	}

	free(visited_nines_map);
	return sum;
}


int get_trailhead_rating(char* str, int pos, int line_length, int size) {
	char here = str[pos];
	if (here == '9') {
		return 1;
	}
	int directions[] = {1, -line_length, -1, line_length};
	int ratings = 0;
	for (int i = 0; i < 4; i++) {
		int new_pos = pos + directions[i];
		if (new_pos < 0 || new_pos >= size)
			continue;
		if (str[new_pos] == here + 1) {
			int rating = get_trailhead_rating(str, new_pos, line_length, size);
			ratings += rating;
		}
	}

	return ratings;
}


int solution_2(const string input) {
	char* str = input.content;
	int line_length = first_occurrence_of_char(str, '\n') + 1;
	int sum = 0;
	int i = 0;
	while (i < input.size) {
		if (str[i] == '0') {
			sum += get_trailhead_rating(str, i, line_length, input.size);
		}
		i++;
	}

	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(10, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


