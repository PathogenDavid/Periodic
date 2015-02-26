#include "BondSolution.h"
#include "periodic.h"

BondSolution::BondSolution()
{
    type = BondType_None;
    compound = NULL;
    data = 0;
}

BondSolution::BondSolution(Compound* compound, BondType type, int data)
{
    this->type = type;
    this->compound = compound;
    this->data = data;
}

BondType BondSolution::GetType()
{
    return type;
}

Compound* BondSolution::GetCompound()
{
    return compound;
}

int BondSolution::GetData()
{
    return data;
}
