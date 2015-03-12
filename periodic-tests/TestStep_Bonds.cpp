#include "TestSteps.h"
#include "Test.h"
#include "periodic.h"
#include "Element.h"
#include "Reaction.h"
#include "BondSolution.h"

//! Supporting function for TestCovalentBond
void __TestCovalentBond(const char* a, const char* b, int numElectronsShared, const char* message)
{
    // Load the elements
    Element ea;
    Element eb;
	int numOuterElectronA;
	int numOuterElectronB;
    TestMessage(message);
    TestEqBool("Get the first element", Element::GetRawElement(a, &ea), true);
    TestEqBool("Get the second element", Element::GetRawElement(b, &eb), true);
	numOuterElectronA = ea.GetNumOuterElectrons();
	numOuterElectronB = eb.GetNumOuterElectrons();

    // Create a reaction and process it
    Reaction reaction;
    reaction.Add(&ea);
    ea.AddBond(BondSide_Right, &eb);
    TestEqBool("Check that a reaction occurs", reaction.Process(), true);

    // Verify the result
	TestEqInt("Check the first element's shared electron number after the reaction", ea.GetSharedElectrons(), numElectronsShared);
	TestEqInt("Check the second element's shared electron number after the reaction", eb.GetSharedElectrons(), numElectronsShared);
	TestEqInt("Check the first element's outer electron number after the reaction", ea.GetNumOuterElectrons(), (numOuterElectronA+1));
	TestEqInt("Check the second element's outer electron number after the reaction", eb.GetNumOuterElectrons(), (numOuterElectronB+1));
}

//! Supporting function for TestIonicBond
void __TestIonicBond(const char* cation, const char* anion, int numElectronsDonated, const char* message)
{
    // Load the elements
    // Note: Cation is the positively charged ion.
    Element c;
    Element a;
    TestMessage(message);
    TestEqBool("Get the cation", Element::GetRawElement(cation, &c), true);
    TestEqBool("Get the anion", Element::GetRawElement(anion, &a), true);

    // Create a reaction and process it
    Reaction reaction;
    reaction.Add(&c);
    c.AddBond(BondSide_Right, &a);
    TestEqBool("Check that a reaction occurs", reaction.Process(), true);

    // Verify the result
    TestEqInt("Check the cation's charge after the reaction", c.GetCharge(), numElectronsDonated);
    TestEqInt("Check the anion's charge after the reaction", a.GetCharge(), -numElectronsDonated);
}

//! Supporting function for TestTripleBond
//! a is the central element.
//TODO: It seems like it'd make more sense to make b the central element.
void __TestTripleBond(const char* a, const char* b, const char* c, BondType expectedBondType, const char* message)
{
    // Load the elements
    Element ea;
    Element eb;
    Element ec;
    TestMessage(message);
    TestEqBool("Get the first element", Element::GetRawElement(a, &ea), true);
    TestEqBool("Get the second element", Element::GetRawElement(b, &eb), true);
    TestEqBool("Get the third element", Element::GetRawElement(c, &ec), true);

    // Create a reaction and process it
    Reaction reaction;
    reaction.Add(&ea);
    ea.AddBond(BondSide_Left, &eb);
    ea.AddBond(BondSide_Right, &ec);
    TestEqBool("Check that a reaction occurs", reaction.Process(), true);

    // Verify the result
    // Element array used so we can keep track of which element is at what position.  
    // currently we only have 3 element bonding if
	// there is an alkali earth metal.  Mark the position of the alkali earth metal
    Element elementArray[3] = { ea, eb, ec };
    int alkaliEarthPosition;
    if (ea.GetGroup() == ALKALIEARTH)
    {
        alkaliEarthPosition = 0;
    }
    else if (eb.GetGroup() == ALKALIEARTH)
    {
        alkaliEarthPosition = 1;
    }
    else if (ec.GetGroup() == ALKALIEARTH)
    {
        alkaliEarthPosition = 2;
    }
    //test Ionic Bond 
    if (expectedBondType == BondType_Ionic)
    {
        //Todo: test bond type. 
        for (int i = 0; i < 3; i++)
        {
            if (i == alkaliEarthPosition)
                TestEqInt("Check the cation's charge after the reaction", elementArray[i].GetCharge(), 2);
            else
                TestEqInt("Check the anion's charge after the reaction", elementArray[i].GetCharge(), -1);
        }
    }
    //test Covalent Bond
    else if (expectedBondType == BondType_Covalent)
    {
        //Todo: test bond type. 
        for (int i = 0; i < 3; i++)
        {
            if (i == alkaliEarthPosition)
                TestEqInt("Check the element's shared electron number after the reaction", elementArray[i].GetSharedElectrons(), 4);
            else
                TestEqInt("Check the element's shared electron number after the reaction", elementArray[i].GetSharedElectrons(), 2);
        }
    }
    else
    {
        TestForceFail("Test called TestTripleBond with an unexpected expectedBondType!");
    }
}

