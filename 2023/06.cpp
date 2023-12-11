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

// returns the smaller X from the quadratic formula (*where a = 1)
inline int64_t quadraticFormula(int64_t b, int64_t c) {
	double discriminant = sqrt(b*b - 4*c);
	return (int64_t)(((double)-b) - discriminant) >> 1;
}

int solution_1(const string& input) {
	int multSum = 1;
	int lineLength = input.size() / 2;

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
		
		int b = -readNumberBackwardsUntilSpace(input, i - 1);
		// numbers always begin on the same index on the next line
		int c = readNumberBackwardsUntilSpace(input, i - 1 + lineLength);

		int64_t x1 = quadraticFormula(b, c);
		int64_t x2 = (-b) - x1 - 1;

		multSum *= (x2 - x1);
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

#define start 7
int solution_2(const string& input) {
	int multSum = 1;
	int lineLength = input.size() / 2 + 1;

	int i = lineLength - 1;

	int64_t b = -readNumberBackwardsUntilEnd(input, i - 1, start);
	int64_t c = readNumberBackwardsUntilEnd(input, i - 1 + lineLength, start + lineLength + 1);

 	int64_t x1 = quadraticFormula(b, c);
	int64_t x2 = (-b) - x1 - 1;

	return (x2 - x1);
}

int main(int argc, char* argv[]) {
	string input = readInput(6, argv);
	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 1000000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 1000000);
	return 0;
}


