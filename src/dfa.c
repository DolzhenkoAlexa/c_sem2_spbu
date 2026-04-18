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
                state = DotNotInInt;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case SIGN:
            if (isDigit(character)) {
                state = INT;
            } else if (character == '.') {
                state = DotNotInInt;
            } else {
                *status = DfaInvalid;
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
                *status = DfaInvalid;
                return false;
            }
            break;

        case DOT:
            if (isDigit(character)) {
                state = FRAC;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case FRAC:
            if (isDigit(character)) {
                state = FRAC;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case DotNotInInt:
            if (isDigit(character)) {
                state = FracNotInInt;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case FracNotInInt:
            if (isDigit(character)) {
                state = FracNotInInt;
            } else if (character == 'e' || character == 'E') {
                state = EXP;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case EXP:
            if (character == '+' || character == '-') {
                state = ExpSign;
            } else if (isDigit(character)) {
                state = NumExp;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case ExpSign:
            if (isDigit(character)) {
                state = NumExp;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case NumExp:
            if (isDigit(character)) {
                state = NumExp;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;
        }
    }

    *status = DfaOk;

    return (state == INT || state == DOT || state == FRAC || state == FracNotInInt || state == NumExp);
}
