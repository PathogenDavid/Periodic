#include "periodic.h"
#include "ObjectPool.h"

class Node : public ObjectPool<Node, 200>
{
public:
	Node();
	~Node();

	char getContent();
	void setContent(char, int);
	bool getReactionMarker();
	void setReactionMarker(bool);
	void setReactionMarker();
	Node* findChild(char);
	void appendChild(Node*);

private:
	char mContent;
	bool mMarker;
    int mReactionType;
    Node* mSibling;
    Node* mFirstChild;
};

