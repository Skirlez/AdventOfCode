#include <iostream>
#include <cstring>
#include "Util.h"

using namespace std;

inline int readNumberBackwards(const char* str, int& pos) {
	int num = 0;
	int multiplier = 1;
	while (true) {
		num += (str[pos] - '0') * multiplier;
		pos--;
		if (str[pos] == ',' || str[pos] == ' ')
			return num;

		multiplier *= 10;
	}
}

inline bool isValidArrangement(int64_t arrangement, const int& bits, const vector<int>& hashCounts) {
	int hashesFound = 0;
	int hashIndex = hashCounts.size() - 1;

	for (int i = bits; i >= 0; i--) {
		if ((arrangement & 1) == 0) {
			if (hashesFound != 0) {
				if (hashCounts[hashIndex] != hashesFound)
					return false;
				hashIndex--;
				hashesFound = 0;
			}
			arrangement >>= 1;
			continue;
		}
		hashesFound++;
		arrangement >>= 1;
    }
	return (hashIndex == -1);
}

enum SpringStatus {
	WORKING,
	DAMAGED,
	UNKNOWN
};

class Arrangement {
	public: 
		int64_t springStatus = 0;
		int64_t unknownSprings = 0;
		int64_t bitCount = 0;
		

		Arrangement() {
			springStatus = 0;
			unknownSprings = 0;
			bitCount = 0;
		}
		Arrangement(const Arrangement& other) {
			springStatus = other.springStatus;
			unknownSprings = other.unknownSprings;
			bitCount = other.bitCount;
		}

		void add(SpringStatus status) {
			if (status == SpringStatus::UNKNOWN)
				unknownSprings |= (1 << bitCount);
			else 
				springStatus |= (status << bitCount);
			
			bitCount++;
		}
		void addStart(SpringStatus status) {
			unknownSprings <<= 1;
			springStatus <<= 1;
			if (status == SpringStatus::UNKNOWN)
				unknownSprings |= 1;
			else 
				springStatus |= status;
			
			bitCount++;
		}
		void add(int64_t statusBits, int64_t unknownBits, int newBitCount) {
			springStatus <<= bitCount;
			springStatus |= statusBits;
			unknownSprings <<= bitCount;
			unknownSprings |= unknownBits;

			bitCount += newBitCount;
		}

		void clear() {
			springStatus = 0;
			unknownSprings = 0;
			bitCount = 0;
		}
		void print() {
			for (int i = 0; i < bitCount; i++) {
				if ((unknownSprings & (1 << i)) == (1 << i)) 
					cout << '?';
				else if ((springStatus & (1 << i)) != (1 << i))
					cout << '.';
				else
					cout << '#';

			}
			cout << '\n';
		}
		SpringStatus eatFirst() {
			
			SpringStatus ret;
			if ((unknownSprings & 1) == 1)
				ret = SpringStatus::UNKNOWN;
			else
				ret = (SpringStatus)((springStatus) & 1);
 			bitCount--;
			unknownSprings >>= 1;
			springStatus >>= 1;
			return ret;
		}
};


int getArrangements(Arrangement& arrangement, const vector<int>& hashCounts, int hashIndex, int hashesFound) {
	int arrangements = 0;
	while (arrangement.bitCount > 0) {
		SpringStatus status = arrangement.eatFirst();
		if (status == SpringStatus::WORKING) {
			if (hashesFound == 0)
				continue;
			if (hashCounts[hashIndex] != hashesFound)
				return arrangements;
			hashesFound = 0;
			hashIndex--;
			continue;
		}
		if (status == SpringStatus::DAMAGED)
			hashesFound++;
		else { // unknown
			// pretend it's damaged, send another guy to pretend it's working
			
			Arrangement copy = arrangement;
		
			copy.addStart(SpringStatus::WORKING);
			arrangements += getArrangements(copy, hashCounts, hashIndex, hashesFound);
			if (hashIndex == -1)
				return arrangements;
			hashesFound++;

		}	
		if (hashesFound > hashCounts[hashIndex])
			return arrangements;
	}
	if (hashCounts[hashIndex] == hashesFound)
		hashIndex--;

	if (hashIndex >= 0)
		return arrangements;

	return arrangements + 1;
}

