#include "Node.h"


Node::Node()
{
	mContent = ' ';
	mMarker = false;
    mFirstChild = NULL;
    mSibling = NULL;
}

Node::~Node()
{
}

char Node::getContent() { return mContent; }

void Node::setContent(char c, int i) { mContent = c; mReactionType = i; }

bool Node::getReactionMarker() { return mMarker; }

void Node::setReactionMarker() { mMarker = true; }

void Node::appendChild(Node* child)
{
    // Search for an appropriate location for the child pointer
    Node** childDest = &mFirstChild;

    while (*childDest != NULL)
    { childDest = &(*childDest)->mSibling; }

    // Store the child pointer
    *childDest = child;
}

Node* Node::findChild(char c)
{
    Node* child = mFirstChild;
    while (child != NULL)
    {
        if (child->getContent() == c)
        {
            return child;
        }

        child = child->mSibling;
    }
    
	return NULL;
}
