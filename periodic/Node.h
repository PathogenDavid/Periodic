#include "periodic.h"
#include "ObjectPool.h"
#include "Element.h"

class Node : public ObjectPool<Node, 200>
{
public:
	Node();
	~Node();

	char getContent();
    bondState getReactionType();
    void setContent(char, bondState);
	bool getReactionMarker();
	void setReactionMarker(bool);
	void setReactionMarker();
	Node* findChild(char);
	void appendChild(Node*);

private:
	char mContent;
	bool mMarker;
    bondState mReactionType;
    Node* mSibling;
    Node* mFirstChild;
};

