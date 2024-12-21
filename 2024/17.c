#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Util.h"

// Everyone always wants to tell the computer what to do. But no one ever asks it how it is. Curious!

static inline uint64_t parse_number_until_char(char* str, int* indexPtr, char stop) {
	int pos = *indexPtr;
	uint64_t num = 0;
	do {
		num = num * 10 + (str[pos] - '0');
		pos++;
	} while (str[pos] != stop);
	*indexPtr = pos;
	return num;
}
// needs to be less than 64bit for bullshit reason otherwise timing the function doesn't work
typedef struct {
	uint32_t data;
	uint8_t size;
} program_output;

static inline void print_output(program_output output) {
	printf("%u", (output.data >> (3 * (output.size - 1))) & 7);
	for (int i = output.size - 2; i >= 0; i--) {
		printf(", %u", (output.data >> (3 * i)) & 7);
	}
}

program_output solution_1(const string input) {
	char* str = input.content;
	// "Register X: " - 12 chars
	int pos = 12;
	uint64_t A = parse_number_until_char(str, &pos, '\n');
	pos += 1 + 12;
	uint64_t B = parse_number_until_char(str, &pos, '\n');
	pos += 1 + 12;
	uint64_t C = parse_number_until_char(str, &pos, '\n');
	pos += 1 + 1; // 2 newlines
	// "Program: " - 10 chars
	pos += 9;

	uint64_t combo[] = {0, 1, 2, 3, A, B, C, 0};

	int characters_left = input.size - pos;
	int amount_of_numbers = characters_left / 2;

	uint8_t* program = malloc(amount_of_numbers * sizeof(uint8_t));
	int ip = 0;
	while (pos < input.size) {
		program[ip] = str[pos] - '0';
		pos += 2;
		ip++;
		program[ip] = str[pos] - '0';
		pos += 2;
		ip++;
	}

	uint32_t output;
	int output_size = 0;
	
	ip = 0;
	while (ip < amount_of_numbers - 1) {
		uint8_t instruction = program[ip];
		uint8_t operand = program[ip + 1];
		switch(instruction) {
			case 0: // adv
				A = A / (1 << combo[operand]);
				combo[4] = A;
				ip += 2;
				break;
			case 1: // bxl
				B = B ^ operand;
				combo[5] = B;
				ip += 2;
				break;
			case 2: // bst
				B = combo[operand] & 7;
				combo[5] = B;
				ip += 2;
				break;
			case 3: // jnz
				if (A == 0)
					ip += 2;
				else {
					ip = operand;
				}
				break;
			case 4: // bxc
				B = B ^ C;
				combo[5] = B;
				ip += 2;
				break;
			case 5: // out
				output <<= 3;
				output |= combo[operand] & 7;
				output_size++;
				ip += 2;
				break;
			case 6: // bdv
				B = A / (1 << combo[operand]);
				combo[5] = B;
				ip += 2;
				break;
			case 7: // cdv
				C = A / (1 << combo[operand]);
				combo[6] = C;
				ip += 2;
				break;
		}
	}
	
	free(program);
	program_output out = {output, output_size};
	return out;
}

// i almost found this out by myself, but i caved in and looked up solutions because i'm falling behind.

// implemented the solution based off of the idea from here:
// https://github.com/maneatingape/advent-of-code-rust/blob/main/src/year2024/day17.rs

uint64_t func(uint64_t try_A, int program_index, uint8_t* program, int program_size) {
	for (int i = 0; i < 8; i++) {
		uint64_t A = try_A + i;
		uint64_t B = 0;
		uint64_t C = 0;
		uint64_t combo[] = {0, 1, 2, 3, A, B, C, 0};
		int ip = 0;
		int current_output_index = 0;
		while (ip < program_size - 1) {
			uint8_t instruction = program[ip];
			uint8_t operand = program[ip + 1];
			switch(instruction) {
				case 0: // adv
					A = A / (1 << combo[operand]);
					combo[4] = A;
					ip += 2;
					break;
				case 1: // bxl
					B = B ^ operand;
					combo[5] = B;
					ip += 2;
					break;
				case 2: // bst
					B = combo[operand] & 7;
					combo[5] = B;
					ip += 2;
					break;
				case 3: // jnz
					if (A == 0)
						ip += 2;
					else {
						ip = operand;
					}
					break;
				case 4: // bxc
					B = B ^ C;
					combo[5] = B;
					ip += 2;
					break;
				case 5: // out
					uint8_t program_byte = combo[operand] & 7;
					if (program_byte != program[program_size - program_index - 1]) {
						ip = program_size;
						break;
					}

					if (program_index + 1 == program_size)
						return (try_A + i);
					uint64_t possible_answer = func((try_A + i) << 3, program_index + 1, program, program_size);
					if (possible_answer != 0)
						return possible_answer;
						
					current_output_index++;
					ip += 2;
					break;
				case 6: // bdv
					B = A / (1 << combo[operand]);
					combo[5] = B;
					ip += 2;
					break;
				case 7: // cdv
					C = A / (1 << combo[operand]);
					combo[6] = C;
					ip += 2;
					break;
			}
		}
	}
	return 0;
}



uint64_t solution_2(const string input) {
	char* str = input.content;
	// "Register X: " - 12 chars
	int pos = 12;
	parse_number_until_char(str, &pos, '\n');
	pos += 1 + 12;
	parse_number_until_char(str, &pos, '\n');
	pos += 1 + 12;
	parse_number_until_char(str, &pos, '\n');
	pos += 1 + 1; // 2 newlines
	// "Program: " - 10 chars
	pos += 9;

	int characters_left = input.size - pos;
	int amount_of_numbers = characters_left / 2;

	uint8_t* program = malloc(amount_of_numbers * sizeof(uint8_t));
	int ip = 0;
	while (pos < input.size) {
		program[ip] = str[pos] - '0';
		pos += 2;
		ip++;
		program[ip] = str[pos] - '0';
		pos += 2;
		ip++;
	}

	uint64_t answer = func(0, 0, program, amount_of_numbers);
	
	free(program);
	return answer;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(17, argv);
	
	printf("Part 1: ");
	program_output output = solution_1(input);
	print_output(output);
	printf("\n");
	time_function_and_print((SolutionFunction)solution_1, input, 10000);

	printf("Part 2: %llu\n", solution_2(input));
	time_function_and_print((SolutionFunction)solution_2, input, 10000);

	free(input.content);
	return 0;
}


