#include "Util.h"
#include "epoch.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <time.h>


void init_timing() {
	Epoch__Init();
}

double time_function(SolutionFunction func, string input, const int iterations) {
	Epoch_t timer;
	Epoch__Start(&timer);
	for (int i = 0; i < iterations; i++) {
		func(input);
	}
	return Epoch__QueryChange(&timer) / iterations;
}

void time_function_and_print(SolutionFunction func, string input, const int iterations) {
	printf("Timing function (running %d iterations...)\n", iterations); 
	double executionTime = time_function(func, input, iterations);
	printf("Average time out of %d iterations: %lld microseconds\n", iterations, (unsigned long long)(executionTime * 1000000.0));

	if (executionTime != 0) {
		#define blinkTime 0.1
		double runs = blinkTime / executionTime;
		printf("Blinking takes roughly 100 milliseconds. The function would run %d times in one blink.\n", (int)runs);
	}
}

const string read_input_base(int day, char* argv[]) {
	char filename[7];
	if (day < 10)
		sprintf(filename, "0%d.txt", day);
	else
		sprintf(filename, "%d.txt", day);
	filename[6] = '\0';

	int path_length = strlen(argv[1]);
	int full_filename_size = path_length + 1 + 6 + 1; // path plus path separator, two digits and .txt, null terminator
	char *full_filename = (char*)malloc(full_filename_size); 

	strcpy(full_filename, argv[1]);
	strcpy(full_filename + path_length, "\\");
	strcpy(full_filename + path_length + 1, filename);
	full_filename[full_filename_size] = '\0';
	FILE *file = fopen(full_filename, "r");
	fseek(file, 0, SEEK_END);
	long file_byte_size = ftell(file);
	rewind(file);

	char* input = malloc(file_byte_size + 1);
	size_t file_size = fread(input, 1, file_byte_size, file);
	fclose(file);

	
	// remove all trailing newlines
	for (size_t i = file_size - 1; i > 0; i--) {
		if (input[i] != '\n')
			break;
		file_size--;
	}
	input[file_size] = '\0';

	const string str = { input, file_size };

	return str;
}


const string read_input(int day, char* argv[]) {
	string str = read_input_base(day, argv);
	
	// add trailing newline
	str.size++;	
	str.content = realloc(str.content, str.size + 1);
	str.content[str.size - 1] = '\n';
	str.content[str.size] = '\0';
	
	return str;
}

const string read_input_no_trailing_newline(int day, char* argv[]) {
	return read_input_base(day, argv);
}