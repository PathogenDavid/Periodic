#include "TestSteps.h"
#include "Test.h"

#include "Element.h"

//! Supporting function for TestCovalentBond
void __TestCovalentBond(const char* a, const char* b, int numElectronsShared, const char* message)
{
    Element ea;
    Element eb;
	int numOuterElectronA;
	int numOuterElectronB;
    TestMessage(message);
    TestEqBool("Get the first element", Element::GetRawElement(a, &ea), true);
    TestEqBool("Get the second element", Element::GetRawElement(b, &eb), true);
	numOuterElectronA = ea.GetNumOuterElectrons();
	numOuterElectronB = eb.GetNumOuterElectrons();

	// React the two elements:ea.ReactWith(&eb);
    //TestForceFail("It is currently impossible to check for a covalent bond in this build.");
	TestEqBool("Check that a reaction occurs", ea.ReactWith(&eb), true);
	TestEqInt("Check the first element's shared electron number after the reaction", ea.GetSharedElectrons(), numElectronsShared);
	TestEqInt("Check the second element's shared electron number after the reaction", eb.GetSharedElectrons(), numElectronsShared);
	TestEqInt("Check the first element's outer electron number after the reaction", ea.GetNumOuterElectrons(), (numOuterElectronA+1));
	TestEqInt("Check the second element's outer electron number after the reaction", eb.GetNumOuterElectrons(), (numOuterElectronB+1));

}

//! Supporting function for TestIonicBond
void __TestIonicBond(const char* cation, const char* anion, int numElectronsDonated, const char* message)
{
    // Note: Cation is the positively charged ion.
    Element c;
    Element a;
    TestMessage(message);
    TestEqBool("Get the cation", Element::GetRawElement(cation, &c), true);
    TestEqBool("Get the anion", Element::GetRawElement(anion, &a), true);

    // React the two elements:
    TestEqBool("Check that a reaction occurs", c.ReactWith(&a), true);
    TestEqInt("Check the cation's charge after the reaction", c.GetCharge(), numElectronsDonated);
    TestEqInt("Check the anion's charge after the reaction", a.GetCharge(), -numElectronsDonated);
}

//! Supporting macro for TestCovalentBond
#define _TestCovalentBond(a, b, numElectronsShared) __TestCovalentBond(a, b, numElectronsShared, "React " a " and " b " with expected electrons shared count of " #numElectronsShared)
//! Supporting macro for TestIonicBond
#define _TestIonicBond(cation, anion, numElectronsDonated) __TestIonicBond(cation, anion, numElectronsDonated, "React " cation " and " anion " with expected charge of " #numElectronsDonated)

//! Tests that two elements covalently bond as expected.
//! This is a supporting macro that ensures bonds are commutative.
#define TestCovalentBond(a, b, numElectronsShared) _TestCovalentBond(a, b, numElectronsShared); _TestCovalentBond(b, a, numElectronsShared)

//! Tests that two elements ionically bond as expected.
//! This is a supporting macro that ensures opposite bonds are mirrored.
#define TestIonicBond(cation, anion, numElectronsDonated) _TestIonicBond(cation, anion, numElectronsDonated); _TestIonicBond(anion, cation, -numElectronsDonated)

void TestStep_Bonds()
{
	TestMessage("Test covalent bonds");
    //#if 0
    TestCovalentBond("H", "H", 2);
    TestCovalentBond("Li", "H", 2);
    TestCovalentBond("Na", "H", 2);
    TestCovalentBond("K", "H", 2);
    TestCovalentBond("Cs", "H", 2);
    TestCovalentBond("Fr", "H", 2);
    TestCovalentBond("F", "F", 2);
    TestCovalentBond("Cl", "Cl", 2);
    TestCovalentBond("Br", "Br", 2);
    TestCovalentBond("I", "I", 2);
    TestCovalentBond("F", "Cl", 2);
    TestCovalentBond("F", "Br", 2);
    TestCovalentBond("F", "I", 2);
    TestCovalentBond("Cl", "Br", 2);
    TestCovalentBond("Cl", "I", 2);
    TestCovalentBond("H", "F", 2);
    TestCovalentBond("H", "Cl", 2);
    TestCovalentBond("H", "Br", 2);
    TestCovalentBond("H", "I", 2);
    //TestCovalentBond("H", "At", 2);  // no At in rawElements[]
    //#endif
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
