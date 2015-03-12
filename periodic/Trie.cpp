#include "Trie.h"

Trie::Trie()
{	
	root = new Node();
}


Trie::~Trie()
{
}
void Trie::addBond(const char* elementBond, int bondType)
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
			current->setReactionMarker();
	}
}



bool Trie::searchBond(const char* elementBond)
{
	int length = strlen(elementBond);
	Node* current = root;
	while (current != NULL)
	{
		for (int i = 0; i < length; i++)
		{
			Node* temp = current->findChild(elementBond[i]);
			if (temp == NULL)
				return false;
			current = temp;
		}

		if (current->getReactionMarker())
			return true;
		else return false;
	}
	return false;
}


Trie* Trie::getTrie()
{
	return this;
}