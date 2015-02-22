#include "Test.h"
#include "periodic.h"
#include <sifteo.h>

//! Internal state to check if test has entered a failure state yet.
//! Should only ever be set to true by testing functions, never false.
static bool testIsFailing = false;

static int numVerifications;
static int numVerificationsFailing;
static int numVerificationsRecord[5][2];
static int passTestStep[5] = {0,0,0,0,0};
static char testSteps[5][40] = {"TestStep_Strcmp:                 ",
                                "TestStep_ElementBasic:           ",
                                "TestStep_2ElementsCovalentBonds: ",
                                "TestStep_2ElementsInoictBonds:   ",
                                "TestStep_3ElementsBonds:         "};
const char* pass = "Pass!";
const char* fail = "Fail!";

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

void TestInit()
{
    for (int i = 0; i < 5; i++)
        passTestStep[i] = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
            numVerificationsRecord[i][j] = 0;
    }
}

void TestStart()
{
    numVerifications = 0;
    numVerificationsFailing = 0;
}

void TestEnd()
{
    //LOG(TEST_PREFIX "%d/%d verifications passed\n", numVerifications - numVerificationsFailing, numVerifications);
    for (int i = 0; i < 5; i++)
    {
        if (passTestStep[i] == 0)
        {
            if (numVerificationsFailing == 0)
                passTestStep[i] = 1; 
            else
                passTestStep[i] = -1; 
            break;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (numVerificationsRecord[i][0] == 0)
        {
            numVerificationsRecord[i][0] = numVerifications - numVerificationsFailing;
            numVerificationsRecord[i][1] = numVerifications;
            break;
        }
    }
}
void TestResultPrint()
{
    TestMessage("----------------Test results-----------------");
    for (int i = 0; i < 5; i++)
    {
        LOG(TEST_PREFIX "%s", testSteps[i]);
        if (passTestStep[i]>0)
            LOG("%s ", pass);
        else
            LOG("%s ", fail);
        LOG("%d/%d verifications passed\n", numVerificationsRecord[i][0], numVerificationsRecord[i][1]);
    }
}

