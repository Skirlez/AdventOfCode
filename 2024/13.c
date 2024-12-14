#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Util.h"

static inline int64_t parse_number_until_char(char* str, int* indexPtr, char stop) {
	int pos = *indexPtr;
	int64_t num = 0;
	do {
		num = num * 10 + (str[pos] - '0');
		pos++;
	} while (str[pos] != stop);
	*indexPtr = pos;
	return num;
}

typedef struct {
	int64_t x1;
	int64_t x2;
	int64_t y1;
	int64_t y2;
	int64_t tx;
	int64_t ty;
} claw_machine;

static inline claw_machine parse_claw_machine(char* str, int* indexPtr) {
	int i = *indexPtr;
	claw_machine machine;
	// "Button A: X+" - 12 chars
	i += 12;

	machine.x1 = parse_number_until_char(str, &i, ',');
	// ", Y+" - 4 chars
	i += 4;
	machine.y1 = parse_number_until_char(str, &i, '\n');
	// newline
	i += 1;

	// "Button B: X+" - 12 chars
	i += 12;

	machine.x2 = parse_number_until_char(str, &i, ',');
	// ", Y+" - 4 chars
	i += 4;
	machine.y2 = parse_number_until_char(str, &i, '\n');
	// newline
	i += 1;

	// "Prize: X=" - 9 chars
	i += 9;
	machine.tx = parse_number_until_char(str, &i, ',');

	// ", Y+" - 4 chars
	i += 4;
	machine.ty = parse_number_until_char(str, &i, '\n');
	
	// two newlines
	i += 2;
	*indexPtr = i;
	return machine;
}


int solution_1(const string input) {
	char* str = input.content;
	int i = 0;
	int sum = 0;
	while (i < input.size) {
	
		claw_machine m = parse_claw_machine(str, &i);
		// The amount of times B must be pressed can be expressed through the amount of times A must be pressed,
		// using this formula (using x1/x2/tx, but works the same for y):
		// B = (Ax1 - tx) / -x2


		// We can create a token function for each axis, like
		// f(A) = 3A + B = 3A - (Ax1 - tx)/x2 for the X axis,
		// and g(A) = 3A + B = 3A - (Ay1 - tx)/y2

		// These functions, for a given A value, will tell us how many tokens we must spend
		// if we pressed the A button A times,
		// and pressed the B button (Ax1 - tx) / -x2 or (Ay1 - ty) / -y2 times.

		// We know that if the claw machine can be beaten, both functions must have a
		// value of A that results in the same amount of tokens, the minimum.
		// And since the functions are linear, it means this value is the ONLY value they share!
		// So all we have to do is compare the two functions and do some algebra, which leads us to this:
		// A = (tx/x2 - ty/y2)/(x1/x2 - y1/y2)

		// or the alternative form A = (tx*y2 - ty*x2)/(x1*y2 - y1*x2),
		// which is better since it has more multiplication instead of division,
		// and it's easier to check if the result is round.

		
		if ((m.tx * m.y2 - m.ty * m.x2) % (m.x1 * m.y2 - m.y1 * m.x2) != 0)
			continue;
		int A = (m.tx * m.y2 - m.ty * m.x2) / (m.x1 * m.y2 - m.y1 * m.x2);
		int B = (A * m.x1 - m.tx) / -m.x2;
		if (A > 100 || B > 100 || A < 0 || B < 0)
			continue;
		sum += 3 * A + B;
	}
	return sum;
}


int64_t solution_2(const string input) {
	char* str = input.content;
	int i = 0;
	int64_t sum = 0;
	while (i < input.size) {
		// Almost the same as part 1, except A and B can be pressed many more times,
		// so we upgrade them to int64_t and don't check if they're smaller than 100.
		claw_machine m = parse_claw_machine(str, &i);
		m.tx += 10000000000000;
		m.ty += 10000000000000;

		
		if ((m.tx * m.y2 - m.ty * m.x2) % (m.x1 * m.y2 - m.y1 * m.x2) != 0)
			continue;
		int64_t A = (m.tx * m.y2 - m.ty * m.x2) / (m.x1 * m.y2 - m.y1 * m.x2);
		int64_t B = (A * m.x1 - m.tx) / -m.x2;
		if (A < 0 || B < 0)
			continue;
		sum += 3 * A + B;
	}
	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(13, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %llu\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 10000);

	free(input.content);
	return 0;
}


