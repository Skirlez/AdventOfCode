#include <iostream>
#include "Util.h"

using namespace std;

#define NUMBER_STRING_ARR_SIZE 9
string arr[NUMBER_STRING_ARR_SIZE] = {"one", "two", "six", "four", "five", "nine", "three", "seven", "eight"};
int shiftArounds[NUMBER_STRING_ARR_SIZE] = {1, 2, 6, 4, 5, 9, 3, 7, 8};

inline int findNumberInString(const string& sub) {
	for (int i = 0; i < NUMBER_STRING_ARR_SIZE; i++) {
		string& numString = arr[i];
		if (sub.size() < numString.size())
			return 0;
		if (sub.find(numString) != string::npos)
			return shiftArounds[i];
	}
	return 0;
}

int solution_1(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int sum = 0;
	int vecSize = vec.size();
	for (int i = 0; i < vecSize; i++) {
		string& str = vec[i];
		int size = str.size();
		int digit = 0;
		for (int j = 0; j < size; j++) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;
		}
		int tens = digit * 10;


		for (int j = size - 1; j >= 0; j--) {
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
	int vecSize = vec.size();
	for (int i = 0; i < vecSize; i++) {
		string& str = vec[i];
		int size = str.size();
		int digit = 0;
		for (int j = 0; j < size; j++) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;

			if (j >= 2) {
				string sub = str.substr(0, j + 1);
				digit = findNumberInString(sub);
				if (digit != 0)
					break;
			}
		}
		int tens = digit * 10;


		for (int j = size - 1; j >= 0; j--) {
			digit = str[j] - '0';
			if (digit >= 0 && digit <= 9)
				break;
				
			int ind = size - 1 - j;
			if (ind >= 2) {
				string sub = str.substr(j, ind + 1);
				digit = findNumberInString(sub);
				if (digit != 0)
					break;
			}
		}

		sum += tens + digit;
	}
	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(1, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 100000);
	return 0;
}


