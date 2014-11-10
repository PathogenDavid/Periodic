#include "Test.h"
#include "TestSteps.h"
#include "periodic.h"

#define TEST_STRCMP(a, b, expected) TestEqInt("Check that strcmp(" a ", " b ") == " #expected, strcmp(a, b), expected)

void TestStep_Strcmp()
{
    TEST_STRCMP("AAA", "BBB", -1);
    TEST_STRCMP("BBB", "AAA", 1);
    TEST_STRCMP("AAA", "AAA", 0);
    TEST_STRCMP("AA", "AAA", -1);
    TEST_STRCMP("AAA", "AA", 1);
    TEST_STRCMP("BB", "AAA", 1);
    TEST_STRCMP("AAA", "BB", -1);
}
