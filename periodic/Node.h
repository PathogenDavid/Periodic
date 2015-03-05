<<<<<<< HEAD
#pragma once
=======
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4
#include <sifteo/math.h>
#include <sifteo/array.h>

using namespace Sifteo;


class Node
{
public:
	Node();
<<<<<<< HEAD
    Node(int);
=======
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4
	~Node();

	char getContent();
	void setContent(char);
	bool getReactionMarker();
<<<<<<< HEAD
	void setReactionMarker(bool);
=======
	void setReactionMarker();
>>>>>>> e8569f07b15b8cea73157a3a617b89e07f6e80d4
	Node* findChild(char);
	void appendChild(Node*);
	Array<Node*, 10, uint32_t> getChildren();

private:
	char mContent;
	bool mMarker;
	Array<Node*, 10, uint32_t> mChildren;
};

