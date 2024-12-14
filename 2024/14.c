#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Util.h"


static inline int parse_number_until_char(char* str, int* indexPtr, char stop) {
	int pos = *indexPtr;
	int num = 0;
	do {
		num = num * 10 + (str[pos] - '0');
		pos++;
	} while (str[pos] != stop);
	*indexPtr = pos;
	return num;
}

static inline int parse_signed_number_until_char(char* str, int* indexPtr, char stop) {
	int pos = *indexPtr;
	int num = 0;

	int sign;
	if ((str[pos] == '-')) {
		sign = -1;
		pos++;
	}
	else
		sign = 1;

	do {
		num = num * 10 + (str[pos] - '0');
		pos++;
	} while (str[pos] != stop);
	*indexPtr = pos;
	return num * sign;
}

int solution_1(const string input) {
	const int width = 101;
	const int height = 103;
	const int time = 100;
	char* str = input.content;
	int first_row[] = {0, 0};
	int second_row[] = {0, 0};
	int* quadrants[] = {first_row, second_row};

	int i = 0;
	while (i < input.size) {
		// p=x,y v=x,y
		// "p=" - 2 chars
		i += 2;
		int x = parse_number_until_char(str, &i, ',');
		// comma
		i++;
		int y = parse_number_until_char(str, &i, ' ');
		// " v=" - 3 chars
		i += 3;
		int vx = parse_signed_number_until_char(str, &i, ',');
		// comma
		i++;
		int vy = parse_signed_number_until_char(str, &i, '\n');
		// newline
		i++;

		int final_x = (x + vx * time) % width;
		int final_y = (y + vy * time) % height;
		if (final_x < 0)
			final_x += width;
		if (final_y < 0)
			final_y += height;
		if (final_x == width / 2 || final_y == height / 2)
			continue;
		int q_x = (final_x < width / 2);
		int q_y = (final_y < height / 2);
		quadrants[q_y][q_x]++;
	}

	return quadrants[0][0] * quadrants[0][1] * quadrants[1][0] * quadrants[1][1];
}


typedef struct  {
	int x;
	int y;
	int vx;
	int vy;
} robot;



int solution_2(const string input) {

	const int width = 101;
	const int height = 103;
	char* str = input.content;
	int* board = malloc(width * height * sizeof(int));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			board[j + i * width] = 0;
		}
	}


	int amount_of_robots = 0;
	for (int i = 0; i < input.size; i++) {
		if (str[i] == '\n')
			amount_of_robots++;
	}
	robot* robots = malloc(amount_of_robots * sizeof(robot));
	{
		int last_index = -1;
		int i = 0;
		while (i < input.size) {
			robot r;

			// p=x,y v=x,y
			// "p=" - 2 chars
			i += 2;
			r.x = parse_number_until_char(str, &i, ',');
			// comma
			i++;
			r.y = parse_number_until_char(str, &i, ' ');
			// " v=" - 3 chars
			i += 3;
			r.vx = parse_signed_number_until_char(str, &i, ',');
			// comma
			i++;
			r.vy = parse_signed_number_until_char(str, &i, '\n');
			// newline
			i++;

			board[r.x + r.y * width]++;

			last_index++;
			robots[last_index] = r;

			
		}
	}

	int time = 1;
	while (1) {
		for (int i = 0; i < amount_of_robots; i++) {
			robot* r = &robots[i];
			board[r->x + r->y * width]--;
			
			r->x += r->vx;
			r->y += r->vy;
			if (r->x < 0)
				r->x += width;
			else if (r->x >= width)
				r->x -= width;
			if (r->y < 0)
				r->y += height;
			else if (r->y >= height)
				r->y -= height;

			board[r->x + r->y * width]++;
		}
		int longest_line = 0;
		
		for (int i = 0; i < height; i++) {
			int line = 0;
			for (int k = 0; k < width; k++) {
				if (board[k + i * width] != 0)
					line++;
				else {
					if (line == 31) {
						/*
						for (int i = 0; i < height; i++) {
							for (int k = 0; k < width; k++) {
								if (board[k + i * width] == 0)
									printf(".");
								else
									printf("%c", board[k + i * width] + '0');
							}
							printf("\n");
						}
						printf("\n");
						*/
						free(robots);
						free(board);
						return time;

					}
					line = 0;
				}
			}
		}

		time++;
	}
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(14, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 2);

	free(input.content);
	return 0;
}


