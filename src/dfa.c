#include "dfa.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct DFA {
    Transition* transitions;
    int transitionCount;
    int* accepting;
    int accCount;
    int startState;
};

static CharClass classifyChar(char c, DfaStatus* status)
{
    if (isdigit((unsigned char)c))
        return Digit;
    if (c == '+' || c == '-')
        return Sign;
    if (c == '.')
        return Dot;
    if (c == 'e' || c == 'E')
        return Exponent;

    if (status)
        *status = DfaInvalidSymbol;
    return Invalid;
}

DFA* makeDfa(Transition* transitions, int transitionCount,
    int* accepting, int accCount, int startState)
{
    DFA* dfa = (DFA*)calloc(1, sizeof(DFA));
    if (!dfa)
        return NULL;

    dfa->transitions = (Transition*)malloc(transitionCount * sizeof(Transition));
    if (!dfa->transitions) {
        free(dfa);
        return NULL;
    }
    // Копирование для безопасности
    memcpy(dfa->transitions, transitions, transitionCount * sizeof(Transition));
    dfa->transitionCount = transitionCount;

    dfa->accepting = (int*)malloc(accCount * sizeof(int));
    if (!dfa->accepting) {
        free(dfa->transitions);
        free(dfa);
        return NULL;
    }
    memcpy(dfa->accepting, accepting, accCount * sizeof(int));
    dfa->accCount = accCount;

    dfa->startState = startState;

    return dfa;
}

void freeDfa(DFA* dfa)
{
    if (dfa) {
        free(dfa->transitions);
        free(dfa->accepting);
        free(dfa);
    }
}

bool checkDfa(DFA* dfa, const char* input, DfaStatus* status)
{
    if (status)
        *status = DfaOk;

    if (input == NULL || input[0] == '\0') {
        if (status)
            *status = DfaEmptyInput;
        return false;
    }

    int state = dfa->startState;

    for (int i = 0; input[i] != '\0'; i++) {
        CharClass c = classifyChar(input[i], status);

        // Если символ не из алфавита
        if (c == Invalid) {
            return false;
        }

        int next = -1;
        for (int j = 0; j < dfa->transitionCount; j++) {
            if (dfa->transitions[j].from == state && dfa->transitions[j].symbol == c) {
                next = dfa->transitions[j].to;
                break;
            }
        }
        // Если нет перехода
        if (next == -1) {
            if (status)
                *status = DfaNoTransition;
            return false;
        }

        state = next;
    }

    for (int i = 0; i < dfa->accCount; i++) {
        if (dfa->accepting[i] == state) {
            return true;
        }
    }

    if (status)
        *status = DfaNoTransition;
    return false;
}
