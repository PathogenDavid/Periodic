#include "Test.h"
#include "periodic.h"
#include <sifteo.h>

//! Internal state to check if test has entered a failure state yet.
//! Should only ever be set to true by testing functions, never false.
static bool testIsFailing = false;

static int numVerifications;
static int numVerificationsFailing;

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
    numVerifications++;
    if (actual != expected)
    {
        PrintTestFailure("EQ", "%d");
        testIsFailing = true;
        numVerificationsFailing++;
    }
}

void __TestEqInt(const char* message, const char* file, unsigned int line, int actual, int expected)
{
    numVerifications++;
    if (actual != expected)
    {
        PrintTestFailure("EQ", "%d");
        testIsFailing = true;
        numVerificationsFailing++;
    }
}

void __TestEqBool(const char* message, const char* file, unsigned int line, bool actual, bool expected)
{
    numVerifications++;
    if (actual != expected)
    {
        PrintTestFailure("EQ", "%d");
        testIsFailing = true;
        numVerificationsFailing++;
    }
}

void __TestEqString(const char* message, const char* file, unsigned int line, const char* actual, const char* expected)
{
    numVerifications++;
    if (strcmp(actual, expected) != 0)
    {
        PrintTestFailure("EQ", "%s");
        testIsFailing = true;
        numVerificationsFailing++;
    }
}

bool TestIsFailing()
{
    return testIsFailing;
}

void TestStart()
{
    numVerifications = 0;
    numVerificationsFailing = 0;
}

void TestEnd()
{
    LOG(TEST_PREFIX "%d/%d verifications passed\n", numVerifications - numVerificationsFailing, numVerifications);
}

//void TestCovalentEnd()
//{
//    //each TestCovalentBond includes 7 tests 
//    LOG(TEST_PREFIX "%d/%d verifications passed\n", (numVerifications - numVerificationsFailing) / 7, numVerifications / 7);
//}
//void TestIonicEnd()
//{
//    //each TestIonicBond includes 5 tests
//    LOG(TEST_PREFIX "%d/%d verifications passed\n", (numVerifications - numVerificationsFailing) / 5, numVerifications / 5);
//
//}
//void TestTripleEnd()
//{
//    //each TestTripleBond includes 5 tests
//    LOG(TEST_PREFIX "%d/%d verifications passed\n", (numVerifications - numVerificationsFailing) / 5, numVerifications / 5);
//}
