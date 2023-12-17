// Ahhh... A star.......


#include <iostream>
#include <array>
#include "Util.h"
using namespace std;


struct Node {
	int fCost;
	size_t x;
	size_t y;
	int dir;
	int sameDir;
	Node* prevNode = nullptr;
};

inline void registerNode(Node* node, vector<Node*>& nodes) {
    int low = 0;
    int high = nodes.size();

    while (low < high) {
        int middle = low + (high - low) / 2;
        if (nodes[middle]->fCost > node->fCost)
            low = middle + 1;
		else
            high = middle;
    }

    nodes.insert(nodes.begin() + low, node);
}

template<bool Ultra> inline bool nodeInNodeVector(Node* node, vector<Node*>& vec) {
	for (size_t i = 0; i < vec.size(); i++) {
		if (node->fCost >= vec[i]->fCost 
				&& node->dir == vec[i]->dir
				&& ((Ultra && node->sameDir == vec[i]->sameDir) 
					|| (!Ultra && node->sameDir >= vec[i]->sameDir)))
			return true;
	}
	return false;
}

const int cardinalDirections[8] = {1, 0, 0, 1, -1, 0, 0, -1};
template<bool Ultra> void registerNeighbors(vector<string>& lines, Node& node, 
		const size_t& xEnd, const size_t yEnd, vector<Node*>& nodes, vector<vector<array<vector<Node*>, 4>>>& allNodes) {
	
	const int sameDirMax = (Ultra) ? 10 : 3;
	for (int i = 0; i < 8; i += 2) {
		if (Ultra) {
			if (node.dir != -1 && node.sameDir < 3 && node.dir != i)
				continue;
		}
		int xOffset = cardinalDirections[i];
		int yOffset = cardinalDirections[i + 1];

		if ((node.x == 0 && xOffset == -1) || (node.x == xEnd && xOffset == 1)
				|| (node.y == 0 && yOffset == -1) || (node.y == yEnd && yOffset == 1)) {
			continue;
		}
		if ((i + 4) % 8 == node.dir)
			continue;

		int sameDir;
		if (i != node.dir)
			sameDir = 0;
		else
			sameDir = node.sameDir + 1;
		if (sameDir >= sameDirMax)
			continue;
		size_t x = node.x + xOffset;
		size_t y = node.y + yOffset;
	
		
		int fCost = (lines[y][x] - '0') + node.fCost - xOffset - yOffset;

		Node* newNode = new Node { fCost, x, y, i, sameDir, &node};
		
		if (nodeInNodeVector<Ultra>(newNode, allNodes[y][x][i >> 1])) {
			delete newNode;
			continue;
		}

		allNodes[y][x][i >> 1].push_back(newNode);
		registerNode(newNode, nodes);
	}
}


template<bool Ultra> int sharedSolution(const string& input) {
	size_t lineLength = findChar(input.c_str(), '\n') + 1;

	vector<string> lines = splitRectangularString(input, lineLength);
	vector<string> copy = lines;
	size_t linesAmount = lines.size();

	const int dirAmount = 4;

	// save me
	vector<vector<array<vector<Node*>, dirAmount>>> allNodes 
		= vector<vector<array<vector<Node*>, dirAmount>>>(linesAmount, vector<array<vector<Node*>, dirAmount>>(lineLength));
	
	vector<Node*> nodes;

	const size_t xEnd = lineLength - 2;
	const size_t yEnd = linesAmount - 1;

	Node firstNode = Node {(xEnd + yEnd), 0, 0, -1, 0, nullptr};
	registerNeighbors<Ultra>(lines, firstNode, xEnd, yEnd, nodes, allNodes);
	Node* node = nodes.back();
	while (true) {
		nodes.pop_back();
		//cout << '(' << node->x << ", " << node->y << ") - " << node->fCost << "! ";

		if (node->x == xEnd && node->y == yEnd && ((Ultra && node->sameDir >= 3) || !Ultra))
			break;
		
		registerNeighbors<Ultra>(lines, *node, xEnd, yEnd, nodes, allNodes);
		node = nodes.back();
	}
	int totalHeat = node->fCost;
	
	/*
	Node* nodePtr = node;
	while (nodePtr->prevNode != nullptr) {
		copy[nodePtr->y][nodePtr->x] = '*';
		nodePtr = nodePtr->prevNode;
	}
	
	cout << '\n';
	for (size_t i = 0; i <= yEnd; i++) {
		for (size_t j = 0; j < lineLength - 1; j++) {
			cout << copy[i][j];
		}
		cout << '\n';
	}
	cout << '\n';
	*/

	for (size_t x = 0; x < allNodes.size(); x++) {
		for (size_t y = 0; y < allNodes[x].size(); y++) {
			for (size_t z = 0; z < dirAmount; z++) {
				for (size_t w = 0; w < allNodes[x][y][z].size(); w++)
					delete allNodes[x][y][z][w];
				
			}
		}
	}

	return totalHeat;
}
int solution_1(const string& input) {
	return sharedSolution<false>(input);
}

int solution_2(const string& input) {
	return sharedSolution<true>(input);
}

int main(int argc, char* argv[]) {
	string input = readInput(17, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 15);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10);
	return 0;
}


