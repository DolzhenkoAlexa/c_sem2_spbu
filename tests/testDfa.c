#include "../src/dfa.h"
#include <stdio.h>

static int compareTest(Transition* transitions, int transitionCount,
                       int* accepting, int accCount,
                       const char* input, int expected)
{
    DFA* dfa = makeDfa(transitions, transitionCount, accepting, accCount, 0);
    if (!dfa) return 0;

    DfaStatus status;
    int result = checkDfa(dfa, input, &status);

    freeDfa(dfa);
    return result == expected;
}

// Простейший автомат, чтобы показать, что он тоже обрабатывается
int testSimple(void)
{
    printf("Test 1: Integer number\n");
    Transition transitions[] = {
        {0, Digit, 1},
        {1, Digit, 1}
    };
    int transitionCount = sizeof(transitions) / sizeof(transitions[0]);
    int accepting[] = {1};
    int accCount = sizeof(accepting) / sizeof(accepting[0]);

    int result = compareTest(transitions, transitionCount, accepting, accCount, "123", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

// Полный автомат для чисел
static Transition fullTransitions[] = {
    {0, Digit, 2}, {0, Dot, 5}, {0, Sign, 1},
    {1, Digit, 2}, {1, Dot, 5},
    {2, Digit, 2}, {2, Dot, 3}, {2, Exponent, 7},
    {3, Digit, 4},
    {4, Digit, 4}, {4, Exponent, 7},
    {5, Digit, 6},
    {6, Digit, 6}, {6, Exponent, 7},
    {7, Digit, 9}, {7, Sign, 8},
    {8, Digit, 9},
    {9, Digit, 9}
};
static int fullAccepting[] = {2, 3, 4, 6, 9};
static int fullTransitionCount = sizeof(fullTransitions) / sizeof(fullTransitions[0]);
static int fullAccCount = sizeof(fullAccepting) / sizeof(fullAccepting[0]);

int testInteger(void)
{
    printf("\n\nTests with full DFA:\n");
    printf("Test 2: Integer number\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "123", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testNegativeInteger(void)
{
    printf("Test 3: Negative integer\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "-456", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testFloat(void)
{
    printf("Test 4: Float number\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "3.14", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testFloatWithoutIntegerPart(void)
{
    printf("Test 5: Float without integer part\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, ".591", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testWithExponent(void)
{
    printf("Test 6: Number with exponent\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "5E10", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testNegativeExponent(void)
{
    printf("Test 7: Number with negative exponent\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "-3.14E-2", 1);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testEmptyString(void)
{
    printf("Test 8: Empty string\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testJustDot(void)
{
    printf("Test 9: Just dot\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, ".", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testDoubleDot(void)
{
    printf("Test 10: Double dot\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "123.45.67", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testExponentWithoutNumber(void)
{
    printf("Test 11: Exponent without number\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "123E", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int testLetterInsteadOfDigit(void)
{
    printf("Test 12: Some letters\n");
    int result = compareTest(fullTransitions, fullTransitionCount,
                             fullAccepting, fullAccCount, "abc", 0);
    printf(result ? "PASSED\n" : "FAILED\n");
    return result;
}

int main(void)
{
    printf("Running tests:\n");
    int total = 0;
    total += testSimple();
    total += testInteger();
    total += testNegativeInteger();
    total += testFloat();
    total += testFloatWithoutIntegerPart();
    total += testWithExponent();
    total += testNegativeExponent();
    total += testEmptyString();
    total += testJustDot();
    total += testDoubleDot();
    total += testExponentWithoutNumber();
    total += testLetterInsteadOfDigit();
    printf("Result: %d out of 12 tests passed\n", total);
    return 0;
}
