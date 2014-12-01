#include "Test.h"
#include "TestSteps.h"
#include "periodic.h"

//! Macro for supporting the Strcmp test step. Prints a user-friendly message to the test reviewer.
#define TEST_STRCMP(a, b, expected) TestEqInt("Check that strcmp(" a ", " b ") == " #expected, strcmp(a, b), expected)

void TestStep_Strcmp()
{
    //Note: The C standard does not require -1 or 1, only negative or positive, but our implementation will only return 0, -1, and 1.

    // Basic operation
    TEST_STRCMP("AAA", "BBB", -1);
    TEST_STRCMP("BBB", "AAA", 1);
    TEST_STRCMP("AAA", "AAA", 0);

    // Mostly matching shorter/longer strings
    TEST_STRCMP("AA", "AAA", -1);
    TEST_STRCMP("AAA", "AA", 1);

    // Mismatched shorter/longer strings
    TEST_STRCMP("BB", "AAA", 1);
    TEST_STRCMP("AAA", "BB", -1);
}
