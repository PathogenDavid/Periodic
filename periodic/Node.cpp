#include "Node.h"


Node::Node()
{
	mContent = ' ';
    mReactionType = NONE;
	mMarker = false;
    mFirstChild = NULL;
    mSibling = NULL;
}

Node::~Node()
{
}

char Node::getContent() { return mContent; }

bondState Node::getReactionType() { return mReactionType; }

void Node::setContent(char c, bondState i) { mContent = c; mReactionType = i; }

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
