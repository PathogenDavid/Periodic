#include "BondSolution.h"
#include "periodic.h"

BondSolution::BondSolution()
{
    type = BondType_None;
    compound = NULL;
}

BondSolution::BondSolution(Compound* compound, BondType type)
{
    this->type = type;
    this->compound = compound;
}

BondType BondSolution::GetType()
{
    return type;
}

Compound* BondSolution::GetCompound()
{
    return compound;
}
