#include "Trie.h"

Trie::Trie()
{	
	root = new Node();
}

void Trie::addBond(const char* elementBond, bondState bondType)
{
	Node* current = root;
	int length = strlen(elementBond);

	if (length == 0)
	{
		current->setReactionMarker();
		return;
	}

	for (int i = 0; i < length; i++)
	{
		Node* child = current->findChild(elementBond[i]);
		if (child != NULL)
		{
			current = child;
		}
		else
		{
			Node* temp = new Node();
			temp->setContent(elementBond[i], bondType);
			current->appendChild(temp);
			current = temp;
		}

		if (i == length - 1)
        {
            Assert(current->getReactionType() == bondType); // Ensure the user doesn't add two identical reactions with different bond types.
			current->setReactionMarker();
        }
	}
}

bondState Trie::searchBond(const char* elementBond)
{
	int length = strlen(elementBond);
	Node* current = root;

    if (current == NULL)
    { return NONE; }

	for (int i = 0; i < length; i++)
	{
		current = current->findChild(elementBond[i]);

        // We didn't complete the reaction, so we bail out.
        //TODO: Should we return POTENTIAL here instead?
        if (current == NULL)
        { return NONE; }
	}

    // If this node marks a complete reaction, we return the type of reaction it should be.
	if (current->getReactionMarker())
    { return current->getReactionType(); }
	
    //TODO: Do we want to return POTENTIAL in this case instead?
    return NONE;
}
