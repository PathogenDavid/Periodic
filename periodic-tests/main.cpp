#include "assets.gen.h"
#include "Test.h"
#include "TestSteps.h"
#include "Element.h"
#include <sifteo.h>

using namespace Sifteo;

//! Sifteo framework metadata
static Metadata M = Metadata()
    .title("Periodic Project Unit Tests")
    .package("edu.ksu.periodic.tests", "0.1")
    .icon(Icon)
    .cubeRange(1)
;

//! Runs the given test step, will print a message to the test reviewer to verify the test was run.
#define RUN_TEST(x) \
    TestMessage("Running test step " #x "..."); x()

//! Global test entry point, runs all tests steps and prints overall test status to the test reviewer.
void main()
{
    TestInit();
    TestStart();
    RUN_TEST(TestStep_Strcmp);
    TestEnd();
    TestStart();
    RUN_TEST(TestStep_ElementBasic);
    TestEnd();
    RUN_TEST(TestStep_Bonds);
    RUN_TEST(TestStep_ObjectPool);

    TestResultPrint();
    if (TestIsFailing())
    {
        TestMessage("!!!TEST FAILURE!!!");
    }
    else
    {
        TestMessage("!!!TEST PASSED!!!");
    }
}
