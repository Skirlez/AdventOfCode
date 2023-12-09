#include <iostream>
#include "Util.h"
#include <algorithm>

using namespace std;

inline int readNumberBackwards(const char* str, int& j) {
	int num = 0;
	int multiplier = 1;
	while (true) {
		const char& c = str[j];
		if (c >= '0' && c <= '9')
			num += (c - '0') * multiplier;
		else if (c == '-') {
			num *= -1;
			j--;
			return num;
		}
		else
			return num;
		j--;
		if (j < 0)
			return num;

		multiplier *= 10;
	} 

}

int solution_1(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int64_t sum = 0;
	for (int i = 0; i < vec.size(); i++) {
		const char* str = vec[i].c_str();
		vector<int> sublist = vector<int>();
		vector<int> lastElements = vector<int>();
		{
			int j = vec[i].size() - 1;
			int prevNum = readNumberBackwards(str, j);
			lastElements.push_back(prevNum);
			j--;
			while (j >= 0) {
				int num = readNumberBackwards(str, j);
				sublist.push_back(prevNum - num);
				prevNum = num;
				j--;
			}
		}
		reverse(sublist.begin(), sublist.end());
		vector<int> nextSublist = vector<int>();
		
		bool allZeros;
		do {
			int prevNum = sublist.front();
			lastElements.push_back(sublist.back());
			int sublistSize = sublist.size();
			allZeros = true;
			for (int j = 1; j < sublistSize; j++) {
				int num = sublist[j];
				int newNum = num - prevNum;
				prevNum = num;
				allZeros = allZeros && (newNum == 0);
				nextSublist.push_back(newNum);
			}
			sublist = nextSublist;
			nextSublist.clear();
		} while(!allZeros);

		for (int j = 0; j < lastElements.size(); j++)
			sum += lastElements[j];

	}
	return sum;
}

int solution_2(const string& input) {
	vector<string> vec = splitString(input, '\n');
	int64_t sum = 0;
	for (int i = 0; i < vec.size(); i++) {
		const char* str = vec[i].c_str();
		vector<int> sublist = vector<int>();
		vector<int> firstElements = vector<int>();
		{
			int j = vec[i].size() - 1;
			int prevNum = readNumberBackwards(str, j);
			j--;
			while (j >= 0) {
				int num = readNumberBackwards(str, j);
				sublist.push_back(prevNum - num);
				prevNum = num;
				j--;
			}
			firstElements.push_back(prevNum);
		}
		reverse(sublist.begin(), sublist.end());
		
		bool allZeros;
		vector<int> nextSublist = vector<int>();
		do {
			int prevNum = sublist.front();
			firstElements.push_back(prevNum);
			int sublistSize = sublist.size();
			allZeros = true;
			for (int j = 1; j < sublistSize; j++) {
				int num = sublist[j];
				int newNum = num - prevNum;
				prevNum = num;
				allZeros = allZeros && (newNum == 0);
				nextSublist.push_back(newNum);
			}
			sublist = nextSublist;
			nextSublist.clear();
		} while(!allZeros);
		int64_t extrapolation = 0;
		for (int j = firstElements.size() - 1; j >= 0; j--)
			extrapolation = firstElements[j] - extrapolation;
		sum += extrapolation;
		
	}
	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(9, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}


