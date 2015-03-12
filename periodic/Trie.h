#include "Node.h"
#include "periodic.h"
#include "ObjectPool.h"

class Trie : public ObjectPool<Trie, 1>
{

public:
	Trie();
    Trie(Node*);
	~Trie();
	void addBond(const char*, int);
	bool searchBond(const char*);

private:
	Node* root;

};

