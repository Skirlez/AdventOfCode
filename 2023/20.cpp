#include <iostream>
#include "Util.h"
#include <climits>
using namespace std;

enum PulseType {
	LOW,
	HIGH
};

struct Module { 
	uint16_t name;
	int type;
	int32_t state = 0;
	size_t destinationsAmount;
	int* destinations;
    ~Module() {
        delete[] destinations;
    }
};


struct Node {
	Module* target;
	Module* sender;
	PulseType pulseType;
	Node* next;
};


void printName(uint16_t name) {
	cout << ((char)((name >> 5) + 'a'));
	cout << ((char)((name & 0b00011111) + 'a'));
}

inline void addToQueueAndProgress(Module* sender, Module** modules, PulseType pulseType, Node*& end) {
	for (size_t i = 0; i < sender->destinationsAmount; i++) {
		if (modules[sender->destinations[i]] == nullptr)
			continue;
		end->next = new Node { modules[sender->destinations[i]], sender, pulseType, nullptr };
		end = end->next;
	}
}


inline void sendPulse(Module* target, Module* sender, Module** modules, vector<uint16_t>** inputMap,
			PulseType pulseType, size_t* sums, Node*& end) {
	if (target->type == 0) {
		if (pulseType == PulseType::HIGH)
			return;
		target->state = !target->state;
		addToQueueAndProgress(target, modules, static_cast<PulseType>(target->state), end);
		sums[target->state] += target->destinationsAmount;
		return;
	}

	//else (module->type == 1) {
	vector<uint16_t>& vec = *inputMap[target->name];
	int32_t i = 0;
	int32_t size = vec.size();
	while (i < size && vec[i] != sender->name) 
		i++;
	target->state &= ~(1 << i);
	target->state |= (((uint32_t)pulseType) << i);
	int ind = !((target->state & ((1 << size) - 1)) == ((1 << size) - 1));
	addToQueueAndProgress(target, modules, static_cast<PulseType>(ind), end);
	sums[ind] += target->destinationsAmount;
}


int solution_1(const string& input) {
	const uint16_t max = (('z' - 'a') << 5 | ('z' - 'a'));
	Module* modules[max + 1];
	fill(modules, modules + max + 1, nullptr);


	vector<uint16_t>* inputMap[max + 1];
	fill(inputMap, inputMap + max + 1, nullptr);

	vector<vector<uint16_t>*> vectorTracker;
	vector<Module*> moduleTracker;

	const char* str = input.c_str();
	size_t size = input.size();
	size_t pos = 0;
	Module* broadcaster = new Module { 0, -1 };
	moduleTracker.push_back(broadcaster);
	while (pos < size) {
		Module* module;
		if ((str[pos] == 'b')) {
			module = broadcaster;
			pos += 15;
		}
		else {
			module = new Module {};
			moduleTracker.push_back(module);
			module->name = (((str[pos + 1] - 'a') << 5) | (str[pos + 2] - 'a'));
			module->type = (str[pos] == '%') ? 0 : 1;
			pos += 7;
		}
		size_t start = pos;
		while (str[pos] != '\n')
			pos++;
		size_t end = pos;
		module->destinationsAmount = ((pos - start) >> 2) + 1;
		module->destinations = new int[module->destinationsAmount];
		int*& arr = module->destinations;

		pos = start;



		size_t ind = 0;
		while (pos < end) {
			uint16_t destName = (((str[pos] - 'a') << 5) | (str[pos + 1] - 'a'));
			arr[ind] = destName;

			vector<uint16_t>* inputs;
			if (inputMap[destName] == nullptr) {
				inputs = new vector<uint16_t>();
				vectorTracker.push_back(inputs);
				inputMap[destName] = inputs;
			}
			else
				inputs = inputMap[destName];

			inputs->push_back(module->name);

			ind++;
			pos += 4;
		}

		
		pos = end + 1;
		if (module != broadcaster)
			modules[module->name] = module;
	}
	
	Node* start = new Node {broadcaster, nullptr, PulseType::LOW, nullptr};
	Node* endBroadcaster = start;
	addToQueueAndProgress(broadcaster, modules, PulseType::LOW, endBroadcaster);
	
	size_t sums[2] = {(1 + broadcaster->destinationsAmount) * 1000, 0};
	for (int i = 0; i < 1000; i++) {	
		Node* end = endBroadcaster;
		Node* node = start->next;
		while (node != nullptr) {
			sendPulse(node->target, node->sender, modules, inputMap, node->pulseType, sums, end);
			node = node->next;
		}
		//cout << '\n';
		node = endBroadcaster->next;
		while (node != nullptr) {
			Node* next = node->next;
			delete node;
			node = next;
		}
	}
	delete start;

	for (size_t i = 0; i < vectorTracker.size(); i++)
		delete vectorTracker[i];
	for (size_t i = 0; i < moduleTracker.size(); i++) 
		delete moduleTracker[i];
	
	return sums[0] * sums[1];
}

int solution_2(const string& input) {
	return 0;
}

int main(int argc, char* argv[]) {
	string input = readInput(20, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 100);
	
	//cout << '\n';

	//cout << "Part 2: " << solution_2(input) << '\n';
	//timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}