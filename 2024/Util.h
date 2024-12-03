typedef struct {
	char* content;
	const long size;
} string;

typedef int (*SolutionFunction)(string input);
double time_function(SolutionFunction func, string input, const int iterations);
void time_function_and_print(SolutionFunction func, string input, const int iterations); 
void init_timing();
const string read_input(int day, char* argv[]);