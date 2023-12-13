#include <iostream>
#include "Util.h"

using namespace std;

inline void printChain(int64_t chain) {
	for (int i = 0; i < 32; i++) {
		cout << (chain & 1);
		chain >>= 1;
	}
	cout << '\n';
}

inline int findReflectionInVector(const vector<int64_t>& vec, const size_t& size) {
	for (int i = 0; i < size - 1; i++) {
		if (vec[i] == vec[i + 1]) {
			int reflectionStart = i;
			
			i += 2;
			if (i >= size) {
				return (reflectionStart + 1);
			}
			int j;
			for (j = reflectionStart - 1; j >= 0; j--) {
				if (vec[j] != vec[i]) {
					i = reflectionStart;
					break;
				}
				i++;
				if (i >= size)
					return (reflectionStart + 1);
			}
			if (j == -1)
				return (reflectionStart + 1);
		}
	}
	return 0;
}

inline int findReflection(const vector<int64_t>& rows, const size_t& rowsSize, const vector<int64_t>& columns, const size_t& columnsSize) {
	int result = findReflectionInVector(rows, rowsSize);
	if (result != 0)
		return result * 100;
	return findReflectionInVector(columns, columnsSize);
}

int solution_1(const string& input) {
	const char* str = input.c_str();
	size_t size = input.size();
	size_t i = 0;
	int sum = 0;
	vector<int64_t> rows;
	vector<int64_t> columns;
	size_t columnsSize = 0;
	size_t rowsSize = 0;
	while (i < size) {
		int j = i;
		int count = 0;
		
		while (str[j] != '\n')  {
			if (count >= columns.size()) {
				columns.push_back(0);
				columnsSize++;
			}
			j++;
			count++;
		}

		int columnIndex = 0;
		int64_t row = 0;
		while (str[i] != '\n') {
			// '#' = 35 and '.' = 46, we can just take the last bit to differenciate
			row <<= 1;
			row |= (str[i] & 1);

			columns[columnIndex] <<= 1;
			columns[columnIndex] |= (str[i] & 1);
			columnIndex++;
			i++;
		}
		rows.push_back(row);
		rowsSize++;
		i++;
		if ((i == size) || str[i] == '\n') {
			sum += findReflection(rows, rowsSize, columns, columnsSize);
			rows.clear();
			columns.clear();
			columnsSize = 0;
			rowsSize = 0;
			i++;
			continue;
		}
	}
	return sum;
}


bool oneBitDifference(int64_t a, int64_t b) {
	int64_t diff = a ^ b;
	return ((diff & (diff - 1)) == 0);
}

inline int findSmudgeInVector(const vector<int64_t>& vec, const size_t& size) {
	bool fixSmudge = false;
	for (int i = 0; i < size - 1; i++) {
		if (vec[i] == vec[i + 1] || oneBitDifference(vec[i], vec[i + 1])) {
			if (vec[i] != vec[i + 1])
				fixSmudge = true;
			
			int reflectionStart = i;
			i += 2;
			if (i >= size) {
				if (fixSmudge)
					return (reflectionStart + 1);
				continue;
			}
			int j;
			for (j = reflectionStart - 1; j >= 0; j--) {
				if (vec[j] != vec[i]) {
					if (!fixSmudge && oneBitDifference(vec[j], vec[i]))
						fixSmudge = true;
					else {
						i = reflectionStart;
						break;
					}
				}
				i++;
				if (i >= size) {
					if (fixSmudge)
						return (reflectionStart + 1);
					i = reflectionStart;
					break;
				}
			}
			if (j == -1) {
				if (fixSmudge)
					return (reflectionStart + 1);
				i = reflectionStart;
			}
			fixSmudge = false;
		}
	}
	return 0;
}

inline int findSmudge(const vector<int64_t>& rows, const vector<int64_t>& columns) {
	int result = findSmudgeInVector(rows);
	if (result != 0)
		return result * 100;
	return findSmudgeInVector(columns);
}

int solution_2(const string& input) {
	const char* str = input.c_str();
	size_t size = input.size();
	size_t i = 0;
	int sum = 0;
	vector<int64_t> rows;
	vector<int64_t> columns;
	size_t columnsSize = 0;
	size_t rowsSize = 0;
	while (i < size) {
		int j = i;
		int count = 0;
		while (str[j] != '\n')  {
			if (count >= columns.size()) {
				columns.push_back(0);
				columnsSize++;
			}
			j++;
			count++;
		}

		int columnIndex = 0;
		int64_t row = 0;
		while (str[i] != '\n') {
			// '#' = 35 and '.' = 46, we can just take the last bit to differenciate
			row <<= 1;
			row |= (str[i] & 1);

			columns[columnIndex] <<= 1;
			columns[columnIndex] |= (str[i] & 1);
			columnIndex++;
			i++;
		}
		rows.push_back(row);
		rowsSize++;
		i++;
		if ((i == size) || str[i] == '\n') {
			sum += findSmudge(rows, columns);
			rows.clear();
			columns.clear();
			i++;
			continue;
		}
	}
	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(13, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}
