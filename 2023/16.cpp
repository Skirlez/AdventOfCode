#include <iostream>
#include "Util.h"

using namespace std;

// angle stuff copied from Day 10
inline void wrapAroundAngle(int& angle) {
    if (angle < 0)
        angle = 4 - ((-angle) % 4);
	else
        angle %= 4;
}
// pretend all of the angles are multiplied by 90
int roundSin(const int& angle) {
	if (angle == 1)
		return 1;
	if (angle == 3)
		return -1;
	return 0;
}
int roundCos(const int& angle) {
	if (angle == 0)
		return 1;
	if (angle == 2)
		return -1;
	return 0;
}


inline void solveMaze(const char* str, const int size, int* energized, const int& lineLength, int i, int angle, int& sum) {
	while (true) {
		wrapAroundAngle(angle);
		i += (-roundSin(angle) * lineLength) + roundCos(angle);
		int angleMulti = ((roundCos(angle) != 0) << 1) - 1; // if horizontal 1, else -1
		if (str[i] == '\n' || (i >= size) || (i < 0))
			return;
		int energy = energized[i];
		if (energy == 0)
			sum++;
		energized[i] |= (1 << angle);
		if (energized[i] == energy)
			return;
		

		const char& next = str[i];
		if (next == '/')
			angle += 1 * angleMulti;
		else if (next == '\\')
			angle -= 1 * angleMulti;
		else if (angle == 1 || angle == 3) {
			if (next == '-') {
				energized[i] |= (1 << 1);
				energized[i] |= (1 << 3);
				solveMaze(str, size, energized, lineLength, i, 0, sum);
				solveMaze(str, size, energized, lineLength, i, 2, sum);
				return;
			}
		}
		else if (next == '|') {
			energized[i] |= (1 << 0);
			energized[i] |= (1 << 2);
			solveMaze(str, size, energized, lineLength, i, 1, sum);
			solveMaze(str, size, energized, lineLength, i, 3, sum);
			return;
		}
	}
}


int solution_1(const string& input) {
	const char* str = input.c_str();
	int* energized = new int[input.size()];
	//strcpy(copy, input.c_str());
	fill(energized, energized + input.size(), 0);
	size_t lineLength = findChar(str, '\n') + 1;
	int sum = 0;


	solveMaze(str, input.size(), energized, lineLength, -1, 0, sum);

	
	delete[] energized;
	return sum;
}

int solution_2(const string& input) {
	const char* str = input.c_str();
	size_t size = input.size();
	int* energized = new int[size];
	//strcpy(copy, input.c_str());
	fill(energized, energized + size, 0);
	size_t lineLength = findChar(str, '\n') + 1;

	size_t lines = size / lineLength;

	int maxSum = 0;
	int sum = 0;

	for (size_t i = 0; i < lines; i++) {
		solveMaze(str, size, energized, lineLength, lineLength * i - 1, 0, sum);
		maxSum = max(sum, maxSum);
		sum = 0;
		fill(energized, energized + size, 0);

		solveMaze(str, size, energized, lineLength, lineLength - 1, 2, sum);
		maxSum = max(sum, maxSum);
		sum = 0;
		fill(energized, energized + size, 0);
	}
	for (size_t i = 0; i < lineLength; i++) {
		solveMaze(str, size, energized, lineLength, size - lineLength - 1 + i, 1, sum);
		maxSum = max(sum, maxSum);
		sum = 0;
		fill(energized, energized + size, 0);

		solveMaze(str, size, energized, lineLength, i - lineLength, 3, sum);
		maxSum = max(sum, maxSum);
		sum = 0;
		fill(energized, energized + size, 0);
	}

	delete[] energized;
	return maxSum;
}

int main(int argc, char* argv[]) {
	string input = readInput(16, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 100);
	return 0;
}


