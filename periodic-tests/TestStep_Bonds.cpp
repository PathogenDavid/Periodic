#include "TestSteps.h"
#include "Test.h"

#include "Element.h"

// Cation is the positively charged ion.
void __TestIonicBond(const char* cation, const char* anion, int numElectronsDonated, const char* message)
{
    Element c;
    Element a;
    TestMessage(message);
    TestEqBool("Get the cation", Element::GetRawElement(cation, &c), true);
    TestEqBool("Get the anion", Element::GetRawElement(anion, &a), true);

    // React the two elements:
    c.ReactWith(&a);
    TestEqInt("Check the cation's charge after the reaction", c.GetCharge(), numElectronsDonated);
    TestEqInt("Check the anion's charge after the reaction", a.GetCharge(), -numElectronsDonated);
}

#define TestIonicBond(cation, anion, numElectronsDonated) __TestIonicBond(cation, anion, numElectronsDonated, "React " cation " and " anion " with expected charge of " #numElectronsDonated)

void TestStep_Bonds()
{
    TestMessage("Test covalent bonds");
    TestForceFail("It is currently impossible to check for a covalent bond in this build.");

    TestMessage("Test ionic compounds");
    TestIonicBond("Li", "F", 1);
    TestIonicBond("Li", "Cl", 1);
    TestIonicBond("Li", "Br", 1);
    TestIonicBond("Li", "I", 1);
    TestIonicBond("Na", "F", 1);
    TestIonicBond("Na", "Cl", 1);
    TestIonicBond("Na", "Br", 1);
    TestIonicBond("Na", "I", 1);
    TestIonicBond("K", "F", 1);
    TestIonicBond("K", "Cl", 1);
    TestIonicBond("K", "Br", 1);
    TestIonicBond("K", "I", 1);
    TestIonicBond("Rb", "F", 1);
    TestIonicBond("Rb", "Cl", 1);
    TestIonicBond("Rb", "Br", 1);
    TestIonicBond("Rb", "I", 1);
    TestIonicBond("Cs", "F", 1);
    TestIonicBond("Cs", "Cl", 1);
    TestIonicBond("Cs", "Br", 1);
    TestIonicBond("Cs", "I", 1);
    TestIonicBond("Fr", "F", 1);
    TestIonicBond("Fr", "Cl", 1);
    TestIonicBond("Fr", "Br", 1);
    TestIonicBond("Fr", "I", 1);
}
