#ifndef __TEST_TESTUTILS_H
#define __TEST_TESTUTILS_H

//Timeout for tests, in ms
#define TEST_TIMEOUT 10000
typedef enum TestResult {
	/**
	 * Test was able to run, and passed
	 */
	TestResult_PASS = 0,
	/**
	 * Test was able to run, and failed
	 */
	TestResult_FAIL = 1,
	/**
	 * Test could not run for some reason.
	 */
	TestResult_INVALID = 2,
	/**
	 * Test did not return in set timeout (fail)
	 */
	TestResult_TIMEOUT = 3
} TestResult;

#define TEST(testname) TestResult testname() __attribute__((visibility ("default")))

#define ASSERT(pred, msg, ...) \
		do {\
			TestResult r = __test_check((pred), TestResult_PASS, TestResult_FAIL, __FILE__, __LINE__, msg, __VA_ARGS);\
			if (r != TestResult_PASS)return r;\
		} while(0);

#define FAIL(message, ...) do {return TestResult_FAIL;}

TestResult __test_check(int cond, TestResult passType, TestResult failType, const char* file, int line, const char* fmt, ...);

int main(int argc, char* argv[]);

extern const struct {
	const char* name,
	TestResult (*test) ();
} TEST_LIST[];

#endif