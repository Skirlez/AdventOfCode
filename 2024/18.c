#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include "Util.h"

// copied from day 16
typedef struct {
	int x;
	int y;
	int g_cost;
	int h_cost;
	int dir_index;
} node;

// priority queue implementation using binary heap
typedef struct {
	node** arr;
	int allocated_size;
	int last_queue_index;
} priority_queue;

priority_queue* create_queue() {
	priority_queue* queue = malloc(sizeof(priority_queue));
	queue->allocated_size = 2;
	queue->last_queue_index = -1;
	queue->arr = malloc(queue->allocated_size * sizeof(node*));
	return queue;
}
void free_queue(priority_queue* queue) {
	free(queue->arr);
	free(queue);
}
void clear_queue(priority_queue* queue) {
	queue->last_queue_index = -1;
}

bool is_queue_empty(priority_queue* queue) {
	return queue->last_queue_index == -1;
}

void add_to_queue(priority_queue* queue, node* new_node) {
	queue->last_queue_index++;
	if (queue->last_queue_index == queue->allocated_size) {
		queue->allocated_size *= 2;
		queue->arr = realloc(queue->arr, queue->allocated_size * sizeof(node*));
	}
	node** arr = queue->arr;

	int f_cost = new_node->g_cost + new_node->h_cost;
	int current_index = queue->last_queue_index;

	arr[current_index] = new_node;
	int parent_index = (current_index - 1) / 2;
	node* parent = arr[parent_index];

	while (parent->g_cost + parent->h_cost > f_cost) {
		arr[current_index] = parent;
		arr[parent_index] = new_node;

		current_index = parent_index;
		parent_index = (current_index - 1) / 2;
		parent = arr[parent_index];
	}
}

void queue_reevaluate_or_add(priority_queue* queue, node* updated_node) {
	node** arr = queue->arr;

	int f_cost = updated_node->g_cost + updated_node->h_cost;

	int last_index = queue->last_queue_index;
	int current_index = 0;
	while (arr[current_index] != updated_node && current_index <= last_index)
		current_index++;

	if (current_index == last_index + 1) {
		add_to_queue(queue, updated_node);
		return;
	}
	int parent_index = (current_index - 1) / 2;
	node* parent = arr[parent_index];
	while (parent->g_cost + parent->h_cost > f_cost) {
		arr[current_index] = parent;
		arr[parent_index] = updated_node;

		current_index = parent_index;
		parent_index = (current_index - 1) / 2;
		parent = arr[parent_index];
	}
}

node* pop_queue(priority_queue* queue) {
	if (queue->last_queue_index == -1)
		return NULL;
	node** arr = queue->arr;
	node* root = arr[0];
	node* last = arr[queue->last_queue_index];
	arr[0] = last;
	int f_cost = last->h_cost + last->g_cost;
	int current_index = 0;

	queue->last_queue_index--;
	do {
		int left_index = 2 * current_index + 1;
		int left_diff = 0;
		node* left = NULL;
		if (left_index <= queue->last_queue_index) {
			left = arr[left_index];
			left_diff = f_cost - (left->h_cost + left->g_cost);
		}
		int right_index = 2 * current_index + 2;
		int right_diff = 0;
		node* right = NULL;
		if (right_index <= queue->last_queue_index) {
			right = arr[right_index];

			right_diff = f_cost - (right->h_cost + right->g_cost);

		}
		if (left_diff <= 0 && right_diff <= 0)
			return root;
		// shameful
		if (left_diff > 0 && right_diff > 0) {
			if (left_diff > right_diff)
				goto left;
			goto right;
		}
		if (left_diff > 0) {
			goto left;
		}
		if (right_diff > 0) {
			goto right;
		}
		left:
			arr[current_index] = left;
			arr[left_index] = last;
			current_index = left_index;
			continue;
		right:
			arr[current_index] = right;
			arr[right_index] = last;
			current_index = right_index;
			continue;
	} while(1);
}




int map_size = 71;
int amount_to_insert = 1024;

