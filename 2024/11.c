#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "Util.h"

typedef uint64_t number;


number shared_solution(int blinks, const string input) {
	number** digits_amounts = malloc(10 * sizeof(number*));
	int allocated_size = 2;

	number* working_array = malloc(allocated_size * sizeof(number));
	number* counts = malloc(allocated_size * sizeof(number));
		
	// Tried caching digits 0-9. This does work but just removing duplicates and not doing this is actually faster for Part 2
	// Does result in slight improvement for Part 1
	for (int k = 1; k < 10; k++) {
		number* results = malloc(blinks * sizeof(number));
		digits_amounts[k] = results;
		int last_index = 0;
		working_array[last_index] = k;
		// how many more additional copies of this number are there
		counts[last_index] = 0;
	
		for (int j = 0; j < blinks; j++) {
			for (int i = 0; i <= last_index; i++) {
				if (working_array[i] == 0) {
					working_array[i] = 1;
				}
				else {
					bool even_digits = false;
					int num = working_array[i];
					number half_multiplier = 1;

					while (num >= 10) {
						num /= 10;
						if (!even_digits)
							half_multiplier *= 10;
						even_digits = !even_digits;
					}
					if (even_digits) {
						number left = working_array[i] / half_multiplier;
						number right = working_array[i] % half_multiplier;
						working_array[i] = left;

						last_index++;
						if (last_index == allocated_size) {
							allocated_size *= 2;
							working_array = realloc(working_array, allocated_size * sizeof(number));
							counts = realloc(counts, allocated_size * sizeof(number));
						}
						memmove(&working_array[i + 1], &working_array[i], (last_index - i) * sizeof(number));
						memmove(&counts[i + 1], &counts[i], (last_index - i) * sizeof(number));
						working_array[i + 1] = right;
						counts[i + 1] = counts[i];
						i++;
		
						
					}
					else {
						working_array[i] *= 2024;
					}
				}

			}
			for (int i = 0; i < last_index; i++) {
				number num = working_array[i];
				for (int k = i + 1; k <= last_index; k++) {
					if (num == working_array[k]) {
						counts[i] += counts[k] + 1;

						memmove(&working_array[k], &working_array[k + 1], (last_index - k) * sizeof(number));
						memmove(&counts[k], &counts[k + 1], (last_index - k) * sizeof(number));
						last_index--;
						k--;
					}
				}
			}


			number additional_sum = 0;
			for (int i = 0; i <= last_index; i++) {
				additional_sum += counts[i];
			}
			//printf("%lld\n", additional_sum + last_index + 1);
			results[j] = additional_sum + last_index + 1;
		}
	}
	
	// 0 is just a shift by 1 of 1
	{
		number* results = malloc(blinks * sizeof(number));
		digits_amounts[0] = results;
		memcpy(&results[1], digits_amounts[1], (blinks - 1) * sizeof(number));
		results[0] = 1;
	}
	char* str = input.content;
	
	
	number sum = 0;
	number string_num = 0;
	int pos = 0;
	while (pos < input.size) {
		string_num = string_num * 10 + (str[pos] - '0');
		pos++;
		if (str[pos] == ' ' || str[pos] == '\n') {
			int last_index = 0;
			working_array[last_index] = string_num;
			counts[last_index] = 0;
			string_num = 0;
			pos++;

			int j = 0;
			while (last_index != -1 && j < blinks) {
				for (int i = 0; i <= last_index; i++) {
					number num = working_array[i];
					if (num < 10) {
						sum += digits_amounts[num][blinks - j - 1] * (counts[i] + 1);
						memmove(&working_array[i], &working_array[i + 1], (last_index - i) * sizeof(number));
						memmove(&counts[i], &counts[i + 1], (last_index - i) * sizeof(number));
						last_index--;
						i--;
						continue;
					}
	
					bool even_digits = false;
					number half_multiplier = 1;
					while (num >= 10) {
						num /= 10;
						if (!even_digits)
							half_multiplier *= 10;
						even_digits = !even_digits;
					}
					if (even_digits) {
						number left = working_array[i] / half_multiplier;
						number right = working_array[i] % half_multiplier;

						working_array[i] = left;
						last_index++;
						if (last_index == allocated_size) {
							allocated_size *= 2;
							working_array = realloc(working_array, allocated_size * sizeof(number));
							counts = realloc(counts, allocated_size * sizeof(number));
						}
						memmove(&working_array[i + 1], &working_array[i], (last_index - i) * sizeof(number));
						memmove(&counts[i + 1], &counts[i], (last_index - i) * sizeof(number));
						working_array[i + 1] = right;
						i++;
					}
					else {
						working_array[i] *= 2024;
					}
				}
				for (int i = 0; i < last_index; i++) {
					number num = working_array[i];

					for (int k = i + 1; k <= last_index; k++) {
						if (num == working_array[k]) {
							counts[i] += counts[k] + 1;

							memmove(&working_array[k], &working_array[k + 1], (last_index - k) * sizeof(number));
							memmove(&counts[k], &counts[k + 1], (last_index - k) * sizeof(number));
							last_index--;
							k--;
						}
					}
				}

				j++;
			}
			number additional_sum = 0;
			for (int i = 0; i <= last_index; i++) {
				additional_sum += counts[i];
			}
			sum += additional_sum + last_index + 1;
		}

	}

	for (int k = 0; k < 10; k++)
		free(digits_amounts[k]);
	free(digits_amounts);
	free(working_array);
	free(counts);
	return sum;
}

number solution_1(const string input) {
	return shared_solution(25, input);
}

number solution_2(const string input) {
	return shared_solution(75, input);
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(11, argv);
	
	printf("Part 1: %llu\n", solution_1(input));
	time_function_and_print((SolutionFunction)solution_1, input, 10);

	printf("Part 2: %llu\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 10);

	free(input.content);
	return 0;
}


