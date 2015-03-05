#include "Trie.h"


Trie::Trie()
{
<<<<<<< HEAD
	
}

Trie::Trie(Node* n)
{
    this->root = n;
}
=======
	root = new Node();
}

>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4


Trie::~Trie()
{
}

void Trie::addBond(String<100> elementBond)
{
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
<<<<<<< HEAD
			Node* temp;
temp->setContent(' ');
temp->setReactionMarker(false);
=======
			Node* temp = new Node();
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4

			temp->setContent(elementBond[i]);
			current->appendChild(temp);
			current = temp;
		}
		if (i == elementBond.size() - 1)
			current->setReactionMarker();
	}
}


bool Trie::searchBond(String<100> elementBond)
{
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
	return false;
}