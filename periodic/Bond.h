#ifndef __BOND_H__
#define __BOND_H__

#include "BondSolution.h"
#include "Set.h"

class Element;
class Compound;

enum BondSide // Should match order and values of Sifteo's Side type.
{
    BondSide_Top,
    BondSide_Left,
    BondSide_Bottom,
    BondSide_Right,
    BondSide_Count,
    BondSide_Invalid = -1
};

class Bond
{
private:
    Element* with;
    BondSide side;
    BondSolution solutions[MAX_COMPOUNDS]; // TODO: Should we use a LinkedList here to save memory in the Element class?
public:
    Bond();
    Bond(BondSide side, Element* with);

    Element* GetElement();
    BondSide GetSide();

    static BondSide GetOppositeSide(BondSide side);

    BondType GetTypeFor(Compound* compound);
    void SetTypeFor(Compound* compound, BondType type, int data);

    int GetDataFor(Compound* compound);
};

#endif