uint8_t** parse_into_map(char* str, int size, int* index_ptr, int amount) {
	int pos = *index_ptr;
	uint8_t** map = malloc(map_size * sizeof(uint8_t*));
	for (int i = 0; i < map_size; i++) {
		map[i] = calloc(map_size, sizeof(uint8_t));
	}
	int amount_parsed = 0;
	while (pos < size && amount_parsed < amount) {
		int x = 0;
		do {
			x = x * 10 + (str[pos] - '0');
			pos++;
		} while (str[pos] != ',');
		pos++;
		int y = 0;
		do {
			y = y * 10 + (str[pos] - '0');
			pos++;
		} while (str[pos] != '\n');
		pos++;

		map[y][x] = 1;
		amount_parsed++;
	}
	/*
	for (int i = 0; i < map_size; i++) {
		for (int j = 0; j < map_size; j++) {
			if (map[i][j] == 0)
				printf(".");
			else
				printf("#");
		}
		printf("\n");
	}
	printf("\n");
	*/
	*index_ptr = pos;
	return map;
}

int x_directions[] = {1, 0, -1, 0};
int y_directions[] = {0, -1, 0, 1};
int opposite_directions[] = {2, 3, 0, 1};

int solution_1(const string input) {
	char* str = input.content;

	int zero = 0;
	uint8_t** map = parse_into_map(str, input.size, &zero, amount_to_insert);
	node*** node_map = malloc(map_size * sizeof(node**));
	for (int i = 0; i < map_size; i++) {
		node_map[i] = calloc(map_size, sizeof(node*));
	}

	int all_nodes_allocated_size = 2;
	int all_nodes_last_index = -1;
	node** all_nodes_list = malloc(all_nodes_allocated_size * sizeof(node*));

	int start_x = 0;
	int start_y = 0;

	int end_x = map_size - 1;
	int end_y = map_size - 1;

	priority_queue* queue = create_queue();

	node start_node;
	start_node.x = start_x;
	start_node.y = start_y;
	start_node.dir_index = 0;
	start_node.g_cost = 0;
	start_node.h_cost = abs(end_x - start_x) + abs(end_y - start_y);
	node_map[start_y][start_x] = &start_node;

	add_to_queue(queue, node_map[start_y][start_x]);
	node* current_node;
	do {
		current_node = pop_queue(queue);
		for (int dir_index = 0; dir_index < 4; dir_index++) {
			int new_x = current_node->x + x_directions[dir_index];
			int new_y = current_node->y + y_directions[dir_index];
			if (current_node->dir_index == opposite_directions[dir_index]
					|| (new_y < 0 || new_x < 0 || new_x >= map_size || new_y >= map_size)) {
				continue;
			}
			int new_g_cost = current_node->g_cost + 1;
			if (map[new_y][new_x] != 1) {
				if (node_map[new_y][new_x] == NULL
						|| node_map[new_y][new_x]->g_cost > new_g_cost) {
					node* new_node; 
					if (node_map[new_y][new_x] == NULL) {
						new_node = malloc(sizeof(node));
						all_nodes_last_index++;
						if (all_nodes_allocated_size == all_nodes_last_index) {
							all_nodes_allocated_size *= 2;
							all_nodes_list = realloc(all_nodes_list, all_nodes_allocated_size * sizeof(node*));
						}
						all_nodes_list[all_nodes_last_index] = new_node;
						node_map[new_y][new_x] = new_node;
						new_node->g_cost = new_g_cost;
						new_node->h_cost = abs(end_x - start_x) + abs(end_y - start_y);
						add_to_queue(queue, new_node);
					}
					else {
						new_node = node_map[new_y][new_x];
						new_node->g_cost = new_g_cost;
						queue_reevaluate_or_add(queue, new_node);
					}

					new_node->dir_index = dir_index;
					new_node->x = new_x;
					new_node->y = new_y;
				}
			}
		}
	} while (!(current_node->x == end_x && current_node->y == end_y) && !is_queue_empty(queue));

	int lowest_cost = current_node->g_cost;

	for (int i = 0; i <= all_nodes_last_index; i++) {
		free(all_nodes_list[i]);
	}
	for (int i = 0; i < map_size; i++) {
		free(map[i]);
		free(node_map[i]);
	}
	free(map);
	free(node_map);
	return lowest_cost;
}

typedef struct {
	uint32_t x;
	uint32_t y;
} part2_answer;


part2_answer parse_one_more_into_map(char* str, int size, uint8_t** map, int* index_ptr) {
	int pos = *index_ptr;
	int x = 0;
	do {
		x = x * 10 + (str[pos] - '0');
		pos++;
	} while (str[pos] != ',');
	pos++;
	int y = 0;
	do {
		y = y * 10 + (str[pos] - '0');
		pos++;
	} while (str[pos] != '\n');
	pos++;

	map[y][x] = 1;
	*index_ptr = pos;
	part2_answer possible_answer = {x, y};
	return possible_answer;
} 


