#include <iostream>
#include "Util.h"
#include <unordered_set>
using namespace std;

enum HandType {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_KIND,
	FULL_HOUSE,
	FOUR_KIND,
	FIVE_KIND,
	SIZE
};

struct Hand {
	char cards[5];
	int bid;
	HandType type;
};

inline void getHandCards(const char* str, int& i, unordered_set<char>& set, Hand& hand) {
	for (int j = 0; j < 5; j++) {
		hand.cards[j] = str[i];
		set.insert(str[i]);
		i++;
	}
	i++; // go over the space (it's always there)
}

inline HandType getHandType(const unordered_set<char>& set, const Hand& hand) {
	int size = set.size();
	if (size == 1) // FIVE KIND
		return HandType::FIVE_KIND;
	if (size == 2) { // FOUR KIND or FULL HOUSE
		// we can figure out which one it is by just counting any one of the hands
		char anyChar = hand.cards[0];
		int count = 0;
		for (int j = 0; j < 5; j++) {
			if (hand.cards[j] == anyChar)
				count++;
		}
		
		if (count == 1 || count == 4)
			return HandType::FOUR_KIND;
		return HandType::FULL_HOUSE;
	}
	if (size == 3) { // TWO PAIR or THREE KIND
		char uniqueChars[2];
		int size = 0;

		std::unordered_set<char>::const_iterator it = set.cbegin();
		uniqueChars[0] = *it;
		it++;
		uniqueChars[1] = *it;

		int counts[3] = {0, 0, 0};
		for (int j = 0; j < 5; j++) {
			char c = hand.cards[j];
			if (c == uniqueChars[0])
				counts[0]++;
			else if (c == uniqueChars[1])
				counts[1]++;
			else
				counts[2]++;
		}
		if (counts[0] == 3 || counts[1] == 3 || counts[2] == 3)
			return HandType::THREE_KIND;
		return HandType::TWO_PAIR;

	}
	if (size == 4) // ONE PAIR
		return HandType::ONE_PAIR;
	//else if (size == 5) // HIGH CARD
	return HandType::HIGH_CARD;
}

inline HandType getHandTypeJoker(const unordered_set<char>& set, const Hand& hand) {
	int size = set.size();
	if (size == 1 || size == 2) // FIVE KIND
		return HandType::FIVE_KIND;
	if (size == 3) { // FOUR KIND or FULL HOUSE
		
		/* we know that this hand contains 3 chars, say AB and J. Say the hand is ABJxx. Order doesn't matter.
		The only situation when a hand isn't a full house is when xx are not the same, meaning there's two of A and two of B.
		All we have to do is find a non-J character, count it, check if it's exactly 2, 
		while also making sure there's only 1 J (so there must be two of the other character left) */
		int i = 0;
		char anyChar;
		do {
			anyChar = hand.cards[i];
			i++;
		} while (anyChar == 'J');

		int count = 0;
		int jCount = 0;
		for (int j = 0; j < 5; j++) {
			char c = hand.cards[j];
			if (c == anyChar)
				count++;
			else if (c == 'J')
				jCount++;

		}
		if (count == 2 && jCount == 1)
			return HandType::FULL_HOUSE;
		return HandType::FOUR_KIND;
	}
	if (size == 4) { // THREE KIND
		// TWO PAIR is impossible if there is a joker and 3 characters.
		return HandType::THREE_KIND;
	}
	// if (size == 5) // ONE PAIR
	return HandType::ONE_PAIR;

}



inline int readHandBid(const char* str, int& i) {
	// numbers are at most 4 digits, so lets assume they are and divide by 10 if we're wrong
	// (there is only 1 4-digit hand in my input file)
	int multiplier = 1000;
	int num = 0;

	num += (str[i] - '0') * multiplier;
	i++;
	for (int j = 0; j < 3; j++) {
		char c = str[i];
		if (c == '\n') {
			num /= multiplier;
			break;
		}
		multiplier /= 10;
		num += (str[i] - '0') * multiplier;
		i++;
	}
	return num;
}


