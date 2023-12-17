#include <string>
#include <vector>


using namespace std;
vector<string> splitRectangularString(const string& str, const size_t& lineLength);
vector<string> splitString(const string& str, char splitter);
int isNumber(string str);
int stringToInt(string str);
string readInput(int day, char* argv[]);

typedef int (*SolutionFunction)(const string& str);
unsigned long long timeFunction(SolutionFunction func, string input, const int iterations);
void timeFunctionAndPrint(SolutionFunction func, string input, const int iterations);

inline size_t findChar(const char* str, char c) {
	size_t i = 0;
	while (str[i] != c) 
		i++;
	return i;
}