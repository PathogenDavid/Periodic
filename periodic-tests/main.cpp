#include "assets.gen.h"
#include "Test.h"
#include "TestSteps.h"
#include "Element.h"
#include <sifteo.h>

using namespace Sifteo;

static Metadata M = Metadata()
    .title("Periodic Project Unit Tests")
    .package("edu.ksu.periodic.tests", "0.1")
    .icon(Icon)
    .cubeRange(1)
;

#define RUN_TEST(x) \
    TestMessage("Running test step " #x "..."); x()

void main()
{
    RUN_TEST(TestStep_Strcmp);
    RUN_TEST(TestStep_ElementBasic);
    RUN_TEST(TestStep_Bonds);

    if (TestIsFailing())
    {
        TestMessage("!!!TEST FAILURE!!!");
    }
    else
    {
        TestMessage("!!!TEST PASSED!!!");
    }
}
