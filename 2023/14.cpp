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

inline void moveBoulders(char* str, const size_t& size, const size_t& dir, size_t start, 
		const size_t& addStart, const size_t& iterations, const size_t& subIterations) {
	
	size_t pos = start;
	int dotArray[subIterations]; 
	int dotIndex = 0;
	int dotArraySize = 0;
	for (size_t i = 0; i < iterations; i++) {
		start += addStart;
		for (size_t j = 0; j < subIterations; j++) {
			if (str[pos] == '.') {
				dotArray[dotArraySize] = pos;
				dotArraySize++;
			}
			else if (str[pos] == '#') {
				dotIndex = 0;
				dotArraySize = 0;
			}
			else if (str[pos] == 'O' && ((dotArraySize - dotIndex) > 0)) {
				str[dotArray[dotIndex]] = 'O';
				str[pos] = '.';
				dotArray[dotArraySize] = pos;
				dotArraySize++;
				dotIndex++;
			}
			pos -= dir;
		}
		pos = start;
		dotIndex = 0;
		dotArraySize = 0;
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
	char* data = str.data();
	for (size_t i = 0; i < loopAmount; i++) {
		moveBoulders(data, size, -lineLength, 0, 1, lineLength - 1, distanceFromBottom);
		moveBoulders(data, size, -1, 0, lineLength, distanceFromBottom, lineLength - 1);
		moveBoulders(data, size, lineLength, size - lineLength, 1, lineLength - 1, distanceFromBottom);
		moveBoulders(data, size, 1, lineLength - 2, lineLength, distanceFromBottom, lineLength - 1);
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
	timeFunctionAndPrint(solution_2, input, 100);
	return 0;
}


