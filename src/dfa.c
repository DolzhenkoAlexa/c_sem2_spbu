#include "dfa.h"
#include <stdbool.h>

static bool isDigit(char character)
{
    return character >= '0' && character <= '9';
}

bool isNumber(const char* input, DfaStatus* status)
{
    StateType state = START;

    for (int i = 0; input[i] != '\0'; i++) {
        char character = input[i];

        switch (state) {

        case START:
            if (character == '+' || character == '-') {
                state = SIGN;
            } else if (isDigit(character)) {
                state = INT;
            } else if (character == '.') {
                state = DOT_NOT_IN_INT;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case SIGN:
            if (isDigit(character)) {
                state = INT;
            } else if (character == '.') {
                state = DOT_NOT_IN_INT;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case INT:
            if (isDigit(character)) {
                state = INT;
            } else if (character == '.') {
                state = DOT;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case DOT:
            if (isDigit(character)) {
                state = FRAC;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case FRAC:
            if (isDigit(character)) {
                state = FRAC;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case DOT_NOT_IN_INT:
            if (isDigit(character)) {
                state = FRAC_NOT_IN_INT;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case FRAC_NOT_IN_INT:
            if (isDigit(character)) {
                state = FRAC_NOT_IN_INT;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case EXP:
            if (character == '+' || character == '-') {
                state = EXP_SIGN;
            } else if (isDigit(character)) {
                state = NUM_EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case EXP_SIGN:
            if (isDigit(character)) {
                state = NUM_EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;

        case NUM_EXP:
            if (isDigit(character)) {
                state = NUM_EXP;
            } else {
                *status = DFA_INVALID;
                return false;
            }
            break;
        }
    }

    *status = DFA_OK;

    return (state == INT ||
            state == DOT ||
            state == FRAC ||
            state == FRAC_NOT_IN_INT ||
            state == NUM_EXP);
}
