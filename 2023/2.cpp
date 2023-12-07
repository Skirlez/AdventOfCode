#include <iostream>
#include "Util.h"

using namespace std;


int maxColors[3] = {12, 13, 14}; // r, g, b

int gameFindColon(const char* str) {
	// find the colon ':'
	int j = 4; // 4/5 is a safe start position
	char c;
	do {
		j++;
		c = str[j];
	} while (c != ':');
	// we have found the colon, jump 2 chars ahead for the next number
	j += 2;
	return j;
}

/* Read the number at position j and progress to the next color char */
int gameReadNumberAndProgress(const char* str, int &j) {
	// there are either 1 or 2 digits
	int digit_1 = str[j] - '0';
	int digit_2 = str[j + 1] - '0';
	if (digit_2 < 0 || digit_2 > 9) {
		j += 2; 
		return digit_1;
	}
	j += 3; // jump one more char to account for the extra digit
	return digit_2 + digit_1 * 10;
}

/* Read the color char at position j and progress to the next number
 We have to jump a different amount forward because the color names are of different lengths,
 so jumpOffsets contains what we need */
int jumpOffsets[3] = {5, 7, 6}; // r, g, b
int gameReadColorIndexAndProgress(const char* str, int &j) {
	char color = str[j];
	int ind;
	if (color == 'r')
		ind = 0;
	else if (color == 'g')
		ind = 1;
	else
		ind = 2;
	j += jumpOffsets[ind];
	return ind;
}


inline bool isGamePossible(const char* str, int strLength) {
	int j = gameFindColon(str);
	do {
		int num = gameReadNumberAndProgress(str, j);
		int ind = gameReadColorIndexAndProgress(str, j);
		if (num > maxColors[ind])
			return false;
	} while (j < strLength);

	return true;
}

int solution_1(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int size = vec.size();
	int sum = 0;
	for (int i = 0; i < size; i++) {
		if (isGamePossible(vec[i].c_str(), vec[i].size()))
			sum += (i + 1);
	}
	return sum;
}

inline int powerOfMinimumCubesForGame(const char* str, int strLength) {
	int minColors[3] = {0, 0, 0}; // r, g, b

	int j = gameFindColon(str);
	do {
		int num = gameReadNumberAndProgress(str, j);
		int ind = gameReadColorIndexAndProgress(str, j);
		if (minColors[ind] < num)
			minColors[ind] = num;
	} while (j < strLength);
	
	return minColors[0] * minColors[1] * minColors[2];
}


int solution_2(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int size = vec.size();
	int sum = 0;
	for (int i = 0; i < size; i++)
		sum += powerOfMinimumCubesForGame(vec[i].c_str(), vec[i].size());
	return sum;
}

int main(int argc, char** argv) {
	string input = readInput(2, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);

	cout << '\n';
	
	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}


