#include <string>
#include <vector>


using namespace std;
vector<string> splitString(const string& str, char splitter);
int isNumber(string str);
int stringToInt(string str);
string readInput(int day, char* argv[]);

typedef int (*SolutionFunction)(const string& str);
unsigned long long timeFunction(SolutionFunction func, string input, const int iterations);
void timeFunctionAndPrint(SolutionFunction func, string input, const int iterations);

inline int findChar(const char* str, char c) {
	int i = 0;
	while (str[i] != c) 
		i++;
	return i;
}