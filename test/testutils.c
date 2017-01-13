#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "testutils.h"

TestResult __test_check(int cond, TestResult passType, TestResult failType, const char* file, int line, const char* fmt, ...) {
	//TODO finish
}

int main(int argc, char* argv[]) {
	unsigned int testsRun = 0;
	unsigned int testsPassed = 0;
	unsigned int testsFailed = 0;
	unsigned int testsTimedOut = 0;
	unsigned int testsSkipped = 0;
	int returnCode = 0;
	for (unsigned int i = 0; i < sizeof(TEST_LIST) / sizeof(TEST_LIST[0]); i++) {
		const char* testName = TEST_LIST[i].name;
		TestResult (*test) () = TEST_LIST[i].test;
		int childPID = fork();
		if (childPID == 0) {
			//Is child; run test and exit
			TestResult result = test();
			exit(result);
		} else if (childPID > 0) {
			//Is parent; wait for child result
			int status;
			int r = waitpid(childPID, &status, 0);
			if (r <= 0) {
				printf("Error %d when waiting for test %s (PID %d)\n", r, testName, childPID);
				testsFailed++;
				break;
			}
			if (!WIFEXITED(status)) {
				fprintf(stderr, "Test %s (PID %d) terminated abnormall: status %d\n", testName, childPID, status);
				testsFailed++;
				//TODO is this recoverable from?
				continue;
			}
			switch (WEXITSTATUS(status)) {
				case TestResult_PASS:
					printf("PASS %s\n", testName);
					testsPassed++;
					break;
				case TestResult_FAIL:
					printf("FAIL %s\n", testName);
					testsFailed++;
					break;
				case TestResult_TIMEOUT:
					printf("TIMEOUT %s\n", testName);
					testsTimedOut++;
					break'
				case TestResult_INVALID:
					printf("INVALID %s\n", testName);
					testsSkipped++;
					break;
				default:
					printf("UNKNOWN %s (%d)\n", testName, WEXITSTATUS(status));
					testsFailed++;
					break;
			}
		} else {
			//Error forking
			fprintf(stderr, "Error %d when forking child process for test %s\n", childPID, testName);
			testsFailed++;
			break;//Exit main loop
		}
	}
	printf("Tests run: %u\n", testsRun);
	printf("Passed:    %u\n", testsPassed);
	printf("Failed:    %u\n", testsFailed);
	printf("Timed out: %u\n", testsTimedOut);
	printf("Skipped:   %u\n", testsSkipped);
	return testsFailed > 0;
}