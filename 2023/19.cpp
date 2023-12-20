// This is a "Predicate" from "Java"

#include <iostream>
#include <cstring>
#include "Util.h"

using namespace std;

struct Rule {
	int varInd;
	bool greaterThan;
	int num = 0;
	int destSize;
	uint16_t destination = 0;
};


#define Workflow vector<Rule>


inline int readWorkflowNameUntil(const char* str, size_t& pos, uint16_t& outName, const char& until) {
	int size = 0;
	do {
		//cout << str[pos];
		outName <<= 5;
		outName |= (str[pos] - 'a');
		pos++;
		size++;
	} while (str[pos] != until);
	
	
	return size;
}

void getAllCombinations(Workflow** workflows, const Workflow& workflow, int64_t start[4], int64_t end[4], int64_t& sum) {
	for (Rule rule : workflow) {
		int varInd = rule.varInd;
		if (varInd == -1 
				|| (rule.greaterThan && rule.num < start[varInd]) 
				|| (!rule.greaterThan && rule.num > end[varInd])) {
			if (rule.destSize == 1) {
				if (rule.destination == 65504) {
					int64_t sums[4] = {0, 0, 0, 0};
					for (int i = 0; i < 4; i++)
						sums[i] = (end[i] - start[i] + 1);
					sum += sums[0] * sums[1] * sums[2] * sums[3];
				}
				return;
			}
			Workflow& newWorkflow = workflows[rule.destSize - 2][rule.destination];
			getAllCombinations(workflows, newWorkflow, start, end, sum);
			return;
		}
		if ((!rule.greaterThan && rule.num < start[varInd]) 
			|| (rule.greaterThan && rule.num > end[varInd])) {
			continue;
		}
		
		int64_t newStart[4];
		int64_t newEnd[4];
		memcpy(newStart, start, 4 * sizeof(int64_t));
		memcpy(newEnd, end, 4 * sizeof(int64_t));

		if (rule.greaterThan) {
			newStart[varInd] = rule.num + 1;
			end[varInd] = rule.num;
		}
		else {
			newEnd[varInd] = rule.num - 1;
			start[varInd] = rule.num;
		}

		if (rule.destSize == 1) {
			if (rule.destination == 65504) {
				int64_t sums[4] = {0, 0, 0, 0};
				for (int i = 0; i < 4; i++)
					sums[i] = (newEnd[i] - newStart[i] + 1);
				sum += sums[0] * sums[1] * sums[2] * sums[3];
			}
			continue;
		}
		Workflow& newWorkflow = workflows[rule.destSize - 2][rule.destination];
		getAllCombinations(workflows, newWorkflow, newStart, newEnd, sum);
	
	}
}



const uint16_t in = ((('i' - 'a') << 5) | ('n' - 'a'));
template<bool Part2> int64_t sharedSolution(const string& input) {
	// Returning trick from Day 8
	//const uint16_t max1 = ('z' - 'a');
	const uint16_t max2 = (('z' - 'a') << 5 | ('z' - 'a'));
	const uint16_t max3 = (('z' - 'a') << 10 | max2);

	Workflow** workflows = new Workflow*[3];
	workflows[0] = new Workflow[max2 + 1];
	workflows[1] = new Workflow[max3 + 1];

	int64_t sum = 0;

	const char* str = input.c_str();
	size_t size = input.size();
	size_t pos = 0;

	do {
		uint16_t name = 0;
		int size = readWorkflowNameUntil(str, pos, name, '{');
		pos++;

		Workflow* workflowPtr = new Workflow();
		Workflow& workflow = *workflowPtr;
		while (true) {
			char compareChar = str[pos + 1];
			if (compareChar != '>' && compareChar != '<') {
				uint16_t destination = 0;
				int destSize = readWorkflowNameUntil(str, pos, destination, '}');
				workflow.push_back(Rule {-1, false, 0, destSize, destination});
				break;
			}

			Rule rule;
			if (str[pos] == 'x')
				rule.varInd = 0;
			else if (str[pos] == 'm')
				rule.varInd = 1;
			else if (str[pos] == 'a')
				rule.varInd = 2;
			else
				rule.varInd = 3;
			pos += 2;
			
			rule.greaterThan = (compareChar == '>');

			while (str[pos] != ':')
				pos++;
			
			int afterColon = pos + 1;
			pos--;
			int multiplier = 1;
			while (str[pos] != compareChar) {
				rule.num += (str[pos] - '0') * multiplier;
				multiplier *= 10;
				pos--;
			}
			pos = afterColon;
			rule.destSize = readWorkflowNameUntil(str, pos, rule.destination, ',');

			workflow.push_back(rule);
			pos++;
		}
		workflows[size - 2][name] = workflow;	
		pos += 2; // jump over '}' and '\n'
	} while (str[pos] != '\n'); 

	if (!Part2) {
		while (pos < size) {
			pos += 3;
			int vars[4] = {0, 0, 0, 0};
			for (int i = 0; i < 3; i++) {
				while (str[pos] != ',')
					pos++;
				size_t afterComma = pos + 1;
				pos--;
				int multiplier = 1;
				while (str[pos] != '=') {
					vars[i] += (str[pos] - '0') * multiplier;
					multiplier *= 10;
					pos--;
				}

				pos = afterComma;
			}

			while (str[pos] != '}')
				pos++;
			size_t afterBracket = pos + 1;
			pos--;
			int multiplier = 1;
			while (str[pos] != '=') {
				vars[3] += (str[pos] - '0') * multiplier;
				multiplier *= 10;
				pos--;
			}

			
			int sizeInd = 0;
			uint16_t name = in;
			bool working = true;
			while (working) {
				Workflow& workflow = workflows[sizeInd][name];
				for (Rule rule : workflow) {
					if (rule.varInd == -1 || (rule.greaterThan && vars[rule.varInd] > rule.num) || (!rule.greaterThan && vars[rule.varInd] < rule.num)) {
						if (rule.destSize == 1)  {
							if (rule.destination == 65504) // 'A'
								sum += vars[0] + vars[1] + vars[2] + vars[3];
							
							working = false;
							break;
						}
						sizeInd = rule.destSize - 2;
						name = rule.destination;
						break;
					}
				}
			}


			pos = afterBracket;
			pos += 2;
		} 
	}
	else {
		// Ranges again?? I had enough of this from day 5...
		Workflow workflow = workflows[0][in];
	
		int64_t start[4] = {1, 1, 1, 1};
		int64_t end[4] = {4000, 4000, 4000, 4000};
		getAllCombinations(workflows, workflow, start, end, sum);
	}
	delete[] workflows[0];
	delete[] workflows[1];
	delete[] workflows;
	return sum;
}


int solution_1(const string& input) {
	return sharedSolution<false>(input);
}

int64_t solution_2(const string& input) {
	return sharedSolution<true>(input);
}

int main(int argc, char* argv[]) {
	string input = readInput(19, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 10000);
	return 0;
}