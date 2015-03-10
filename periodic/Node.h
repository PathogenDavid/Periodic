#include <sifteo/array.h>
#include "periodic.h";

class Node
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
	Array<Node*, 10, uint32_t> getChildren();

private:
	char mContent;
	bool mMarker;
	Array<Node*, 10, uint32_t> mChildren;
};

