#include <iostream>
#include "Util.h"

using namespace std;
/* Old solution, pre part 2; it's slower.
void printGrid(const vector<vector<char>>& grid) {
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			cout << grid[i][j];
		}
		cout << '\n';
	}
}
int solution_1(const string& input) {
	int sum = 0;
	const char* str = input.c_str();
	int lineLength = findChar(str, '\n') + 1;

	// transform the input string into a grid vector while also scaling it up in the process

	vector<vector<char>> grid = vector<vector<char>>();
	int originalLines = (input.size()) / (lineLength);
	int lines = originalLines;

	vector<int> emptyColumnSums = vector<int>(lineLength);
	grid.reserve(lines);
	grid.push_back(vector<char>());
	grid[0].reserve(lineLength);
	int ind = 0;
	vector<pair<int, int>> galaxies;


	bool allEmpty = true;
	for (int i = 0; i < input.size() - 1; i++) {
		if (str[i] == '\n') {
			if (allEmpty) {
				ind++;
				lines++;
				grid.push_back(vector<char>(lineLength - 1, '.'));
			}
			else
				allEmpty = true;
			ind++;
			grid.push_back(vector<char>());
			grid[ind].reserve(lineLength - 1);
		}
		else {
			grid[ind].push_back(str[i]);
			int x = i % lineLength;
			if ((str[i] == '.'))
				emptyColumnSums[x]++;
			else {
				allEmpty = false;
				galaxies.push_back(pair(x, ind));
			}
		}
	}
	vector<int> insertPositions;
	for (int i = 0; i < emptyColumnSums.size(); i++) {
		if (emptyColumnSums[i] == originalLines) {
			insertPositions.push_back(i + 1);
		}
	}
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < insertPositions.size(); j++) {
			grid[i].insert(grid[i].begin() + insertPositions[j] + j, '.');
		}
	}
	for (int j = 0; j < insertPositions.size(); j++) {
		for (int g = 0; g < galaxies.size(); g++) {
			if (galaxies[g].first >= insertPositions[j] + j)
				galaxies[g].first++;
		}
	}


	for (int i = 0; i < galaxies.size(); i++) {
		int x = galaxies[i].first;
		int y = galaxies[i].second;
		for (int j = 0; j < galaxies.size(); j++) {
			sum += abs(x - galaxies[j].first) + abs(y - galaxies[j].second);
		}
	}
	//cout << '\n';
	//printGrid(grid);
	return sum / 2;
}
*/
int64_t sharedSolution(const string& input, const int& expansionSize) {
	const char* str = input.c_str();
	int lineLength = findChar(str, '\n') + 1;

	int lines = (input.size()) / (lineLength);

	vector<int> emptyColumnSums = vector<int>(lineLength);
	vector<pair<int, int>> galaxies;
	int x = 0;
	int y = 0;
	bool allEmpty = true;
	for (size_t i = 0; i < input.size() - 1; i++) {
		if (str[i] == '\n') {
			if (allEmpty)
				y += expansionSize - 1;
			else
				allEmpty = true;
			y++;
			x = 0;
		}
		else {
			if ((str[i] == '.'))
				emptyColumnSums[x]++;
			else {
				allEmpty = false;
				galaxies.push_back(pair(x, y));
			}
			x++;
		}
	}
	int galaxiesAmount = galaxies.size();
	int offset = 0;
	for (size_t i = 0; i < emptyColumnSums.size(); i++) {
		if (emptyColumnSums[i] == lines) {
			int insertPos = i + 1;
			for (size_t g = 0; g < galaxiesAmount; g++) {
				if (galaxies[g].first >= insertPos + offset)
					galaxies[g].first += expansionSize - 1;
			}
			offset += expansionSize - 1;
		}
	}
	int64_t sum = 0;
	for (size_t i = 0; i < galaxiesAmount; i++) {
		int x = galaxies[i].first;
		int y = galaxies[i].second;
		for (size_t j = i + 1; j < galaxiesAmount; j++)
			sum += abs(x - galaxies[j].first) + abs(y - galaxies[j].second);
	}

	return sum;
}

int solution_1(const string& input) {
	return sharedSolution(input, 2);
}
int64_t solution_2(const string& input) {
	return sharedSolution(input, 1000000);
}

int main(int argc, char* argv[]) {
	string input = readInput(11, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 10000);
	return 0;
}


