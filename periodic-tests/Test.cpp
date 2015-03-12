#include "Test.h"
#include "periodic.h"
#include <sifteo.h>

//! Internal state to check if test has entered a failure state yet.
//! Should only ever be set to true by testing functions, never false.
static bool testIsFailing = false;

#define TEST_STEP_COUNT 6

static int numVerifications;
static int numVerificationsFailing;
static int numVerificationsRecord[TEST_STEP_COUNT][2];
static int passTestStep[TEST_STEP_COUNT]; // Since this is static, it will be all 0 by default.
static char testSteps[TEST_STEP_COUNT][40] = // TODO: Make it so all this doesn't have to be static.
{
    "TestStep_Strcmp:                 ",
    "TestStep_ElementBasic:           ",
    "TestStep_2ElementsCovalentBonds: ",
    "TestStep_2ElementsInoicBonds:    ",
    "TestStep_3ElementsBonds:         ",
    "TestStep_ObjectPool:             "
};

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

void __TestNePointer(const char* message, const char* file, unsigned int line, void* actual, void* expected)
{
    // We test using pointer difference to ensure that the test output doesn't change just because the pointer offset changed.
    unsigned char* pointerDifference = (unsigned char*)((unsigned char*)actual - (unsigned char*)expected);
    numVerifications++;

    if (pointerDifference == 0)
    {
        PrintTestFailure("NE", "0x%X");
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
    { passTestStep[i] = 0; }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
        { numVerificationsRecord[i][j] = 0; }
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
    for (int i = 0; i < TEST_STEP_COUNT; i++)
    {
        if (passTestStep[i] == 0)
        {
            if (numVerificationsFailing == 0)
            { passTestStep[i] = 1; }
            else
            { passTestStep[i] = -1; }
            break;
        }
    }
    for (int i = 0; i < TEST_STEP_COUNT; i++)
    {
        if (numVerificationsRecord[i][0] == 0)
        {
            numVerificationsRecord[i][0] = numVerifications - numVerificationsFailing;
            numVerificationsRecord[i][1] = numVerifications;
            break;
        }
    }
}

const char* pass = "Pass!";
const char* fail = "Fail!";
void TestResultPrint()
{
    TestMessage("----------------Test results-----------------");
    int totalPass = 0;
    int totalDone = 0;
    for (int i = 0; i < TEST_STEP_COUNT; i++)
    {
        LOG(TEST_PREFIX "%s%s ", testSteps[i], passTestStep[i] > 0 ? pass : fail);
        LOG("%d/%d verifications passed\n", numVerificationsRecord[i][0], numVerificationsRecord[i][1]);
        totalPass += numVerificationsRecord[i][0];
        totalDone += numVerificationsRecord[i][1];
    }

    LOG(TEST_PREFIX "Total:                           %s %d/%d verifications passed\n", testIsFailing ? fail : pass, totalPass, totalDone);
}

