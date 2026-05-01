#include "../src/dfa.h"
#include <stdio.h>

static int compareTest(Transition* transitions, int transCount,
    int* accepting, int accCount,
    const char* input, int expected)
{
    DFA* dfa = makeDfa(transitions, transCount, accepting, accCount, 0);
    if (!dfa)
        return 0;

    DfaStatus status;
    int result = checkDfa(dfa, input, &status);

    freeDfa(dfa);
    return result == expected;
}

int test1(void)
{
    printf("Test 1: Integer number\n");

    Transition transitions[] = {
        { 0, 'd', 1 },
        { 1, 'd', 1 }
    };
    int transCount = sizeof(transitions) / sizeof(transitions[0]);
    int accepting[] = { 1 };
    int accCount = sizeof(accepting) / sizeof(accepting[0]);

    int result = compareTest(transitions, transCount, accepting, accCount, "123", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test2(void)
{
    printf("Test 2: Not a number (contains letter)\n");

    Transition transitions[] = {
        { 0, 'd', 1 },
        { 1, 'd', 1 }
    };
    int transCount = sizeof(transitions) / sizeof(transitions[0]);
    int accepting[] = { 1 };
    int accCount = sizeof(accepting) / sizeof(accepting[0]);

    int result = compareTest(transitions, transCount, accepting, accCount, "12a3", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int test3(void)
{
    printf("Test 3: Negative float with exponent\n");

    Transition transitions[] = {
        { 0, 'd', 2 }, { 0, '.', 5 }, { 0, 's', 1 },
        { 1, 'd', 2 }, { 1, '.', 5 },
        { 2, 'd', 2 }, { 2, '.', 3 }, { 2, 'e', 7 },
        { 3, 'd', 4 },
        { 4, 'd', 4 }, { 4, 'e', 7 },
        { 5, 'd', 6 },
        { 6, 'd', 6 }, { 6, 'e', 7 },
        { 7, 'd', 9 }, { 7, 's', 8 },
        { 8, 'd', 9 },
        { 9, 'd', 9 }
    };
    int transCount = sizeof(transitions) / sizeof(transitions[0]);
    int accepting[] = { 2, 3, 4, 6, 9 };
    int accCount = sizeof(accepting) / sizeof(accepting[0]);

    int result = compareTest(transitions, transCount, accepting, accCount, "-3.14E-2", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int main(void)
{
    printf("Running tests:\n");
    int total = 0;
    total += test1();
    total += test2();
    total += test3();
    printf("Result: %d out of 3 tests passed\n", total);
    return 0;
}