#include <iostream>
#include "Util.h"
#include <cstring>

using namespace std;

inline int findChar(const char* str, char c) {
	int i = 0;
	while (str[i] != c) 
		i++;
	return i;
}

int roundSin(const int& angle) {
	if (angle == 90)
		return 1;
	if (angle == 270)
		return -1;
	return 0;
}
int roundCos(const int& angle) {
	if (angle == 0)
		return 1;
	if (angle == 180)
		return -1;
	return 0;
}

inline void wrapAroundAngle(int& angle) {
	while (angle < 0)
		angle += 360;
	while (angle >= 360)
		angle -= 360;
	
}

inline bool advanceMaze(const char* str, const int& lineLength, int& i, int& angle) {
	i += (-roundSin(angle) * lineLength) + roundCos(angle);
	int angleMulti = (roundCos(angle) != 0) * 2 - 1; // if moving horizontally, 1, otherwise, -1
	const char& next = str[i];
	if (next == 'S')
		return true;
	if (next == 'J' || next == 'F')
		angle += 90 * angleMulti;
	else if (next == 'L' || next == '7')
		angle -= 90 * angleMulti;
	return false;
}

inline int findStartingAngle(const char* str, const int& lastInd, const int& lineLength, const int& i) {
	if (i + lineLength <= lastInd) {
		if (str[i + lineLength] == '|' || str[i + lineLength] == 'J' || str[i + lineLength] == 'L')
			return 270;
		if ((i + 1 <= lastInd) && (str[i + 1] == '-' || str[i + 1] == 'J' || str[i + 1] == '7'))
			return 0;
	}
	if (i - lineLength >= 0) {
		if (str[i - lineLength] == '|' || str[i - lineLength] == 'F' || str[i - lineLength] == '7')
			return 90;
		if ((i - 1 >= 0) && (str[i - 1] == '-' || str[i + 1] == 'F' || str[i + 1] == 'L'))
			return 180;
	}
	return 0;
}

int solution_1(const string& input) {
	const char* str = input.c_str();
	int lineLength = findChar(str, '\n') + 1;
	int i = findChar(str, 'S');
	int angle = findStartingAngle(str, input.size() - 1, lineLength, i);
	int count = 0;
	while (true) {
		bool done = advanceMaze(str, lineLength, i, angle);
		wrapAroundAngle(angle);
		count++;
		if (done)
			break;
	}
	

	return count / 2;
}

void floodFill(char* str, const int& pos, const int& lineLength, const int& lastInd) {
	char c = str[pos];
	if (c == '.') {
		str[pos] = 'O';
		if (pos + lineLength <= lastInd) {
			floodFill(str, pos + lineLength, lineLength, lastInd);
			if (pos <= lastInd)
				floodFill(str, pos + 1, lineLength, lastInd);
		}
		if (pos - lineLength >= 0) {
			floodFill(str, pos - lineLength, lineLength, lastInd);
			if (pos >= 0)
				floodFill(str, pos - 1, lineLength, lastInd);
		}
	}
}


int solution_2(const string& input) {
	
	const char* str = input.c_str();
	int size = input.size();
	char* copy = new char[size + 1];
	strncpy(copy, str, size + 1);

	int lineLength = findChar(str, '\n') + 1;
	int i = findChar(str, 'S');

	for (int j = 0; j < size; j++) {
		if (copy[j] != '\n')
			copy[j] = '.';
	}
	int startingAngle = findStartingAngle(str, input.size() - 1, lineLength, i);
	int angle = startingAngle;
	int leftRightSum = 0;
	while (true) {
		int prevAngle = angle;
		bool done = advanceMaze(str, lineLength, i, angle);
		int diff = angle - prevAngle;
		// 90 - 0 left
		if (diff < 0)
			leftRightSum += 1;
		else if (diff > 0)
			leftRightSum -= 1;
		wrapAroundAngle(angle);
		copy[i] = str[i];
		if (done)
			break;
	}
	int dir = (leftRightSum > 0) ? -1 : 1;

	/* While sketching the problem I noticed that if you pick a direction perpendicular to the angle you're exploring the loop,
	you will either only ever see tiles inside the loop or tiles outside the loop. I initially thought to try checking if you can
	reach the edges to determine if the tiles we just found are inside or outside the loop, but a better way is to count the amount of left
	and right turns: if there's more right turns than left, look right, otherwise look left. 
	From there we just floodfill every point we see. */

	angle = startingAngle;
	while (true) {
		int prevAngle = angle;
		bool done = advanceMaze(str, lineLength, i, angle);
		wrapAroundAngle(angle);
		if (prevAngle == angle) {
			int sideAngle = angle + 90 * dir;
			wrapAroundAngle(sideAngle);
			int floodStart = i + (-roundSin(sideAngle) * lineLength) + roundCos(sideAngle);
			floodFill(copy, floodStart, lineLength, size - 1);
		}
		else {
			int prevSideAngle = prevAngle + 90 * dir;
			int sideAngle = angle + 90 * dir;
			wrapAroundAngle(sideAngle);
			wrapAroundAngle(prevSideAngle);
			int floodStart1 = i + (-roundSin(prevSideAngle) * lineLength) + roundCos(prevSideAngle);
			int floodStart2 = i + (-roundSin(sideAngle) * lineLength) + roundCos(sideAngle);
			floodFill(copy, floodStart1, lineLength, size - 1);
			floodFill(copy, floodStart2, lineLength, size - 1);
		}
		if (done)
			break;
	}

	int count = 0;
	for (int j = 0; j < size; j++) {
		if (copy[j] == 'O')
			count++;
	}
	delete[] copy;
	return count;
}

int main(int argc, char* argv[]) {
	string input = readInput(10, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}


