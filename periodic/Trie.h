#pragma once
#include <sifteo\string.h>
#include "Node.h"

using namespace Sifteo;

class Trie
{

public:
	Trie();
	~Trie();
	void addBond(String<100>);
	bool searchBond(String<100>);

private:
	Node* root;

};

