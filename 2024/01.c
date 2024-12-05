#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

// Possible ways to improve include a better sorting algorithm, or see the comment on part 2.


int compare(const void *a, const void *b) {
	return (*(int*)a - *(int*)b);
}

static inline void parse_to_lists(const string input, size_t lineLength, int* list_1, int* list_2) {
	int line = 0;
	const char* str = input.content;
	for (int i = 0; i < input.size; i += lineLength) {
		int j = i + lineLength - 2;
		int multiplier = 1;
		int number_2 = 0;
		while (str[j] != ' ') {
			number_2 += (str[j] - '0') * multiplier;
			multiplier *= 10;
			j--;
		}

		while (str[j] == ' ')
			j--;
		multiplier = 1;
		int number_1 = 0;
		
		while (j >= i) {
			number_1 += (str[j] - '0') * multiplier;
			multiplier *= 10;
			j--;
		}
		list_1[line] = number_1;
		list_2[line] = number_2;
		line++;
	}
}


int solution_1(const string input) {
	const char* str = input.content;
	size_t lineLength = first_occurrence_of_char(str, '\n') + 1;
	const size_t lines = input.size / lineLength;

	int* list_1 = malloc(lines * sizeof(int));
	int* list_2 = malloc(lines * sizeof(int));

	parse_to_lists(input, lineLength, list_1, list_2);

	qsort(list_1, lines, sizeof(int), compare);
	qsort(list_2, lines, sizeof(int), compare);

	int sum = 0;
	for (size_t i = 0; i < lines; i++) {
		sum += abs(list_1[i] - list_2[i]);
	}

	free(list_1);
	free(list_2);
	return sum;
}


// If the lists are sorted, all the occurrences of a number are going to be
// right next to each other. So that's worth doing, since we can also implement binary search.

// There's maybe a possibility of using the fact that the lists are probably very similar when sorted 
// (a number in index i in list 1 is going to find the numbers equal to it in list 2 somewhere close to index i)
// But for now it seems the overhead of sorting is too big.

int solution_2(const string input) {
	const char* str = input.content;
	size_t lineLength = first_occurrence_of_char(str, '\n') + 1;
	const size_t lines = input.size / lineLength;

	int* list_1 = malloc(lines * sizeof(int));
	int* list_2 = malloc(lines * sizeof(int));

	parse_to_lists(input, lineLength, list_1, list_2);

	//qsort(list_1, lines, sizeof(int), compare);
	qsort(list_2, lines, sizeof(int), compare);
	
	int sum = 0;
	for (size_t i = 0; i < lines; i++) {
		int number = list_1[i];
		int count = 0;

		int low = 0;
		int high = lines - 1;

		while (low <= high) {
			int middle = (low + high) / 2;
			if (list_2[middle] < number) {
				low = middle + 1;
			}
			else if (list_2[middle] > number) {
				high = middle - 1;
			}
			else
				break;
		}
		int middle = (low + high) / 2;
		if (list_2[middle] == number) {
			middle--;
			while (middle >= 0 && list_2[middle] == number) 
				middle--;
			middle++;
			while (list_2[middle] == number) {
				middle++;
				count++;
			}
		}

		sum += number * count;
	}

	free(list_1);
	free(list_2);

	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(1, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


