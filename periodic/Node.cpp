#include "Node.h"


Node::Node()
{
	mContent = ' ';
	mMarker = false;
}

Node::Node(int i)
{
    mContent = ' ';
    mMarker = false;
}


Node::~Node()
{
}

char Node::getContent() { return mContent; }

void Node::setContent(char c) { mContent = c; }

bool Node::getReactionMarker() { return mMarker; }

void Node::setReactionMarker(bool marker) { mMarker = marker; }

void Node::appendChild(Node* child) { mChildren.push_back(child); }

Array<Node*, 10, uint32_t> Node::getChildren() { return mChildren; }

Node* Node::findChild(char c)
{
	for (int i = 0; i < mChildren.count; i++)
	{
		Node* temp = mChildren[i];
		if (temp->getContent() == c)
		{
			return temp;
		}
	}
	return NULL;
}
