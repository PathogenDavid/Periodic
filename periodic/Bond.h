#ifndef __BOND_H__
#define __BOND_H__

class Element;

enum BondSide // Should match order and values of Sifteo's Side type.
{
    BondSide_Top,
    BondSide_Left,
    BondSide_Bottom,
    BondSide_Right,
    BondSide_Count,
    BondSide_Invalid = -1
};

enum BondType
{
    BondType_None,
    BondType_Ionic,
    BondType_Covalent,
    BondType_Potential,
};

class Bond
{
private:
    Element* with;
    BondSide side;
    //BondType type;
public:
    Bond();
    Bond(BondSide side, Element* with);

    Element* GetElement();
    BondSide GetSide();
    //BondType GetType();
    //void SetType(BondType type);

    static BondSide GetOppositeSide(BondSide side);
};

#endif