int solution_1(const string& input) {
	int size = input.size();
	char* str = new char[size + 1];
	strncpy(str, input.c_str(), size + 1);

	int sum = 0;

	int pos = 0;
	
	vector<int> hashCounts = vector<int>();
	vector<int> questionIndicies = vector<int>();
	Arrangement a;
	while (pos < size) {
		int lineStartPos = pos;
		while (str[pos] != ' ') {
			if (str[pos] == '?')
				a.add(SpringStatus::UNKNOWN);
			else if (str[pos] == '#')
				a.add(SpringStatus::DAMAGED);
			else
				a.add(SpringStatus::WORKING);
			
			pos++;
		}

		while (str[pos] != '\n')
			pos++;
		
		int endLinePos = pos;

		while (str[pos] != ' ') {
			pos--;
			int num = readNumberBackwards(str, pos);
			hashCounts.push_back(num);
		}
		
		int arrangements = getArrangements(a, hashCounts, hashCounts.size() - 1, 0);
		//cout << arrangements << '\n';
		sum += arrangements;
		hashCounts.clear();
		questionIndicies.clear();
		pos = endLinePos + 1;
		a.clear();
	}
	return sum;
}






int solution_2(const string& input) {
	int size = input.size();
	char* str = new char[size + 1];
	strncpy(str, input.c_str(), size + 1);

	int sum = 0;

	int pos = 0;
	
	vector<int> hashCounts = vector<int>();
	vector<int> questionIndicies = vector<int>();
	
	while (pos < size) {
		int lineStartPos = pos;

		Arrangement a;
		Arrangement b;
		Arrangement c;

		int totalHashes = 0;
		int bits = 0;
		
		while (str[pos] != ' ') {
			//cout << str[pos];
			if (str[pos] == '?') {
				a.add(SpringStatus::UNKNOWN);
				b.springStatus = c.springStatus;
				b.unknownSprings = c.unknownSprings;
				c.clear();
			}
			else if (str[pos] == '#') {
				a.add(SpringStatus::DAMAGED);
				c.add(SpringStatus::DAMAGED);
			}
			else {
				a.add(SpringStatus::WORKING);
				c.add(SpringStatus::WORKING);
			}

			pos++;
		}
		cout << '\n';
		a.print();
		b.print();
		c.print();

		int hashesFound = 0;
		while (a.bitCount != 0) {
			SpringStatus status = a.eatFirst();
			if (status == SpringStatus::WORKING) {
				if (hashesFound == 0)
					continue;

				continue;
			}
			if (status == SpringStatus::UNKNOWN) {
				continue;
			}
			hashesFound++;
		}


		while (str[pos] != '\n')
			pos++;
		int endLinePos = pos;

		while (str[pos] != ' ') {
			pos--;
			int num = readNumberBackwards(str, pos);
			hashCounts.push_back(num);
		}
		
		

		int allArrangements = 1 << size;
		
		int pb = 0;
		int pcqb = 0;
		int pcqa = 0;
		/*
		int64_t bArrangement = arrangement;
		for (int i = 0; i < allArrangements; i++) {
			int valueSums = 0;
			for (int j = 0; j < size; j++) {
				int value = (i >> j) & 1;
				arrangement |= (value << questionIndicies[j]);
				valueSums += value;
			}
			//printArrangement(arrangement, bits);
			if (valueSums == requiredHashes)
				pb += isValidArrangement(arrangement, bits, hashCounts);			
			arrangement = bArrangement;
		}
		*/

		sum += pb * pcqb*pcqb*pcqb * pcqa;
		hashCounts.clear();
		pos = endLinePos + 1;
	}
	return sum;

	return 0;
}


int main(int argc, char* argv[]) {
	string input = readInput(12, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100);
	
	//cout << '\n';

	//cout << "Part 2: " << solution_2(input) << '\n';
	//timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}


