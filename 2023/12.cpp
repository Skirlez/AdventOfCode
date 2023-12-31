#include <iostream>
#include <cstring>
#include "Util.h"
#include <algorithm>

using namespace std;

inline int readNumberBackwards(const char* str, size_t& pos) {
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

enum SpringStatus {
	WORKING,
	DAMAGED,
	UNKNOWN
};

template <typename T> class Arrangement {
	public: 
		T springStatus;
		T unknownSprings;
		int64_t bitCount = 0;
		
		T one = (T)1;

		Arrangement() {
			springStatus = 0;
			unknownSprings = 0;
			bitCount = 0;
		}

		void add(SpringStatus status) {
			T bits = (T)(status);
			if (status == SpringStatus::UNKNOWN)
				unknownSprings |= (one << bitCount);
			else 
				springStatus |= (bits << bitCount);
			
			bitCount++;
		}
		void clear() {
			springStatus = 0;
			unknownSprings = 0;
			bitCount = 0;
		}
		void print() {
			for (int i = 0; i < bitCount; i++) {
				

				if ((unknownSprings & one) == one) 
					cout << '?';
				else if ((springStatus & one) != one)
					cout << '.';
				else
					cout << '#';

			}
			cout << '\n';
		}
		
		SpringStatus getStatusAt(int ind) const {
			if ((unknownSprings & (one << ind)) != 0)
				return SpringStatus::UNKNOWN;
			else
				return (SpringStatus)(((springStatus) & (one << ind)) != 0);
		}
};

vector<vector<vector<int64_t>>> cache;

template<typename T, bool DoCache> int64_t getArrangements(const Arrangement<T>& arrangement, const vector<int>& hashCounts, int hashIndex, int hashesFound, int amountEaten) {
	int64_t arrangements = 0;
	bool entry = true;
	while (arrangement.bitCount > amountEaten) {
		SpringStatus status;
		if (!entry) {
			status = arrangement.getStatusAt(amountEaten);
			amountEaten++;
		}
		if (entry || status == SpringStatus::WORKING) {
			entry = false;
			if (hashesFound == 0)
				continue;
			if (hashCounts[hashIndex] != hashesFound)
				return arrangements;
			hashesFound = 0;
			hashIndex--;
			continue;
		}	
		else if (status == SpringStatus::UNKNOWN) {
			// pretend it's damaged, send another guy to pretend it's working
			if (DoCache) {
				int64_t add;
				if (cache[hashIndex + 1][amountEaten][hashesFound] == -1) {
					add = getArrangements<T, true>(arrangement, hashCounts, hashIndex, hashesFound, amountEaten);
					cache[hashIndex + 1][amountEaten][hashesFound] = add;
				}
				else
					add = cache[hashIndex + 1][amountEaten][hashesFound];
				arrangements += add;
			}
			else
				arrangements += getArrangements<T, false>(arrangement, hashCounts, hashIndex, hashesFound, amountEaten);	
		}	
		hashesFound++; // either we found a hash or we're pretending there is one
		if ((hashIndex == -1) || (hashesFound > hashCounts[hashIndex]))
			return arrangements;
	}

	if (hashCounts[hashIndex] == hashesFound)
		hashIndex--;
	if (hashIndex >= 0)
		return arrangements;

	return arrangements + 1;
}

int solution_1(const string& input) {
	size_t size = input.size();
	const char* str = input.c_str();

	int sum = 0;
	size_t pos = 0;
	
	vector<int> hashCounts = vector<int>();
	Arrangement<int32_t> a;
	while (pos < size) {
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
		int arrangements = getArrangements<int32_t, false>(a, hashCounts, hashCounts.size() - 1, 0, 0);

		//cout << arrangements << '\n';
		sum += arrangements;
		hashCounts.clear();
		pos = endLinePos + 1;
		a.clear();
	}
	return sum;
}

int64_t solution_2(const string& input) {
	size_t size = input.size();
	const char* str = input.c_str();

	size_t pos = 0;
	int64_t sum = 0;
	
	vector<int> hashCounts = vector<int>();
	Arrangement<__uint128_t> a;
	
	while (pos < size) {
		int lineStartPos = pos;
		for (int j = 0; j < 5; j++) {
			pos = lineStartPos;
			if (j != 0)
				a.add(SpringStatus::UNKNOWN);
			while (str[pos] != ' ') {
				if (str[pos] == '?')
					a.add(SpringStatus::UNKNOWN);
				else if (str[pos] == '#')
					a.add(SpringStatus::DAMAGED);
				else
					a.add(SpringStatus::WORKING);
				
				pos++;
			}
		}
		while (str[pos] != '\n')
			pos++;
		
		int endLinePos = pos;
		for (int j = 0; j < 5; j++) {
			pos = endLinePos;
			while (str[pos] != ' ') {
				pos--;
				int num = readNumberBackwards(str, pos);
				hashCounts.push_back(num);
			}
		}

		for (size_t i = cache.size(); i < hashCounts.size() + 1; i++)
			cache.push_back(vector<vector<int64_t>>());
		
		for (size_t i = 0; i < cache.size(); i++) {
			if (cache[i].size() < a.bitCount + 1)
				cache[i].resize(a.bitCount + 1);
			vector<int64_t> vec = vector<int64_t>(a.bitCount + 1, -1);
			for (size_t j = 0; j <= a.bitCount; j++) {
				cache[i][j] = vec;
			}
		}
	
		int64_t arrangements = getArrangements<__uint128_t, true>(a, hashCounts, hashCounts.size() - 1, 0, 0);
		sum += arrangements;
		hashCounts.clear();
		pos = endLinePos + 1;
		a.clear();
	}
	return sum;
}


int main(int argc, char* argv[]) {
	string input = readInput(12, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 10);
	return 0;
}


