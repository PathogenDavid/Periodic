#pragma once
#include <sifteo/math.h>
#include <sifteo/array.h>


class Node
{
public:
	Node();
	~Node();

	char getContent();
	void setContent(char);
	bool getReactionMarker();
	void setReactionMarker();
	Node* findChild(char);
	void appendChild(Node*);
	Sifteo::Array<Node*, 10, uint32_t> getChildren();

private:
	char mContent;
	bool mMarker;
	Sifteo::Array<Node*, 10, uint32_t> mChildren;
};

