#include <iostream>
#include "Util.h"
#include <utility>

using namespace std;

inline vector<int> getDirections(const char* str, size_t& i) {
	vector<int> directions = vector<int>();
	while (true) {
		char c = str[i];
		if (c == '\n')
			break;
		/*
		'L' = 76
		'R' = 82

		76 >> 4 = 4
		82 >> 4 = 5

		4 - 4 = 0 
		5 - 4 = 1 

		and one final ! operation to switch the two
		so L -> 1 and R -> 0
		This will remove one (1) branch in the future
		*/
		int ind = !((c >> 4) - 4);
		directions.push_back(ind);
		i++;
	}
	return directions;
}

inline int16_t convertCharsToNumberAndProgress(const char* str, size_t& i) {
	//cout << str[i] << str[i + 1] << str[i + 2] << '\n';
	return ((str[i] - 'A') << 10 | (str[++i] - 'A') << 5 | (str[++i] - 'A'));
}

int solution_1(const string& input) {
	size_t i = 0;

	const int16_t max = (('Z' - 'A') << 10 | ('Z' - 'A') << 5 | ('Z' - 'A'));
	int32_t *arr = new int32_t[max];

	const char* str = input.c_str();
	vector<int> directions = getDirections(str, i);
	size_t dirSize = directions.size();
	i += 2; // jump over two newlines

	size_t inputSize = input.size();
	while (i < inputSize) {
		int16_t num = convertCharsToNumberAndProgress(str, i);
			
		i += 5; // jump over spaces, equal and parenthesis
		int32_t left = convertCharsToNumberAndProgress(str, i);
		i += 3; // jump over comma and space
		int16_t right = convertCharsToNumberAndProgress(str, i);

		arr[num] = (left << 16 | right);
		i += 3; // jump over parenthesis and newline
	}


	int16_t current = 0; // AAA becomes 0 when making it into a number
	int j = 0;
	int steps = 0;
	while (true) {
		int dir = directions[j];
		int32_t nums = arr[current];
		current = nums >> (16 * dir);
		steps++;
		if (current == max)
			break;
		j++;
		if (j >= dirSize)
			j = 0;
	}

	delete[] arr;

	return steps;
}

inline int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int64_t solution_2(const string& input) {
	size_t i = 0;

	const int16_t max = (('Z' - 'A') << 10 | ('Z' - 'A') << 5 | ('Z' - 'A'));
	int32_t *arr = new int32_t[max];

	const char* str = input.c_str();
	vector<int> directions = getDirections(str, i);
	int dirSize = directions.size();
	i += 2; // jump over two newlines

	vector<int16_t> startingNumbers = vector<int16_t>();

	const int16_t lastCharMask = (1 << 5) - 1;

	size_t inputSize = input.size();
	while (i < inputSize) {
		int16_t num = convertCharsToNumberAndProgress(str, i);
		if ((num & lastCharMask) == 0) // 0 == 'A' - 'A'
			startingNumbers.push_back(num);

		i += 5; // jump over spaces, equal and parenthesis
		int32_t left = convertCharsToNumberAndProgress(str, i);
		i += 3; // jump over comma and space
		int16_t right = convertCharsToNumberAndProgress(str, i);

		arr[num] = (left << 16 | right);
		i += 3; // jump over parenthesis and newline
	}

	size_t startingSize = startingNumbers.size();
	size_t loops[startingSize];
	int64_t steps = 0;

	for (size_t m = 0; m < startingSize; m++) {
		size_t j = 0;
		int16_t& current = startingNumbers[m];

		size_t loop = 0;
		do {
			int dir = directions[j];
			int32_t& nums = arr[current];
			current = nums >> (16 * dir);
			j++;
			if (j >= dirSize)
				j = 0; 

			loop++;
		} while ((current & lastCharMask) != ('Z' - 'A'));
		
		loops[m] = loop;
	}


	/* I can barely tell you why this works. We are calculating the LCM (Least Common Multiple) of all of the loop numbers
	we found earlier. As it turns out it is GUARANTEED that the amount of steps from each ghost's starting position
	to the only ending position it can reach (Yes, each ghost will only ever reach one) is the same as the time it would take
	to reach that ending position when you start from it (Yes, it's a loop!) So the LCM of all of the loop lengths that we just calculated is the precise amount of steps that will take
	all of the ghosts to "synchronize" and all be on their ending points at the same time.
	*/
	int64_t ans = loops[0];
	for (int m = 1; m < startingSize; m++)
		ans *= loops[m] / gcd(ans, loops[m]);
	
	delete[] arr;

	return ans;
}

int main(int argc, char* argv[]) {
	string input = readInput(8, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 10000);
	return 0;
}


