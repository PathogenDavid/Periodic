#include "Bond.h"
#include "Element.h"
#include "periodic.h"

#include <sifteo.h>

Bond::Bond()
{
    with = NULL;
    side = BondSide_Invalid;
}

Bond::Bond(BondSide side, Element* with)
{
    this->with = with;
    this->side = side;
}

Element* Bond::GetElement()
{
    return with;
}

BondSide Bond::GetSide()
{
    return side;
}

//BondType Bond::GetType()
//{
//    return type;
//}
//
//void Bond::SetType(BondType type)
//{
//    this->type = type;
//}

BondSide Bond::GetOppositeSide(BondSide side)
{
    switch (side)
    {
    case BondSide_Top:
        return BondSide_Bottom;
    case BondSide_Bottom:
        return BondSide_Top;
    case BondSide_Left:
        return BondSide_Right;
    case BondSide_Right:
        return BondSide_Left;
    default:
        AssertAlways();
        return BondSide_Invalid;
    }
}
