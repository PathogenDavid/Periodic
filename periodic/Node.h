#include <sifteo/array.h>
#include "periodic.h"
#include "ObjectPool.h"

class Node : public ObjectPool<Node, 200>
{
public:
	Node();
	~Node();

	char getContent();
	void setContent(char, int);
	bool getReactionMarker();
	void setReactionMarker(bool);
	void setReactionMarker();
	Node* findChild(char);
	void appendChild(Node*);
	Sifteo::Array<Node*, 20, uint32_t> getChildren();

private:
	char mContent;
	bool mMarker;
	int mReactionType;
	Sifteo::Array<Node*, 20, uint32_t> mChildren;
};

