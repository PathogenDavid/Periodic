#ifndef __BONDSOLUTION_H__
#define __BONDSOLUTION_H__

class Compound;

enum BondType
{
    BondType_None,
    BondType_Ionic,
    BondType_Covalent,
    BondType_Potential,
    BondType_Cound,
    BondType_Invalid
};

class BondSolution
{
private:
    BondType type;
    Compound* compound;
public:
    BondSolution();
    BondSolution(Compound* compound, BondType type);
    BondType GetType();
    Compound* GetCompound();
};

#endif
