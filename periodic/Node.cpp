#include "Node.h"


Node::Node()
{
	mContent = ' ';
	mMarker = false;
}

<<<<<<< HEAD
Node::Node(int i)
{
    mContent = ' ';
    mMarker = false;
}
=======
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4


Node::~Node()
{
}

char Node::getContent() { return mContent; }

void Node::setContent(char c) { mContent = c; }

bool Node::getReactionMarker() { return mMarker; }

<<<<<<< HEAD
void Node::setReactionMarker(bool marker) { mMarker = marker; }
=======
void Node::setReactionMarker() { mMarker = true; }
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4

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