inline Hand getNextHand(const char* str, int& i) {
	Hand hand;
	unordered_set<char> set = unordered_set<char>();
	getHandCards(str, i, set, hand);
	hand.type = getHandType(set, hand);
	hand.bid = readHandBid(str, i);

	i++; // move one more to get over the newline
	return hand;
}
inline Hand getNextHandJoker(const char* str, int& i) {
	Hand hand;
	unordered_set<char> set = unordered_set<char>();
	getHandCards(str, i, set, hand);
	if (set.count('J') == 0)
		hand.type = getHandType(set, hand);
	else
		hand.type = getHandTypeJoker(set, hand);
	hand.bid = readHandBid(str, i);

	i++; // move one more to get over the newline
	return hand;
}


int jokerWorth = 11;
// i'm *pretty* sure this is still faster than an an entire hashset object dedicated to 6 chars, but it looks very bad...
inline int getCardWorth(const char& c) {
	if (c >= '2' && c <= '9')
		return c - '0';
	else if (c == 'T')
		return 10;
	else if (c == 'J')
		return jokerWorth;
	else if (c == 'Q')
		return 12;
	else if (c == 'K')
		return 13;
	return 14;
}

inline bool isRightHandStronger(const Hand& left, const Hand& right) {
	for (int i = 0; i < 5; i++) {
		int l = getCardWorth(left.cards[i]);
		int r = getCardWorth(right.cards[i]); 
		if (r > l)
			return true;
		else if (r != l)
			return false;
	}
	return true; // sure whatever
}


inline void addHandToHandVector(const Hand& hand, vector<Hand>& vec) {
	if (vec.empty()) {
		vec.push_back(hand);
		return;
	}
    int low = 0;
    int high = vec.size();

    while (low < high) {
        int middle = low + (high - low) / 2;
        if (isRightHandStronger(vec[middle], hand))
            low = middle + 1;
		else
            high = middle;
    }

    vec.insert(vec.begin() + low, hand);
}

int solution_1(const string& input) {
	jokerWorth = 11;
	vector<Hand>* handsTypeSeparator[HandType::SIZE];
	for (int i = 0; i < HandType::SIZE; i++)
		handsTypeSeparator[i] = new vector<Hand>();


	const char* str = input.c_str();
	int i = 0;
	int size = input.size();
	while (i < size) {
		Hand hand = getNextHand(str, i);
		vector<Hand>& vec = *handsTypeSeparator[hand.type];
		addHandToHandVector(hand, vec);
	}
	
	int winnings = 0;
	int rank = 1;
	for (int i = 0; i < HandType::SIZE; i++) {
		vector<Hand>& vec = *handsTypeSeparator[i];
		int vecSize = vec.size();
		for (int j = 0; j < vecSize; j++) {
			int bid = vec[j].bid;
			winnings += rank * bid;
			rank++;
		}
		delete handsTypeSeparator[i];
	}

	return winnings;
}

int solution_2(const string& input) {
	jokerWorth = 1;
	vector<Hand>* handsTypeSeparator[HandType::SIZE];
	for (int i = 0; i < HandType::SIZE; i++)
		handsTypeSeparator[i] = new vector<Hand>();


	const char* str = input.c_str();
	int i = 0;
	int size = input.size();
	while (i < size) {
		Hand hand = getNextHandJoker(str, i);
		vector<Hand>& vec = *handsTypeSeparator[hand.type];
		addHandToHandVector(hand, vec);
	}
	
	int winnings = 0;
	int rank = 1;
	for (int i = 0; i < HandType::SIZE; i++) {
		vector<Hand>& vec = *handsTypeSeparator[i];
		int vecSize = vec.size();
		for (int j = 0; j < vecSize; j++) {
			winnings += rank * vec[j].bid;
			rank++;
		}
		delete handsTypeSeparator[i];
	}

	return winnings;
}

int main(int argc, char* argv[]) {
	string input = readInput(7, argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	timeFunctionAndPrint(solution_1, input, 10000);
	
	cout << '\n';

	cout << "Part 2: " << solution_2(input) << '\n';
	timeFunctionAndPrint(solution_2, input, 10000);
	return 0;
}