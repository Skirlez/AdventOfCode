#include <iostream>
#include <queue>
#include "Util.h"

using namespace std;


// ASSUMPTIONS

// There cannot be cardinal neighboring O characters

int findSteps(string input, int lineLength, int startPos, int steps) {
	const size_t size = input.size();
	string copy = input;
	int count = 0;
	queue<int> positions = queue<int>();
	positions.push(startPos);

	const int xOffsets[4] = {1, 0, -1, 0};
	const int yOffsets[4] = {0, lineLength, 0, -lineLength};

	bool add = false;
	for (int i = 0; i < steps; i++) {
		size_t batch = positions.size();
		for (size_t j = 0; j < batch; j++) {
			int currentPos = positions.front();
			positions.pop();
			for (size_t k = 0; k < 4; k++) {
				int newPos = currentPos + xOffsets[k] + yOffsets[k];
				if (newPos < 0 || newPos >= size)
					continue;
				if (copy[newPos] == '.') {
					copy[newPos] = 'O';
					count += add;
					positions.push(newPos);
				}
			}
		}

		add = !add;
	}
	return count;
}

int solution_1(const string& input) {
	string copy = input;
	
	const char* str = input.c_str();
	int lineLength = findChar(str, '\n') + 1;
	int startPos = findChar(str, 'S');

	queue<int> positions = queue<int>();
	positions.push(startPos);

	copy[startPos] = '.';
	return findSteps(copy, lineLength, startPos, 64);
}

int solution_2(const string& input) {
	return 0;
}

int main(int argc, char* argv[]) {
	string input = readInput(21, argv);

	//cout << "Part 1: " << solution_1(input) << '\n';
	//timeFunctionAndPrint(solution_1, input, 10000);
	
	//cout << '\n';

	//cout << "Part 2: " << solution_2(input) << '\n';
	//timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}