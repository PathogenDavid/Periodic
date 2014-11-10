#include "TestSteps.h"
#include "Test.h"
#include "Element.h"

void TestStep_ElementBasic()
{
    Element e;

    TestMessage("Test GetRawElement with a symbol name.");
    Element::GetRawElement("Kr", &e);
    TestEqString("Check if Krypton is returned for symbol 'Kr'", e.GetName(), "Krypton");
}
