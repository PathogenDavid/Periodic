#include "Node.h"
#include "periodic.h"
#include "ObjectPool.h"

class Trie : public ObjectPool<Trie, 1>
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

