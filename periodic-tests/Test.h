#ifndef __TEST_H__
#define __TEST_H__

void __TestMessage(const char* message, const char* file, unsigned int line);
void __TestForceFail(const char* message, const char* file, unsigned int line);

void __TestEqUint(const char* message, const char* file, unsigned int line, unsigned int actual, unsigned int expected);
void __TestEqInt(const char* message, const char* file, unsigned int line, int actual, int expected);
void __TestEqBool(const char* message, const char* file, unsigned int line, bool actual, bool expected);
void __TestEqString(const char* message, const char* file, unsigned int line, const char* actual, const char* expected);

bool TestIsFailing();

#define TestMessage(message) __TestMessage(message, __FILE__, __LINE__)
#define TestForceFail(message) __TestForceFail(message, __FILE__, __LINE__)

#define TestEqUint(message, actual, expected) __TestEqUint(message, __FILE__, __LINE__, actual, expected)
#define TestEqInt(message, actual, expected) __TestEqInt(message, __FILE__, __LINE__, actual, expected)
#define TestEqBool(message, actual, expected) __TestEqBool(message, __FILE__, __LINE__, actual, expected)
#define TestEqString(message, actual, expected) __TestEqString(message, __FILE__, __LINE__, actual, expected)

#endif
