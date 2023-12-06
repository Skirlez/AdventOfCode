#include <iostream>
#include "Util.h"

using namespace std;

#define numtype unsigned long long

inline numtype getLocation(numtype number, const vector<vector<numtype>*>& mapVector) {
	for (int i = 0; i < mapVector.size(); i++) {
		vector<numtype>& nums = *mapVector[i];
		for (int j = 0; j < nums.size(); j += 3) {
			numtype sourceStart = nums[j + 1];
			numtype count = nums[j];
			if (number >= sourceStart && number <= sourceStart + count) {
				numtype destStart = nums[j + 2];
				number = destStart + (number - sourceStart);
				break;
			}
		}
	}
	return number;
}

inline numtype readNumberBackwardsUntilSpace(string& str, int& j) {
	numtype multiplier = 1;
	numtype num = 0;
	while (str[j] != ' ') {
		num += (str[j] - '0') * multiplier;
		multiplier *= 10;
		j--;
		if (j == -1)
			return num;
	}
	j--;
	return num;
}

inline vector<vector<numtype>*> createMapVector(vector<string>& vec) {
	vector<vector<numtype>*> mapVector = vector<vector<numtype>*>();
	int mapCount = 0;
	int vecSize = vec.size();
	int i = 3;
	while (i < vecSize) {
		string& line = vec[i];
		if (line.empty()) {
			mapCount++;
			i += 2;
			continue;
		}
		int j = vec[i].size() - 1;
		numtype count = readNumberBackwardsUntilSpace(line, j);
		numtype sourceStart = readNumberBackwardsUntilSpace(line, j);
		numtype destStart = readNumberBackwardsUntilSpace(line, j);

		vector<numtype>* newVec;
		if (mapVector.size() == mapCount) {
			newVec = new vector<numtype>();
			mapVector.push_back(newVec);
		}
		else
			newVec = mapVector[mapCount];

		newVec->push_back(count);
		newVec->push_back(sourceStart);
		newVec->push_back(destStart);
		i++;
	}

	return mapVector;
}


int solution_1(const string& input) {
	vector<string> vec = splitString(input, '\n');
	vector<vector<numtype>*> mapVector = createMapVector(vec);
	
	// get the seed numbers
	const char* seedsLine = vec[0].c_str();
	// the most efficient way to read the numbers is to start from the end
	numtype multiplier = 1;
	numtype num = 0;
	numtype minLocation = 0;
	for (int i = vec[0].size() - 1; i > 5; i--) { // numbers end at index 5
		int digit = seedsLine[i];
		if (digit == ' ') {
			numtype location = getLocation(num, mapVector);
			minLocation = (minLocation == 0) ? location : min(location, minLocation);
			num = 0;
			multiplier = 1;
			continue;
		}
		num += ((digit - '0') * multiplier);
		multiplier *= 10;
	} 

	for (int i = 0; i < mapVector.size(); i++)
		delete mapVector[i];
	

	return minLocation;
}

int numtype getMinimumLocationRanged(numtype start, numtype end, int mapInd, int jStart, const vector<vector<numtype>*>& mapVector) {
	if (mapInd == mapVector.size())
		return start;
	
	vector<numtype>& nums = *mapVector[mapInd];
	numtype location = -1;
	for (int j = jStart; j < nums.size(); j += 3) {
		numtype destStart = nums[j + 2];
		numtype sourceStart = nums[j + 1];
		numtype rangesCount = nums[j];
		numtype sourceEnd = sourceStart + rangesCount;

		if (start >= sourceStart) {
			if (start <= sourceEnd) {
				if (end > sourceEnd) {
					location = min(location, getMinimumLocationRanged(sourceEnd + 1, end, mapInd, j, mapVector));
					end = sourceEnd;
				}
				long long offset = (destStart - sourceStart);
				return min(location, getMinimumLocationRanged(start + offset, end + offset, mapInd + 1, 0, mapVector));
			}
		}
		else if (end >= sourceStart) {
			if (end > sourceEnd) {
				location = min(location, getMinimumLocationRanged(sourceEnd + 1, end, mapInd, j, mapVector));
				end = sourceEnd;
			}
			long long offset = (destStart - sourceStart);
			location = min(location, getMinimumLocationRanged(sourceStart + offset, end + offset, mapInd + 1, 0, mapVector));
			end = sourceStart - 1;
		}
		
	}
	return min(location, getMinimumLocationRanged(start, end, mapInd + 1, 0, mapVector));
}



int solution_2(const string& input) {
	vector<string> vec = splitString(input, '\n');
	vector<vector<numtype>*> mapVector = createMapVector(vec);
	// get the seed numbers
	string& seedsLine = vec[0];
	// the most efficient way to read the numbers is to start from the end
	
	numtype num = 0;
	numtype minLocation = -1;
	int i = vec[0].size() - 1;
	while (i > 5) { // numbers end at index 5
		int digit = seedsLine[i];
		numtype count = readNumberBackwardsUntilSpace(seedsLine, i);
		numtype start = readNumberBackwardsUntilSpace(seedsLine, i);
		numtype location = getMinimumLocationRanged(start, start + count, 0, 0, mapVector);
		minLocation = min(location, minLocation);
	} 

	for (int i = 0; i < mapVector.size(); i++)
		delete mapVector[i];
	

	return minLocation;
}

int main(int argc, char* argv[]) {
	string input = readInput(5, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 100000);
	return 0;
}


