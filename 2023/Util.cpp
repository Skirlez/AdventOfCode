#include "Util.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

vector<string> splitString(const string& str, char splitter) {
	vector<string> vec = vector<string>();

	int last = 0;
	int size = str.size();
	int i;
	for (i = 0; i < size; i++) {
		if (str[i] == splitter) {
			vec.push_back(str.substr(last, i - last));
			last = i + 1;
		}
	}
	string leftover = str.substr(last, i - last);
	if (!leftover.empty())
		vec.push_back(str.substr(last, i - last));

	return vec;
}


int stringToInt(string str) {
	int num = 0;
	int multiplier = 1;
	for (int i = str.size() - 1; i >= 0; i--) {
		int digit = str[i] - '0';
		num += digit * multiplier;
		multiplier *= 10;
	}
	return num;
}


unsigned long long timeFunction(SolutionFunction func, string input, const int iterations) {
	auto start = std::chrono::high_resolution_clock::now();
	for (int m = 0; m < iterations; m++)
		func(input);
	auto end = std::chrono::high_resolution_clock::now();
	unsigned long long diff = chrono::duration_cast<chrono::nanoseconds>(end - start).count() / iterations;
	return diff;
}

string nanoToMicro(unsigned long long num) {
    string resultString = ((ostringstream()) << num).str();
	int pos = resultString.length() - 3;
	while (pos < 0) {
		resultString.insert(0, "0");
		pos++;
	}
	if (pos == 0) {
		resultString.insert(0, "0");
		pos++;
	}
	if (pos >= 5) {
		resultString = resultString.substr(0, resultString.size() - 3);
	}
	else {
		resultString.insert(pos, ".");
		resultString = resultString.substr(0, 6);
	}
    return resultString;
}

void timeFunctionAndPrint(SolutionFunction func, string input, const int iterations) {
	cout << "Timing function (running " << iterations << " iterations...)" << '\n'; 
	unsigned long long executionTime = timeFunction(func, input, iterations);
	cout << "The function took " << nanoToMicro(executionTime) << " microseconds to run on average from " 
		<< iterations << " iterations." << '\n';

	if (executionTime != 0) {
		#define blinkTime 100000000
		unsigned long long runs = blinkTime / executionTime;
		cout << "The smallest accepted average duration for blinking is 100 milliseconds." 
			<< " The function would run " << runs << " times in one blink." << '\n';
	}
}


string readInput(int day, char* argv[]) {
	if (argv[1] == nullptr) {
		cerr << "Error: Please provide the path to the inputs folder" << '\n';
		exit(EXIT_FAILURE);
	}
	ifstream inputFile = ifstream((stringstream() << argv[1] << '\\' << day << ".txt").str());
    if (!inputFile.is_open()) {
		if (day >= 10) {
			cerr << "Error: input file not found. Please name it " << day << ".txt and place it in the provided folder." << '\n';
			exit(EXIT_FAILURE);
		}
        inputFile = ifstream((stringstream() << argv[1] << '\\' << '0' << day << ".txt").str());
		if (!inputFile.is_open()) {
			cerr << "Error: input file not found. Please name it either " << day << ".txt or 0" << day << ".txt and place it in the provided folder." << '\n';
        	exit(EXIT_FAILURE);
		}
    }

	string fileContent = string((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

	// idiot detector
	while (fileContent.back() == '\n')
		fileContent.erase(fileContent.size() - 1);
	fileContent.push_back('\n');

	inputFile.close();
	return fileContent;
}