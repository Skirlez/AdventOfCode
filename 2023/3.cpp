#include <iostream>
#include "Util.h"

using namespace std;

bool scanBoundsCheck(const char* str, int scan, int size, int lineLength) {
	if (scan < 0 || scan > size) // check if out of bounds 
		return true;

	// we actually don't need to check if the scan goes too far to the left or right so much that it goes to other lines
	// (then, a number on a different line could be considered adjacent to another character, when it isn't)
	// because the walls of '\n' characters fail this number check

	if (str[scan] < '0' || str[scan] > '9')
		return true;
	return false;
}

int solution_1(const string& input) {
	const char* str = input.c_str();
	int size = input.size();
	int sum = 0;
	int lineLength = 0;
	while (str[lineLength] != '\n')
		lineLength++;
	lineLength++; // one more (length, not index)
	for (int i = 0; i < size; i++) {
		char c = str[i];
		if (c >= '0' && c <= '9' || c == '.' || c == '\n')
			continue; // if the current char is a number or dot, we do not care
		
		int offsets_x[3] = {-1, 0, 1};
		int offsets_y[3] = {-lineLength, 0, lineLength};

		// we found a special character; scan the nearby numbers
		for (int y = 0; y < 3; y++) {
			int foundNumberInd = 0;
			for (int x = 0; x < 3; x++) {
				int scan = i + offsets_x[x] + offsets_y[y];
				if (scanBoundsCheck(str, scan, size, lineLength))
					continue;

				// we have found a number
				// move all the way to its right, then back left, to parse all of it

				int inc = 0;
				while (!scanBoundsCheck(str, scan + inc + 1, size, lineLength))
					inc++;
				
				if (foundNumberInd == 0)
					foundNumberInd = scan + inc;
				else if (foundNumberInd == scan + inc) // we have already counted this number
					continue;

				int multiplier = 1;
				int num = 0;
				while (inc >= 0 || !scanBoundsCheck(str, scan + inc, size, lineLength)) {
					int digit = str[scan + inc] - '0';
					num += digit * multiplier;
					multiplier *= 10;
					inc--;
				}
				sum += num;
			}
		}

	}

	return sum;
}


int solution_2(const string& input) {
	const char* str = input.c_str();
	int size = input.size();
	int sum = 0;
	int lineLength = 0;
	while (str[lineLength] != '\n')
		lineLength++;
	lineLength++; // one more (length, not index)
	for (int i = 0; i < size; i++) {
		char c = str[i];
		if (c != '*')
			continue;
		
		int offsets_x[3] = {-1, 0, 1};
		int offsets_y[3] = {-lineLength, 0, lineLength};

		// we found the gear; scan the nearby numbers

		int scannedCount = 0;
		int gearRatio = 1;

		for (int y = 0; y < 3 && scannedCount < 2; y++) {
			int foundNumberInd = 0;
			for (int x = 0; x < 3 && scannedCount < 2; x++) {
				int scan = i + offsets_x[x] + offsets_y[y];
				if (scanBoundsCheck(str, scan, size, lineLength))
					continue;

				// we have found a number
				// move all the way to its right, then back left, to parse all of it

				int inc = 0;
				while (!scanBoundsCheck(str, scan + inc + 1, size, lineLength))
					inc++;
				
				if (foundNumberInd == 0)
					foundNumberInd = scan + inc;
				else if (foundNumberInd == scan + inc) // we have already counted this number
					continue;

				int multiplier = 1;
				int num = 0;
				while (inc >= 0 || !scanBoundsCheck(str, scan + inc, size, lineLength)) {
					int digit = str[scan + inc] - '0';
					num += digit * multiplier;
					multiplier *= 10;
					inc--;
				}
				scannedCount++;
				gearRatio *= num;
			}
		}
		if (scannedCount == 2)
			sum += gearRatio;
	}

	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(3, argv);
	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_1, input, 10000);

	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 10000);
	return 0;
}


