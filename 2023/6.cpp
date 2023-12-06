#include <iostream>
#include "Util.h"
#include <cmath>

using namespace std;

inline int readNumberBackwardsUntilSpace(const string& str, int i) {
	int multiplier = 1;
	int num = 0;
	while (str[i] != ' ') {
		num += (str[i] - '0') * multiplier;
		multiplier *= 10;
		i--;
	}
	return num;
}

int solution_1(const string& input) {
	int multSum = 1;
	int lineLength = input.size() / 2 + 1;

	int i = 7; // safe start position
	char c;
	while (i < lineLength) {
		do {
			i++;
			c = input[i];
		} while (c < '0' || c > '9');

		do {
			i++;
			c = input[i];
		} while (c >= '0' && c <= '9');
		
		int time = readNumberBackwardsUntilSpace(input, i - 1);
		// numbers always begin on the same index on the next line
		int distance = readNumberBackwardsUntilSpace(input, i - 1 + lineLength);

		int waysToBeat = 0;
		for (int j = 1; j < time; j++) {
			if ((time - j) * j > distance)
				waysToBeat++;
		}
		multSum *= waysToBeat;
		i++;
	}

	return multSum;
}

inline int64_t readNumberBackwardsUntilEnd(const string& str, int i, int end) {
	int64_t multiplier = 1;
	int64_t num = 0;
	while (i != end) {
		if (str[i] >= '0' && str[i] <= '9') {
			num += (str[i] - '0') * multiplier;
			multiplier *= 10;
		}
		i--;
	}
	return num;
}

int solution_2(const string& input) {
	int multSum = 1;
	int lineLength = input.size() / 2 + 1;

	int i = lineLength - 1;

	// we are doing the quadratic formula !!!!
	#define start 7

	const int a = 1;
	int64_t b = -readNumberBackwardsUntilEnd(input, i - 1, start);
	int64_t c = readNumberBackwardsUntilEnd(input, i - 1 + lineLength, start + lineLength + 1);
	double discriminant = sqrt(b*b - 4*a*c);

 	int64_t x1 = (-(double)b - discriminant) / 2*a;
	int64_t midpoint = -b/2*a;

	int64_t result = (midpoint - x1) * 2;

	// if the midpoint is a whole number, we've counted it as two points in the previous calculation, so we must subtract 1
	if (-b % 2 == 0)
		result--; 
	return result;
}

int main(int argc, char* argv[]) {
	string input = readInput(6, argv);
	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 100000);
	return 0;
}


