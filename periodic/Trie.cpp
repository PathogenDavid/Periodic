#include "Trie.h"


Trie::Trie()
{
}



Trie::~Trie()
{
}

void Trie::addBond(char* elementBond)
{
	/*
	Node* current = root;

	if (elementBond.empty())
	{
		current->setReactionMarker();
		return;
	}

	for (int i = 0; i < elementBond.size(); i++)
	{
		Node* child = current->findChild(elementBond[i]);
		if (child != NULL)
		{
			current = child;
		}
		else
		{
			//Node* temp = new Node();

			//temp->setContent(elementBond[i]);
			//current->appendChild(temp);
			//current = temp;
		}
		if (i == elementBond.size() - 1)
			current->setReactionMarker();
	}*/
}


bool Trie::searchBond(char* elementBond)
{
	/*
	Node* current = root;
	while (current != NULL)
	{
		for (int i = 0; i < elementBond.size(); i++)
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
	*/
	return false;
}