#include "Test.h"
#include "periodic.h"
#include <sifteo.h>

//! Internal state to check if test has entered a failure state yet.
//! Should only ever be set to true by testing functions, never false.
static bool testIsFailing = false;

//! Prefix used for messages printed by the testing framework.
#define TEST_PREFIX "TEST: "

//! Prints a formatted test failure message to the test reviwer.
//! dataFormat must be a pure string as it is used for compile-timer concatenation.
#define PrintTestFailure(comparisonType, dataFormat) \
    LOG(TEST_PREFIX "VERIFICATION FAILURE ON %s:%d\n", file, line); \
    LOG(TEST_PREFIX "\tWhile checking condition: '%s'\n", message); \
    LOG(TEST_PREFIX "\tComparison: " comparisonType "\n"); \
    LOG(TEST_PREFIX "\tActual: " dataFormat "\n", actual); \
    LOG(TEST_PREFIX "\tExpected: " dataFormat "\n", expected)

void __TestMessage(const char* message, const char* file, unsigned int line)
{
    LOG(TEST_PREFIX "%s\n", message);
}

void __TestForceFail(const char* message, const char* file, unsigned int line)
{
    LOG(TEST_PREFIX "TEST FAILURE FORCED ON %s:%d\n", file, line);
    testIsFailing = true;
}

void __TestEqUint(const char* message, const char* file, unsigned int line, unsigned int actual, unsigned int expected)
{
    if (actual != expected)
    {
        PrintTestFailure("EQ", "%d");
        testIsFailing = true;
    }
}

void __TestEqInt(const char* message, const char* file, unsigned int line, int actual, int expected)
{
    if (actual != expected)
    {
        PrintTestFailure("EQ", "%d");
        testIsFailing = true;
    }
}

void __TestEqBool(const char* message, const char* file, unsigned int line, bool actual, bool expected)
{
    if (actual != expected)
    {
        PrintTestFailure("EQ", "%d");
        testIsFailing = true;
    }
}

void __TestEqString(const char* message, const char* file, unsigned int line, const char* actual, const char* expected)
{
    if (strcmp(actual, expected) != 0)
    {
        PrintTestFailure("EQ", "%s");
        testIsFailing = true;
    }
}

void __TestNePointer(const char* message, const char* file, unsigned int line, void* actual, void* expected)
{
    // We test using pointer difference to ensure that the test output doesn't change just because the pointer offset changed.
    unsigned char* pointerDifference = (unsigned char*)((unsigned char*)actual - (unsigned char*)expected);

    if (pointerDifference == 0)
    {
        PrintTestFailure("NE", "0x%X");
        testIsFailing = true;
    }
}

bool TestIsFailing()
{
    return testIsFailing;
}
