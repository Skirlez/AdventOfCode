#include <iostream>
#include <deque>
#include "Util.h"

using namespace std;

const char arr[4] = {'R', 'D', 'L', 'U'};
template<bool ColorParse> int64_t sharedSolution(const string& input) {
	const char* str = input.c_str();
	const size_t size = input.size();
	
	size_t pos;
	int64_t x = 0;
	int64_t y = 0;
	int64_t area = 0;
	int64_t borderPoints = 0;

	if (!ColorParse)
		pos = 0;
	else
		pos = 12;
	while (pos < size) {
		char dir;
		int num = 0;
		int multiplier = 1;
		int digitAmount = 0;
		if (!ColorParse) {
			dir = str[pos];
			pos += 3;
			while (str[pos] != ' ')
				pos++;
			pos--;
			while (str[pos] != ' ') {
				num += (str[pos] - '0') * multiplier;
				multiplier *= 10;
				digitAmount++;
				pos--;
			}
		}
		else {
			while (str[pos] != ')')
				pos++;
			pos--;
			dir = arr[str[pos] - '0'];
			pos--;
		
			for (int i = 0; i < 5; i++) {
				const char& c = str[pos];
				if (c >= '0' && c <= '9')
					num += (c - '0') * multiplier;
				else
					num += (c - 'a' + 10) * multiplier;
				multiplier *= 16;
				pos--;
			}
		}

		if (dir == 'R') {
			area -= y * num; // (x * y - (x + num) * y) -> (y * -num)
			x += num;
		}
		else if (dir == 'L') {
			area += y * num; // (x * y - (x - num) * y) -> (y * num)
			x -= num;
		}
		else if (dir == 'D') {
			area += x * num; // (x * (y + num) - x * y) -> (x * num)
			y += num;
		}
		else /* if (dir == 'U') */ {
			area -= x * num; // (x * (y - num) - x * y) -> (x * -num)
			y -= num;
		}

		borderPoints += num;

		if (!ColorParse) {
			// we are currently on the space character before the digit(s)
			pos += digitAmount + 1 + 11; // jump over digit(s), space, color
		}
		else {
			// we are currently on the '(', jump to (near) the ')'
			pos += 21;
		}
	}

	// This Is the shoelace formula combined with pick's theorem (Which I did not do on Day 10 it might make that day faster)
	area = abs(area) >> 1;
	return area + (borderPoints >> 1) + 1;
}

int solution_1(const string& input) {
	return sharedSolution<false>(input);
}

int64_t solution_2(const string& input) {
	return sharedSolution<true>(input);
}

int main(int argc, char* argv[]) {
	string input = readInput(18, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 100000);
	return 0;
}


