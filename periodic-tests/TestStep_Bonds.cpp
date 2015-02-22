#include "TestSteps.h"
#include "Test.h"
#include "periodic.h"
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

//!Supporting function for TestTripleBond
void _TestTripleBond(const char* a, const char* b, const char* c, const char* expectedBond, const char* message)
{
    Element ea;
    Element eb;
    Element ec;
    TestMessage(message);
    TestEqBool("Get the first element", Element::GetRawElement(a, &ea), true);
    TestEqBool("Get the second element", Element::GetRawElement(b, &eb), true);
    TestEqBool("Get the third element", Element::GetRawElement(c, &ec), true);

    TestEqBool("Check that a reaction occurs", ea.ReactWith(&eb, &ec), true);
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
    if (strcmp(expectedBond, "IONIC") == 0)
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
    else if (strcmp(expectedBond, "COVALENT") == 0)
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
}

//! Supporting macro for TestCovalentBond
#define _TestCovalentBond(a, b, numElectronsShared) __TestCovalentBond(a, b, numElectronsShared, "React " a " and " b " with expected electrons shared count of " #numElectronsShared)
//! Supporting macro for TestIonicBond
#define _TestIonicBond(cation, anion, numElectronsDonated) __TestIonicBond(cation, anion, numElectronsDonated, "React " cation " and " anion " with expected charge of " #numElectronsDonated)
//! Supporting macro for TestTripleBond
#define _TestTripleBond(a, b, c, expectedBond) _TestTripleBond(a, b, c, expectedBond, "React " a " and " b " and " c " with expected bond of " expectedBond)

//! Tests that two elements covalently bond as expected.
//! This is a supporting macro that ensures bonds are commutative.
#define TestCovalentBond(a, b, numElectronsShared) _TestCovalentBond(a, b, numElectronsShared); _TestCovalentBond(b, a, numElectronsShared)

//! Tests that two elements ionically bond as expected.
//! This is a supporting macro that ensures opposite bonds are mirrored.
#define TestIonicBond(cation, anion, numElectronsDonated) _TestIonicBond(cation, anion, numElectronsDonated); _TestIonicBond(anion, cation, -numElectronsDonated)

//Todo:
//! Tests that 3 elements bond as expected.
//! This is a supporting macro that ensures all bonds are covered.


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
    _TestTripleBond("Be", "H", "H", "COVALENT");
    _TestTripleBond("Mg", "H", "H", "COVALENT");
    _TestTripleBond("Ca", "H", "H", "IONIC");
    _TestTripleBond("Sr", "H", "H", "IONIC");
    _TestTripleBond("Ba", "H", "H", "IONIC");
    _TestTripleBond("Be", "F", "F", "COVALENT");
    _TestTripleBond("Be", "Cl", "Cl", "COVALENT");
    _TestTripleBond("Be", "Br", "Br", "COVALENT");
    _TestTripleBond("Be", "I", "I", "COVALENT");
    _TestTripleBond("Be", "At", "At", "COVALENT");
    TestEnd();
#if 0
    _TestTripleBond("Mg", "F", "F", "IONIC");
    _TestTripleBond("Mg", "Cl", "Cl", "IONIC");
    _TestTripleBond("Mg", "Br", "Br", "IONIC");
    _TestTripleBond("Mg", "I", "I", "IONIC");
    _TestTripleBond("Mg", "At", "At", "IONIC");
    _TestTripleBond("Ca", "F", "F", "IONIC");
    _TestTripleBond("Ca", "Cl", "Cl", "IONIC");
    _TestTripleBond("Ca", "Br", "Br", "IONIC");
    _TestTripleBond("Ca", "I", "I", "IONIC");
    _TestTripleBond("Ca", "At", "At", "IONIC");
    _TestTripleBond("Sr", "F", "F", "IONIC");
    _TestTripleBond("Sr", "Cl", "Cl", "IONIC");
    _TestTripleBond("Sr", "Br", "Br", "IONIC");
    _TestTripleBond("Sr", "I", "I", "IONIC");
    _TestTripleBond("Sr", "At", "At", "IONIC");
    _TestTripleBond("Ba", "F", "F", "IONIC");
    _TestTripleBond("Ba", "Cl", "Cl", "IONIC");
    _TestTripleBond("Ba", "Br", "Br", "IONIC");
    _TestTripleBond("Ba", "I", "I", "IONIC");
    _TestTripleBond("Ba", "At", "At", "IONIC");
#endif  
}
