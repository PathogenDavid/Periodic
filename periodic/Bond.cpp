#include "Bond.h"
#include "Element.h"
#include "Compound.h"
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
    PeriodicMemset(solutions, 0, sizeof(solutions));
}

Element* Bond::GetElement()
{
    return with;
}

BondSide Bond::GetSide()
{
    return side;
}

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

BondType Bond::GetTypeFor(Compound* compound)
{
    if (compound == NULL)
    { return BondType_None; }

    BondSolution* solution = &solutions[compound->GetIndex()];
    
    if (solution->GetCompound() == NULL)
    { return BondType_None; }

    Assert(solution->GetCompound() == compound);
    return solution->GetType();
}

void Bond::SetTypeFor(Compound* compound, BondType type)
{
    BondSolution* solution = &solutions[compound->GetIndex()];
    Assert(solution->GetCompound() == NULL || solution->GetCompound() == compound);
    solutions[compound->GetIndex()] = BondSolution(compound, type);
}
