#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "Util.h"

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


int x_directions[] = {1, 0, -1, 0};
int y_directions[] = {0, -1, 0, 1};
int opposite_directions[] = {2, 3, 0, 1};


int solution_1(const string input) {
	char* str = input.content;
	int all_nodes_allocated_size = 2;
	int all_nodes_last_index = -1;
	node** all_nodes_list = malloc(all_nodes_allocated_size * sizeof(node*));

	priority_queue* queue = create_queue();

	int row_size = first_occurrence_of_char(str, '\n');
	int column_size = input.size / (row_size + 1); // + 1 to consider the newline characters
	
	node*** node_map = malloc(column_size * sizeof(node**));

	char** map = malloc(column_size * sizeof(char*));
	for (int i = 0; i < column_size; i++) {
		map[i] = malloc(row_size);
		memcpy(map[i], &str[i * (row_size + 1)], row_size);
		node_map[i] = calloc(row_size, sizeof(node*));
	}

	int start_pos = first_occurrence_of_char(str, 'S');
	int start_x = start_pos % (row_size + 1);
	int start_y = start_pos / (row_size + 1);

	int end_pos = first_occurrence_of_char(str, 'E');
	int end_x = end_pos % (row_size + 1);
	int end_y = end_pos / (row_size + 1);

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
			if (current_node->dir_index == opposite_directions[dir_index])
				continue;
			int new_x = current_node->x + x_directions[dir_index];
			int new_y = current_node->y + y_directions[dir_index];
			
			int new_g_cost = current_node->g_cost + 1 + (dir_index != current_node->dir_index) * 1000;
			if (map[new_y][new_x] != '#') {
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
	for (int i = 0; i < column_size; i++) {
		free(map[i]);
		free(node_map[i]);
	}
	free(map);
	free(node_map);
	return lowest_cost;
}

int count_best_paths_backwards(node* current_node, node* start, node*** node_map,  char** map) {
	if (current_node == start) {
		map[current_node->y][current_node->x] = 'O';
		return 1;
	}
	map[current_node->y][current_node->x] = 'O';
	int sum = 1;
	for (int dir_index = 0; dir_index < 4; dir_index++) {
		int new_x = current_node->x + x_directions[dir_index];
		int new_y = current_node->y + y_directions[dir_index];
		node* new_node = node_map[new_y][new_x];
		if (new_node == NULL)
			continue;
		if (map[new_y][new_x] == 'O')
			continue;

		int g_cost = current_node->g_cost;
		if (new_node->g_cost + 1 == g_cost) {
			sum += count_best_paths_backwards(new_node, start, node_map, map);
			continue;
		}
		if (new_node->g_cost + 1 + (new_node->dir_index % 2 != dir_index % 2) * 1000 == g_cost) {
			sum += count_best_paths_backwards(new_node, start, node_map, map);

			// Consider the following situation. current_node is where the arrow is.
			// OOO <-
			//  O
			

			// Two best paths join together in the center O. We are coming from the left.
			// The center O has a score -1001 of our own. The O to the left has a score -2 of our own.
			// Once we get to the center O, it will ignore the O to the left, since there isn't a turn between them,
			// and they have mismatching scores. So we're checking if such an O exists right here.

			int new_x_2 = new_x + x_directions[dir_index];
			int new_y_2 = new_y + y_directions[dir_index];
			node* new_node_2 = node_map[new_y_2][new_x_2];
			if (new_node_2 == NULL)
				continue;
			if (map[new_y_2][new_x_2] == 'O')
				continue;
			if (new_node_2->g_cost + 2 != g_cost) {
				continue;
			}
			sum += count_best_paths_backwards(new_node_2, start, node_map, map);
		}



	}
	return sum;
}

int solution_2(const string input) {
	char* str = input.content;
	int all_nodes_allocated_size = 2;
	int all_nodes_last_index = -1;
	node** all_nodes_list = malloc(all_nodes_allocated_size * sizeof(node*));

	priority_queue* queue = create_queue();

	int row_size = first_occurrence_of_char(str, '\n');
	int column_size = input.size / (row_size + 1); // + 1 to consider the newline characters
	
	node*** node_map = malloc(column_size * sizeof(node**));

	char** map = malloc(column_size * sizeof(char*));
	for (int i = 0; i < column_size; i++) {
		map[i] = malloc(row_size);
		memcpy(map[i], &str[i * (row_size + 1)], row_size);
		node_map[i] = calloc(row_size, sizeof(node*));
	}

	int start_pos = first_occurrence_of_char(str, 'S');
	int start_x = start_pos % (row_size + 1);
	int start_y = start_pos / (row_size + 1);

	int end_pos = first_occurrence_of_char(str, 'E');
	int end_x = end_pos % (row_size + 1);
	int end_y = end_pos / (row_size + 1);

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
			if (current_node->dir_index == opposite_directions[dir_index])
				continue;
			int new_x = current_node->x + x_directions[dir_index];
			int new_y = current_node->y + y_directions[dir_index];
			
			int new_g_cost = current_node->g_cost + 1 + (dir_index != current_node->dir_index) * 1000;
			if (map[new_y][new_x] != '#') {
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

	int count = count_best_paths_backwards(current_node, &start_node, node_map, map);
	
	for (int i = 0; i <= all_nodes_last_index; i++) {
		free(all_nodes_list[i]);
	}
	for (int i = 0; i < column_size; i++) {
		free(map[i]);
		free(node_map[i]);
	}
	free(map);
	free(node_map);
	return count;
}

int main(int argc, char* argv[]) {
	init_timing();
	const string input = read_input(16, argv);
	
	printf("Part 1: %d\n", solution_1(input));
	time_function_and_print(solution_1, input, 5000);

	printf("Part 2: %d\n", solution_2(input));
	time_function_and_print(solution_2, input, 5000);

	free(input.content);
	return 0;
}


