#include "Test.h"
#include "periodic.h"
#include <sifteo.h>

static bool testIsFailing = false;

#define TEST_PREFIX "TEST: "

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
    if (strcmp(actual, expected) == 0)
    {
        PrintTestFailure("EQ", "%s");
        testIsFailing = true;
    }
}

bool TestIsFailing()
{
    return testIsFailing;
}
