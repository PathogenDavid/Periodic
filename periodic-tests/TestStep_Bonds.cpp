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
//! b is the central element.
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
    reaction.Add(&eb);
    eb.AddBond(BondSide_Left, &ea);
    eb.AddBond(BondSide_Right, &ec);
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
                TestEqInt("Check the element's shared electron number after the reaction", elementArray[i].GetSharedElectrons(), 2);
            else
                TestEqInt("Check the element's shared electron number after the reaction", elementArray[i].GetSharedElectrons(), 1);
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
#define TestTripleBond(a, b, c, expectedBondType) _TestTripleBond(a, b, c, expectedBondType); _TestTripleBond(c, b, a, expectedBondType)

void TestStep_Bonds()
{
    TestStart();
	TestMessage("Test 2 elements covalent bonds");
    TestCovalentBond("H", "H", 1);
    TestCovalentBond("F", "F", 1);
    TestCovalentBond("Cl", "Cl", 1);
    TestCovalentBond("Br", "Br", 1);
    TestCovalentBond("I", "I", 1);
    TestCovalentBond("F", "Cl", 1);
    TestCovalentBond("F", "Br", 1);
    TestCovalentBond("F", "I", 1);
    TestCovalentBond("Cl", "Br", 1);
    TestCovalentBond("Cl", "I", 1);
    TestCovalentBond("H", "F", 1);
    TestCovalentBond("H", "Cl", 1);
    TestCovalentBond("H", "Br", 1); 
    TestCovalentBond("H", "I", 1);
    TestCovalentBond("H", "At", 1);
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
    TestTripleBond("H" , "Be", "H" , BondType_Covalent);
    TestTripleBond("H" , "Mg", "H" , BondType_Covalent);
    TestTripleBond("H" , "Ca", "H" , BondType_Ionic);
    TestTripleBond("H" , "Sr", "H" , BondType_Ionic);
    TestTripleBond("H" , "Ba", "H" , BondType_Ionic);
    TestTripleBond("F" , "Be", "F" , BondType_Covalent);
    TestTripleBond("Cl", "Be", "Cl", BondType_Covalent);
    TestTripleBond("Br", "Be", "Br", BondType_Covalent);
    TestTripleBond("I" , "Be", "I" , BondType_Covalent);
    TestTripleBond("At", "Be", "At", BondType_Covalent);

    TestTripleBond("F" , "Mg", "F" , BondType_Ionic);
    TestTripleBond("Cl", "Mg", "Cl", BondType_Ionic);
    TestTripleBond("Br", "Mg", "Br", BondType_Ionic);
    TestTripleBond("I" , "Mg", "I" , BondType_Ionic);
    TestTripleBond("At", "Mg", "At", BondType_Ionic);
    TestTripleBond("F" , "Ca", "F" , BondType_Ionic);
    TestTripleBond("Cl", "Ca", "Cl", BondType_Ionic);
    TestTripleBond("Br", "Ca", "Br", BondType_Ionic);
    TestTripleBond("I" , "Ca", "I" , BondType_Ionic);
    TestTripleBond("At", "Ca", "At", BondType_Ionic);
    TestTripleBond("F" , "Sr", "F" , BondType_Ionic);
    TestTripleBond("Cl", "Sr", "Cl", BondType_Ionic);
    TestTripleBond("Br", "Sr", "Br", BondType_Ionic);
    TestTripleBond("I" , "Sr", "I" , BondType_Ionic);
    TestTripleBond("At", "Sr", "At", BondType_Ionic);
    TestTripleBond("F" , "Ba", "F" , BondType_Ionic);
    TestTripleBond("Cl", "Ba", "Cl", BondType_Ionic);
    TestTripleBond("Br", "Ba", "Br", BondType_Ionic);
    TestTripleBond("I" , "Ba", "I" , BondType_Ionic);
    TestTripleBond("At", "Ba", "At", BondType_Ionic);
    TestEnd();
#if 0  
#endif  
}
