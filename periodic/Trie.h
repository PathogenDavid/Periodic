#include "Node.h"
#include "periodic.h";

class Trie
{

public:
	Trie();
    Trie(Node*);
	~Trie();
	void addBond(const char*);
	bool searchBond(const char*);

private:
	Node* root;

};

