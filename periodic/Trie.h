#include "Node.h"
#include "Element.h"

class Trie
{
public:
	Trie();
    void addBond(const char*, bondState);
	bondState searchBond(const char*);

private:
	Node* root;
};

