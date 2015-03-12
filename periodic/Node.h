#include <sifteo/array.h>
#include "periodic.h"
#include "ObjectPool.h"

class Node : public ObjectPool<Node, 100>
{
public:
	Node();
	~Node();

	char getContent();
	void setContent(char);
	bool getReactionMarker();
	void setReactionMarker(bool);
	void setReactionMarker();
	Node* findChild(char);
	void appendChild(Node*);
	Sifteo::Array<Node*, 10, uint32_t> getChildren();

private:
	char mContent;
	bool mMarker;
	Sifteo::Array<Node*, 10, uint32_t> mChildren;
};

