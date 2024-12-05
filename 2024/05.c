#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Util.h"


// We are assuming all the numbers in this puzzle are two digit

static inline int* parse_rules(char* str, int* indexPtr) {
	int* arr = malloc(100 * 100 * sizeof(int));
	memset(arr, 0, 100 * 100 * sizeof(int));

	// we are using this 1d array like a map. it works like this:
	// if there is a rule like x|y, it means y must appear after x.
	// in the map, this will show as the index [x * 100 + y] being set to 1.
	// when the program encounters an update like y, ... x, 
	// it checks [x * 100 + n], with n being any of the previous numbers in the update.
	// once it gets to [x * 100 + y] and finds that it's set to 1, it knows that x must appear before y,
	// and so knows the update is invalid.

	int i = 0;
	while (str[i] != '\n') {
		int left = (str[i] - '0') * 10 + (str[i + 1] - '0');
		int right = (str[i + 3] - '0') * 10 + (str[i + 4] - '0');

		arr[left * 100 + right] = 1;
		i += 6;
	}

	// move to the update sections
	i++;

	*indexPtr = i;
	return arr;
}


static inline int get_update_middle_if_valid(char* str, int* arr, int* page_arr, int page_amount, int i) {
	int middle_index = (page_amount / 2);
	int middle_number = 0;
	for (int j = 0; j < page_amount; j++) {
		int number = (str[i] - '0') * 10 + (str[i + 1] - '0');
		if (j == middle_index)
			middle_number = number;
		// check if this number wants to show up before any of the previous numbers. if it does, this line is invalid.
		for (int k = 0; k < j; k++) {
			if (arr[number * 100 + page_arr[k]])
				return 0;
		}
		page_arr[j] = number;
		i += 3;
	}
	return middle_number;
}

int solution_1(const string input) {
	char* str = input.content;
	int i;
	int* arr = parse_rules(str, &i);

	int sum = 0;
	
	int max_page_amount = 1;
	int* page_arr = malloc(max_page_amount * sizeof(int));

	while (i < input.size) {
		int start = i;

		int page_amount = 1;
		i += 2; // skip to comma
		while (str[i] != '\n') {
			page_amount++;
			i += 3;
		}
		i++; // now on next line
		if (page_amount > max_page_amount) {
			page_arr = realloc(page_arr, page_amount * sizeof(int));
			max_page_amount = page_amount;
		}

		sum += get_update_middle_if_valid(str, arr, page_arr, page_amount, start);
	}

	free(page_arr);
	free(arr);

	return sum;
}

static inline int correct_update_if_invalid_and_get_middle(char* str, int* arr, int* page_arr, int page_amount, int i) {
	int middle_index = (page_amount / 2);
	for (int j = 0; j < page_amount; j++) {
		int number = (str[i] - '0') * 10 + (str[i + 1] - '0');
		page_arr[j] = number;
		i += 3;
	}
	int corrections = 0;
	for (int j = 0; j < page_amount; j++) {
		int number = page_arr[j];
		for (int k = 0; k <= j; k++) {
			if (arr[number * 100 + page_arr[k]]) {
				// this number wants to show up before the number at k
				int keep = page_arr[k];
				page_arr[j] = keep;
				page_arr[k] = number;

				// now focus on this number, it may need to be moved too
				number = keep;
				corrections++;
			}
		}
	}
	if (corrections == 0)
		return 0;
	return page_arr[middle_index];
}


int solution_2(const string input) {
	char* str = input.content;
	int i;
	int* arr = parse_rules(str, &i);

	int sum = 0;
	
	int max_page_amount = 1;
	int* page_arr = malloc(max_page_amount * sizeof(int));


	while (i < input.size) {
		int start = i;

		int page_amount = 1;
		i += 2; // skip to comma
		while (str[i] != '\n') {
			page_amount++;
			i += 3;
		}
		i++; // now on next line
		if (page_amount > max_page_amount) {
			page_arr = realloc(page_arr, page_amount * sizeof(int));
			max_page_amount = page_amount;
		}

		sum += correct_update_if_invalid_and_get_middle(str, arr, page_arr, page_amount, start);
	}

	free(page_arr);
	free(arr);

	return sum;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(5, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 10000);

	free(input.content);
	return 0;
}


