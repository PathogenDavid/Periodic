#include "Node.h"
#include "periodic.h"
#include "ObjectPool.h"

class Trie : public ObjectPool<Trie, 1>
{

public:
	Trie();
	~Trie();
	void addBond(const char*, int);
	bool searchBond(const char*);
	Trie* getTrie();

private:
	Node* root;

};

