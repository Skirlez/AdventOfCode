#include <iostream>
#include "Util.h"

using namespace std;

int solution_1(const string& input) {
	const char* str = input.c_str();
	const size_t size = input.size() - 1; // avoid newline

	unsigned int sum = 0;
	unsigned int value = 0;
	for (size_t i = 0; i < size; i++) {
		if (str[i] == ',') {
			sum += value;
			value = 0;
			continue;
		}
		value += str[i];
		value *= 17;
		value &= 0xFF;
	}
	return sum + value;
}

inline bool areLabelsEqual(const char* str, const size_t size, size_t label1, size_t label2) {
	for (int j = 0; ((label1 + j) < size) && ((label2 + j) < size); j++) {
		if (str[label1 + j] == '-' ||  str[label1 + j] == '=')
			return (str[label2 + j] == '-' || str[label2 + j] == '=');
		if (str[label1 + j] != str[label2 + j])
			return false;
	}
	return true;
}

inline int findLabelInVector(const char* str, const size_t size, const vector<pair<size_t, int>>& vec, size_t start) {
	for (size_t i = 0; i < vec.size(); i++) {
		if (areLabelsEqual(str, size, start, vec[i].first))
			return i;
	}
	return -1;
}

int solution_2(const string& input) {
	const char* str = input.c_str();
	const size_t size = input.size() - 1; // avoid newline

	const size_t boxAmount = 256;
	vector<pair<size_t, int>> boxes[boxAmount];
	fill(boxes, boxes + boxAmount, vector<pair<size_t, int>>());

	unsigned int hash = 0;
	size_t labelStart = 0;
	for (size_t i = 0; i < size; i++) {
		if (str[i] == '=') {
			i++;
			int num = str[i] - '0';
			vector<pair<size_t, int>>& vec = boxes[hash];
			int ind = findLabelInVector(str, size, vec, labelStart);
			if (ind == -1)
				vec.push_back(pair<size_t, int>(labelStart, num));
			else
				vec[ind].second = num;

			hash = 0;
			i++; // skip comma
			labelStart = i + 1;
			continue;
		}
		if (str[i] == '-') {
			vector<pair<size_t, int>>& vec = boxes[hash];
			int ind = findLabelInVector(str, size, vec, labelStart);
			if (ind != -1)
				boxes[hash].erase(boxes[hash].begin() + ind);
			hash = 0;
			i++; // skip comma
			labelStart = i + 1;
			continue;
		}
		hash += str[i];
		hash *= 17;
		hash &= 0xFF;
	}
	int sum = 0;
	for (size_t i = 0; i < boxAmount; i++) {
		vector<pair<size_t, int>>& box = boxes[i];
		if (box.size() == 0)
			continue;
		for (size_t j = 0; j < box.size(); j++)
			sum += (i + 1) * (j + 1) * box[j].second;
	}


	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(15, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}


