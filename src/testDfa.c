#include "dfa.h"
#include <stdio.h>

int compareTest(const char* input, int expected)
{
    DfaStatus status;
    int result = isNumber(input, &status);

    if (result == expected) {
        return 1;
    }
    return 0;
}

int test1()
{
    printf("Test 1: Integer number\n");
    int result = compareTest("123", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test2()
{
    printf("Test 2: Negative number\n");
    int result = compareTest("-456", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test3()
{
    printf("Test 3: Float with exponent\n");
    int result = compareTest("38.871E5", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test4()
{
    printf("Test 4: Invalid double dot\n");
    int result = compareTest("823.16.10", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test5()
{
    printf("Test 5: Not a number text\n");
    int result = compareTest("А я число?", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test6()
{
    printf("Test 6: Empty string\n");
    int result = compareTest("", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test7()
{
    printf("Test 7: Just dot\n");
    int result = compareTest(".", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test8()
{
    printf("Test 8: Exponent without number\n");
    int result = compareTest("e10", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test9()
{
    printf("Test 9: Plus sign only\n");
    int result = compareTest("+", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int main()
{
    printf("Running tests:\n");
    int total = 0;

    total += test1();
    total += test2();
    total += test3();
    total += test4();
    total += test5();
    total += test6();
    total += test7();
    total += test8();
    total += test9();
    printf("Result: %d out of 9 tests passed\n", total);

    return 0;
}
