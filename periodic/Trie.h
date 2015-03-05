<<<<<<< HEAD
#pragma once
=======
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4
#include <sifteo\string.h>
#include "Node.h"

using namespace Sifteo;

class Trie
{

public:
	Trie();
<<<<<<< HEAD
    Trie(Node*);
=======
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4
	~Trie();
	void addBond(String<100>);
	bool searchBond(String<100>);

private:
	Node* root;

};