part2_answer solution_2(const string input) {
	char* str = input.content;

	int pos = 0;
	uint8_t** map = parse_into_map(str, input.size, &pos, amount_to_insert);
	node*** node_map = malloc(map_size * sizeof(node**));
	for (int i = 0; i < map_size; i++) {
		node_map[i] = calloc(map_size, sizeof(node*));
	}

	int all_nodes_allocated_size = 2;
	int all_nodes_last_index = -1;
	node** all_nodes_list = malloc(all_nodes_allocated_size * sizeof(node*));

	int start_x = 0;
	int start_y = 0;

	int end_x = map_size - 1;
	int end_y = map_size - 1;

	priority_queue* queue = create_queue();

	node start_node;
	start_node.x = start_x;
	start_node.y = start_y;
	start_node.dir_index = 0;
	start_node.g_cost = 0;
	start_node.h_cost = abs(end_x - start_x) + abs(end_y - start_y);
	node_map[start_y][start_x] = &start_node;

	part2_answer answer;

	add_to_queue(queue, node_map[start_y][start_x]);
	node* current_node;
	do {
		do {
			current_node = pop_queue(queue);
			for (int dir_index = 0; dir_index < 4; dir_index++) {
				int new_x = current_node->x + x_directions[dir_index];
				int new_y = current_node->y + y_directions[dir_index];
				if (current_node->dir_index == opposite_directions[dir_index]
						|| (new_y < 0 || new_x < 0 || new_x >= map_size || new_y >= map_size)) {
					continue;
				}
				int new_g_cost = current_node->g_cost + 1;
				if (map[new_y][new_x] != 1) {
					if (node_map[new_y][new_x] == NULL
							|| node_map[new_y][new_x]->g_cost > new_g_cost) {
						node* new_node; 
						if (node_map[new_y][new_x] == NULL) {
							new_node = malloc(sizeof(node));
							all_nodes_last_index++;
							if (all_nodes_allocated_size == all_nodes_last_index) {
								all_nodes_allocated_size *= 2;
								all_nodes_list = realloc(all_nodes_list, all_nodes_allocated_size * sizeof(node*));
							}
							all_nodes_list[all_nodes_last_index] = new_node;
							node_map[new_y][new_x] = new_node;
							new_node->g_cost = new_g_cost;
							new_node->h_cost = abs(end_x - start_x) + abs(end_y - start_y);
							add_to_queue(queue, new_node);
						}
						else {
							new_node = node_map[new_y][new_x];
							new_node->g_cost = new_g_cost;
							queue_reevaluate_or_add(queue, new_node);
						}

						new_node->dir_index = dir_index;
						new_node->x = new_x;
						new_node->y = new_y;
					}
				}
			}
		} while (!(current_node->x == end_x && current_node->y == end_y) && !is_queue_empty(queue));

		bool reached_end = current_node->x == end_x && current_node->y == end_y;
		if (reached_end) {
			do {
				answer = parse_one_more_into_map(str, input.size, map, &pos);
			} while (node_map[answer.y][answer.x] == NULL);
			
			int max_g_cost = node_map[answer.y][answer.x]->g_cost;

			int new_all_nodes_last_index = -1;
			node** new_all_nodes_list = malloc(all_nodes_allocated_size * sizeof(node*));

			clear_queue(queue);

			for (int i = 0; i <= all_nodes_last_index; i++) {
				node* n = all_nodes_list[i];
				if (n->g_cost >= max_g_cost) {
					node_map[n->y][n->x] = NULL;
					free(n);
				}
				else {
					new_all_nodes_last_index++;
					new_all_nodes_list[new_all_nodes_last_index] = n;
					add_to_queue(queue, n);
				}
			}
			free(all_nodes_list);

			all_nodes_list = new_all_nodes_list;
			all_nodes_last_index = new_all_nodes_last_index;

			node_map[start_y][start_x] = &start_node;
			add_to_queue(queue, node_map[start_y][start_x]);
		}
		else
			break;
	} while (1);

	
	
	for (int i = 0; i <= all_nodes_last_index; i++) {
		free(all_nodes_list[i]);
	}
	for (int i = 0; i < map_size; i++) {
		free(map[i]);
		free(node_map[i]);
	}
	free(map);
	free(node_map);
	return answer;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(18, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 10000);

	part2_answer ans = solution_2(input);

	printf("Part 2: %d,%d\n", ans.x, ans.y);
	time_function_and_print((SolutionFunction)solution_2, input, 2);

	free(input.content);
	return 0;
}


