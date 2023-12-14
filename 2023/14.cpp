#include <iostream>
#include <cstring>
#include <unordered_set>
#include "Util.h"

using namespace std;


inline int getNorthWeight(const char* str, size_t size, size_t lineLength, size_t distanceFromBottom) {
	int sum = 0;
	for (size_t i = 0; i < lineLength - 1; i++) {
		size_t pos = i;
		size_t realDist = distanceFromBottom;
		size_t dist = distanceFromBottom;
		while (pos < size) {
			if (str[pos] == '#')
				dist = realDist;
			else if (str[pos] == 'O')
				sum += dist;
			else if (str[pos] == '.') {
				pos += lineLength;
				realDist--;
				while (pos < size) {
					if (str[pos] == '.') {
						pos += lineLength;
						realDist--;
						continue;
					}
					if (str[pos] == '#')
						dist = realDist - 1;
					else /* if (str[pos] == 'O') */ {
						sum += dist;
						dist--;
					}
					realDist--;
					pos += lineLength;
					break;
				}
				continue;
			}
			pos += lineLength;
			dist--;
			realDist--;
		}
	}
	return sum;
}

int solution_1(const string& input) {
	const char* str = input.c_str();
	const size_t size = input.size();
	size_t lineLength = findChar(str, '\n') + 1;
	size_t distanceFromBottom = size / lineLength;
	return getNorthWeight(str, size, lineLength, distanceFromBottom);
}

inline int getWeight(const char* str, size_t size, size_t lineLength, size_t distanceFromBottom) {
	int sum = 0;
	for (size_t i = 0; i < lineLength - 1; i++) {
		size_t pos = i;
		size_t dist = distanceFromBottom;
		while (pos < size) {
			if (str[pos] == 'O')
				sum += dist;
			pos += lineLength;
			dist--;
		}
	}
	return sum;
}

inline void moveBoulders(char* str, size_t size, size_t dir, size_t start, size_t addStart) {
	size_t pos = start;
	while (pos < size && pos >= 0) {
		start += addStart;
		while (pos < size && pos >= 0) {
			if (str[pos] == '.') {
				size_t savePos = pos;
				pos -= dir;
				while (pos < size && pos >= 0) {
					if (str[pos] == '.') {
						pos -= dir;
						continue;
					}
					if (str[pos] == 'O') {
						str[pos] = '.';
						str[savePos] = 'O';
						pos = savePos;
					}
					pos -= dir;
					break;
				}
				continue;
			}
			pos -= dir;
		}
		pos = start;
	}
}


int solution_2(const string& input) {
	const size_t size = input.size();
	size_t lineLength = findChar(input.c_str(), '\n') + 1;
	size_t distanceFromBottom = size / lineLength;

	const int loopAmount = 1000000000;
	unordered_set<string> set;
	vector<string> list;
	string str = input;
	string end;
	for (size_t i = 0; i < loopAmount; i++) {
		moveBoulders(str.data(), size, -lineLength, 0, 1);
		moveBoulders(str.data(), size, -1, 0, lineLength);
		moveBoulders(str.data(), size, lineLength, size - lineLength, 1);
		moveBoulders(str.data(), size, 1, lineLength - 2, lineLength);
		if (set.count(str) == 0) {
			string copy = str;
			set.insert(copy);
			list.push_back(copy);
			continue;
		}
		size_t loopStart = 0;
		while (list[loopStart].compare(str) != 0)
			loopStart++;

		size_t loopSize = (i - loopStart);
		size_t ind = (loopAmount - loopStart - 1) % loopSize;
		end = list[loopStart + ind];
		break;
	}

	int result = getWeight(end.c_str(), size, lineLength, distanceFromBottom);
	return result;
}

int main(int argc, char* argv[]) {
	string input = readInput(14, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10);
	return 0;
}


