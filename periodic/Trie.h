#include "Node.h"

using namespace Sifteo;

class Trie
{

public:
	Trie();
	~Trie();
	void addBond(char*);
	bool searchBond(char*);

private:
	Node* root;

};

