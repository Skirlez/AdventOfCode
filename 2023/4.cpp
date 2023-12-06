#include <iostream>
#include "Util.h"

using namespace std;

inline bool isNumberInVector(int num, const vector<int>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		if (num == vec[i])
			return true;
	}
	return false;
}

int solution_1(const string& input) {
	vector<string> cards = splitString(input, '\n');
	int size = cards.size();
	int sum = 0;
	for (int i = 0; i < size; i++) {
		const char* str = cards[i].c_str();
		int stringSize = cards[i].size();

		// the starting colon is always in position 8, and we jump 2 to get to the first number
		int j = 8 + 2;

		vector<int> winningNumbers = vector<int>();
		while (true) {
			int tens = str[j];
			if (tens == '|')
				break;
			int ones = str[j + 1] - '0';
			int number;
			if (tens == ' ')
				number = ones;
			else
				number = ones + (tens - '0') * 10;
			winningNumbers.push_back(number);
			
			j += 3;
		}
		j += 2;
		int value = 1;
		while (j < stringSize) {
			int tens = str[j];
			int ones = str[j + 1] - '0';
			int number;
			if (tens == ' ')
				number = ones;
			else
				number = ones + (tens - '0') * 10;
			if (isNumberInVector(number, winningNumbers))
				value <<= 1;
		
			j += 3;
		}
		value >>= 1;
		sum += value;
	}
	return sum;
}

int solution_2(const string& input) {
	vector<string> cards = splitString(input, '\n');
	int size = cards.size();
	int cardCounts[size];
	fill(cardCounts, cardCounts + size, 1); // set all values of the array to 1
	int sum = size;

	for (int i = 0; i < size; i++) {
		const char* str = cards[i].c_str();
		int stringSize = cards[i].size();

		// the starting colon is always in position 8, and we jump 2 to get to the first number
		int j = 8 + 2;

		vector<int> winningNumbers = vector<int>();
		while (true) {
			int tens = str[j];
			if (tens == '|')
				break;
			int ones = str[j + 1] - '0';
			int number;
			if (tens == ' ')
				number = ones;
			else
				number = ones + (tens - '0') * 10;
			winningNumbers.push_back(number);
			
			j += 3;
		}
		j += 2;
		int wins = 0;
		while (j < stringSize) {
			int tens = str[j];
			int ones = str[j + 1] - '0';
			int number;
			if (tens == ' ')
				number = ones;
			else
				number = ones + (tens - '0') * 10;
			if (isNumberInVector(number, winningNumbers))
				wins++;
		
			j += 3;
		}
		int count = cardCounts[i];
		for (int m = 0; m < wins; m++) {
			cardCounts[i + m + 1] += count;
			sum += count;
		}
		
	}
	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(4, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100000);

	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 100000);
	return 0;
}


