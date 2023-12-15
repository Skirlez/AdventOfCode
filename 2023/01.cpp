#include <iostream>
#include "Util.h"

using namespace std;

#define NUMBER_STRING_ARR_SIZE 9
string arr[NUMBER_STRING_ARR_SIZE] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
unsigned int progress[NUMBER_STRING_ARR_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

inline int progressNumberInString(const string& str, const size_t& ind) {
	for (size_t i = 0; i < NUMBER_STRING_ARR_SIZE; i++) {
		const string& num = arr[i];
		if (num[progress[i]] == str[ind]) {
			progress[i]++;
			if (progress[i] == num.size())
				return (i + 1);
		}
		else
			progress[i] = (num[0] == str[ind]);
	}
	return 0;
}
inline int progressNumberInStringBackwards(const string& str, const size_t& ind) {
	for (size_t i = 0; i < NUMBER_STRING_ARR_SIZE; i++) {
		const string& num = arr[i];
		if (num[num.size() - progress[i] - 1] == str[ind]) {
			progress[i]++;
			if (progress[i] == num.size())
				return (i + 1);
		}
		else
			progress[i] = (num[num.size() - 1] == str[ind]);
	}
	return 0;
}


int solution_1(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int sum = 0;
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++) {
		const string& str = vec[i];
		size_t size = str.size();
		int digit = 0;
		for (size_t j = 0; j < size; j++) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;
		}
		int tens = digit * 10;


		for (size_t j = size - 1; j >= 0; j--) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;
		}
		sum += tens + digit;
	}
	return sum;
}



int solution_2(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int sum = 0;
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++) {
		const string& str = vec[i];
		size_t size = str.size();
		int digit = 0;
		for (size_t j = 0; j < size; j++) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;

			digit = progressNumberInString(str, j);
			if (digit != 0)
				break;
		}
		int tens = digit * 10;
		fill(progress, progress + NUMBER_STRING_ARR_SIZE, 0);

		for (size_t j = size - 1; j >= 0; j--) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;
				
			digit = progressNumberInStringBackwards(str, j);
			if (digit != 0)
				break;
		}

		sum += tens + digit;
		fill(progress, progress + NUMBER_STRING_ARR_SIZE, 0);
	}
	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(1, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}


