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


// The writing suggests that the robots have a cycle for when they form the tree.

// Each robot has a horizontal cycle of 101 and a vertical cycle of 103 (since they're the width and height).
// If you did this puzzle by bruteforce, you likely saw that occassionally a lot of the robots
// would form a line pattern vertically and then horizontally. That is what you were seeing.

// So if there is a cycle, it must be at 0<n<LCM(101, 103). 101 and 103 are coprime too, so the LCM is just 101*103.

// The tree also seems to all be in one quadrant. We will identify where in the horizontal cycle and vertical cycle the tree is
// by running the simulation 101 times and seeing at what point there's the biggest discrepancy left/right,
// and also running the simulation 103 times and seeing at what point there's the biggest discrepancy up/down.
// (we'll do 103 and stop looking at the X axis for the last 2)

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
	int x_cycle = 0;
	int y_cycle = 0;
	int max_diff_x = 0;
	int max_diff_y = 0;
	for (int i = 0; i < 103; i++) {


		int left_count = 0;
		int right_count = 0;
		int up_count = 0;
		int down_count = 0;
		for (int j = 0; j < amount_of_robots; j++) {
			robot* r = &robots[j];

			if (r->x != width / 2 && r->y != height / 2) {
				if (r->x < width / 2)
					left_count++;
				else
					right_count++;

				if (r->y < height / 2)
					up_count++;
				else
					down_count++;
			}


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
		}
		if (i < 101) {
			int diff_x = abs(left_count - right_count);
			if (diff_x > max_diff_x) {
				max_diff_x = diff_x;
				x_cycle = i;
			}
		}
		int diff_y = abs(up_count - down_count);
		if (diff_y > max_diff_y) {
			max_diff_y = diff_y;
			y_cycle = i;
		}

	}
	// so we're looking for the smallest number between 0 < n < 10403
	// that n % 101 == x_cycle
	// and that n % 103 == y_cycle

	// Since 101 and 103 are coprime we can apply the chinese remainder theorem. But I don't actually know how to do that!
	// And every time I read about it I still don't understand. So let's do my idea instead.
	// We know that n % 101 == x_cycle. We know that all possible value of n must be on
	// f(x) = x_cycle + 101 * x
	// We're looking for the x where f(x) % 103 == y_cycle.

	// Since this is modular arithmetic, let's see how f changes for every increase in x:
	int diff = (x_cycle + 101 + 101) % 103 - (x_cycle + 101) % 103;
	// And create a set of functions we can work better with:
	// g(x) = (f(0) + diff * x) + 103k (k is some integer)
	// For every one of those lines, there is an x for which a y is equal to y_cycle.
	// We are looking for the first one where the x is whole. That is our x!
	
	// We will find k by bruteforce.
	// We need to decrease k into negatives if the slope (diff) is positive, or increase if it's negative, otherwise
	// our x might be negative - though we can do absolute value on it and it gives the correct answer, i don't know why it does.
	int k_add = diff > 0 ? -1 : 1;

	// f(0) + diff * x + 103k = y_cycle
	// x_cycle + diff*x +103k = y_cycle
	// diff*x + 103k = y_cycle-x_cycle
	// diff*x = y_cycle-x_cycle-103k
	// x = y_cycle-x_cycle-103k/diff
	int k = 0;
	while ((y_cycle - x_cycle - 103 * k) % diff != 0) {
		k += k_add;
	} 

	int x = (y_cycle - x_cycle - 103 * k) / diff;
	int n = x_cycle + 101 * x;

	return n;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(14, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


