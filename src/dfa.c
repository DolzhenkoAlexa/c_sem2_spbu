#include "dfa.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

struct DFA {
    Transition* transitions;
    int transCount;
    int* accepting;
    int accCount;
    int startState;
};

static char classifyChar(char c, DfaStatus* status)
{
    if (isdigit((unsigned char)c))
        return 'd';
    if (c == '+' || c == '-')
        return 's';
    if (c == '.')
        return '.';
    if (c == 'e' || c == 'E')
        return 'e';

    if (status)
        *status = DfaInvalidSymbol;
    return 0;
}

DFA* makeDfa(Transition* transitions, int transCount,
    int* accepting, int accCount, int startState)
{
    DFA* dfa = (DFA*)calloc(1, sizeof(DFA));
    if (!dfa)
        return NULL;

    dfa->transitions = transitions;
    dfa->transCount = transCount;
    dfa->accepting = accepting;
    dfa->accCount = accCount;
    dfa->startState = startState;

    return dfa;
}

void freeDfa(DFA* dfa)
{
    free(dfa);
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
        char c = classifyChar(input[i], status);

        // Если символ не из алфавита
        if (c == 0) {
            return false;
        }

        int next = -1;
        for (int j = 0; j < dfa->transCount; j++) {
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
            if (status)
                *status = DfaOk;
            return true;
        }
    }
    if (status)
        *status = DfaNoTransition;
    return false;
}