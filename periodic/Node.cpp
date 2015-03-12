#include "Node.h"


Node::Node()
{
	mContent = ' ';
	mMarker = false;
}


Node::~Node()
{
}

char Node::getContent() { return mContent; }

void Node::setContent(char c, int i) { mContent = c; mReactionType = i; }

bool Node::getReactionMarker() { return mMarker; }

void Node::setReactionMarker() { mMarker = true; }

void Node::appendChild(Node* child) { mChildren.push_back(child); }

Sifteo::Array<Node*, 20, uint32_t> Node::getChildren() { return mChildren; }

Node* Node::findChild(char c)
{
	for (int i = 0; i < mChildren.count(); i++)
	{
		Node* temp = mChildren[i];
		if (temp->getContent() == c)
		{
			return temp;
		}
	}
	return NULL;
}
