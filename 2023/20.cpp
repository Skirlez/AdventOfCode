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
			PulseType pulseType, Node*& end) {
	if (target->type == 0) {
		if (pulseType == PulseType::HIGH)
			return;
		target->state = !target->state;
		addToQueueAndProgress(target, modules, static_cast<PulseType>(target->state), end);
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
}

inline void sendPulseAndSum(Module* target, Module* sender, Module** modules, vector<uint16_t>** inputMap,
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
	int ind = !((target->state & ((1 << size) - 1)) == ((1 << size) - 1)); // if false if all the bits are set, true otherwise.
	addToQueueAndProgress(target, modules, static_cast<PulseType>(ind), end);
	sums[ind] += target->destinationsAmount;
}


inline Module* parseModules(const char* str, size_t size, Module** modules, vector<uint16_t>** inputMap, vector<Module*>& moduleTracker, vector<vector<uint16_t>*>& vectorTracker) {
	Module* broadcaster = new Module { 0, -1 };
	moduleTracker.push_back(broadcaster);
	size_t pos = 0;
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
	return broadcaster;
}

const uint16_t modulesMax = (('z' - 'a') << 5 | ('z' - 'a'));

int solution_1(const string& input) {
	Module* modules[modulesMax + 1];
	fill(modules, modules + modulesMax + 1, nullptr);

	vector<uint16_t>* inputMap[modulesMax + 1];
	fill(inputMap, inputMap + modulesMax + 1, nullptr);

	vector<vector<uint16_t>*> vectorTracker;
	vector<Module*> moduleTracker;

	const char* str = input.c_str();
	size_t size = input.size();

	Module* broadcaster = parseModules(str, size, modules, inputMap, moduleTracker, vectorTracker);
	
	Node* start = new Node {broadcaster, nullptr, PulseType::LOW, nullptr};
	Node* endBroadcaster = start;
	addToQueueAndProgress(broadcaster, modules, PulseType::LOW, endBroadcaster);
	
	size_t sums[2] = {(1 + broadcaster->destinationsAmount) * 1000, 0};
	for (int i = 0; i < 1000; i++) {	
		Node* end = endBroadcaster;
		Node* node = start->next;
		while (node != nullptr) {
			sendPulseAndSum(node->target, node->sender, modules, inputMap, node->pulseType, sums, end);
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

inline uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


const uint16_t rx = (('r' - 'a') << 5 | ('x' - 'a'));
uint64_t solution_2(const string& input) {
	Module* modules[modulesMax + 1];
	fill(modules, modules + modulesMax + 1, nullptr);

	vector<uint16_t>* inputMap[modulesMax + 1];
	fill(inputMap, inputMap + modulesMax + 1, nullptr);

	vector<vector<uint16_t>*> vectorTracker;
	vector<Module*> moduleTracker;

	const char* str = input.c_str();
	size_t size = input.size();

	Module* broadcaster = parseModules(str, size, modules, inputMap, moduleTracker, vectorTracker);
	
	uint16_t rxSenderName = 0;
	for (size_t i = 0; i < moduleTracker.size(); i++) {
		if (moduleTracker[i]->destinationsAmount == 1 && moduleTracker[i]->destinations[0] == rx) {
			rxSenderName = moduleTracker[i]->name;
			break;
		}
	}

	// 1. Only conjuction modules send to the (always) conjuction module that sends to rx.
	// 2. They all have a consistent cycle where they send Kow a lot, then High one time
	// 3. Day 8 came back in more than one way (finding the LCM of all the cycle lengths)

	// vector keeping track of senders to the sender to rx
	vector<Module*> senderSenders;

	for (size_t i = 0; i < moduleTracker.size(); i++) {
		if (moduleTracker[i]->destinationsAmount == 1 && moduleTracker[i]->destinations[0] == rxSenderName)
			senderSenders.push_back(moduleTracker[i]);
	}
	uint64_t cycleLengths[senderSenders.size()];
	fill(cycleLengths, cycleLengths + senderSenders.size(), 0);

	Node* start = new Node {broadcaster, nullptr, PulseType::LOW, nullptr};
	Node* endBroadcaster = start;
	addToQueueAndProgress(broadcaster, modules, PulseType::LOW, endBroadcaster);
	

	size_t i = 1;
	unsigned int cyclesFound = 0;
	while (true) {	
		Node* end = endBroadcaster;
		Node* node = start->next;
		while (node != nullptr) {
			if (node->pulseType == PulseType::HIGH) {
				for (size_t j = 0; j < senderSenders.size(); j++) {
					if (senderSenders[j] == node->sender) {
						if (cycleLengths[j] == 0)
							cyclesFound++;
						cycleLengths[j] = i;
					}
				}
			}
			sendPulse(node->target, node->sender, modules, inputMap, node->pulseType, end);
			node = node->next;
		}
		if (cyclesFound >= senderSenders.size())
			break;
		//cout << '\n';
		node = endBroadcaster->next;
		while (node != nullptr) {
			Node* next = node->next;
			delete node;
			node = next;
		}
		i++;
	}
	delete start;

	for (size_t i = 0; i < vectorTracker.size(); i++)
		delete vectorTracker[i];
	for (size_t i = 0; i < moduleTracker.size(); i++) 
		delete moduleTracker[i];

	uint64_t lcm = cycleLengths[0];
	for (size_t i = 1; i < senderSenders.size(); i++)
		lcm *= cycleLengths[i] / gcd(lcm, cycleLengths[i]);

	return lcm;
}

int main(int argc, char* argv[]) {
	string input = readInput(20, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 1000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint((SolutionFunction)solution_2, input, 1000);
	return 0;
}