//! Supporting macro for TestCovalentBond
#define _TestCovalentBond(a, b, numElectronsShared) __TestCovalentBond(a, b, numElectronsShared, "React " a " and " b " with expected electrons shared count of " #numElectronsShared)
//! Supporting macro for TestIonicBond
#define _TestIonicBond(cation, anion, numElectronsDonated) __TestIonicBond(cation, anion, numElectronsDonated, "React " cation " and " anion " with expected charge of " #numElectronsDonated)
//! Supporting macro for TestTripleBond
#define _TestTripleBond(a, b, c, expectedBond) __TestTripleBond(a, b, c, expectedBond, "React " a " and " b " and " c " with expected bond of " #expectedBond)

//! Tests that two elements covalently bond as expected.
//! This is a supporting macro that ensures bonds are commutative.
#define TestCovalentBond(a, b, numElectronsShared) _TestCovalentBond(a, b, numElectronsShared); _TestCovalentBond(b, a, numElectronsShared)

//! Tests that two elements ionically bond as expected.
//! This is a supporting macro that ensures opposite bonds are mirrored.
#define TestIonicBond(cation, anion, numElectronsDonated) _TestIonicBond(cation, anion, numElectronsDonated); _TestIonicBond(anion, cation, -numElectronsDonated)

//! Tests that 3 elements bond as expected.
//! This is a supporting macro that ensures all bonds are covered.
#define TestTripleBond(a, b, c, expectedBondType) _TestTripleBond(a, b, c, expectedBondType); _TestTripleBond(a, c, b, expectedBondType)

void TestStep_Bonds()
{
    TestStart();
	TestMessage("Test 2 elements covalent bonds");
    TestCovalentBond("H", "H", 2);
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
    TestCovalentBond("H", "At", 2);
    TestEnd();
  
    TestStart();
    TestMessage("Test 2 elements ionic compounds");
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
    TestEnd();

    TestStart(); 
    TestMessage("Test 3 elements bonds");
    TestTripleBond("Be", "H", "H", BondType_Covalent);
    TestTripleBond("Mg", "H", "H", BondType_Covalent);
    TestTripleBond("Ca", "H", "H", BondType_Ionic);
    TestTripleBond("Sr", "H", "H", BondType_Ionic);
    TestTripleBond("Ba", "H", "H", BondType_Ionic);
    TestTripleBond("Be", "F", "F", BondType_Covalent);
    TestTripleBond("Be", "Cl", "Cl", BondType_Covalent);
    TestTripleBond("Be", "Br", "Br", BondType_Covalent);
    TestTripleBond("Be", "I", "I", BondType_Covalent);
    TestTripleBond("Be", "At", "At", BondType_Covalent);
    TestEnd();
#if 0
    TestTripleBond("Mg", "F", "F", BondType_Ionic);
    TestTripleBond("Mg", "Cl", "Cl", BondType_Ionic);
    TestTripleBond("Mg", "Br", "Br", BondType_Ionic);
    TestTripleBond("Mg", "I", "I", BondType_Ionic);
    TestTripleBond("Mg", "At", "At", BondType_Ionic);
    TestTripleBond("Ca", "F", "F", BondType_Ionic);
    TestTripleBond("Ca", "Cl", "Cl", BondType_Ionic);
    TestTripleBond("Ca", "Br", "Br", BondType_Ionic);
    TestTripleBond("Ca", "I", "I", BondType_Ionic);
    TestTripleBond("Ca", "At", "At", BondType_Ionic);
    TestTripleBond("Sr", "F", "F", BondType_Ionic);
    TestTripleBond("Sr", "Cl", "Cl", BondType_Ionic);
    TestTripleBond("Sr", "Br", "Br", BondType_Ionic);
    TestTripleBond("Sr", "I", "I", BondType_Ionic);
    TestTripleBond("Sr", "At", "At", BondType_Ionic);
    TestTripleBond("Ba", "F", "F", BondType_Ionic);
    TestTripleBond("Ba", "Cl", "Cl", BondType_Ionic);
    TestTripleBond("Ba", "Br", "Br", BondType_Ionic);
    TestTripleBond("Ba", "I", "I", BondType_Ionic);
    TestTripleBond("Ba", "At", "At", BondType_Ionic);
#endif  
}
