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
    int data;
    Compound* compound;
public:
    BondSolution();
    BondSolution(Compound* compound, BondType type, int data);
    BondType GetType();
    Compound* GetCompound();
    int GetData();
};

#endif
