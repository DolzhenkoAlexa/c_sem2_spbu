#include "dfa.h"
#include <stdbool.h>

static bool isDigit(char character)
{
    return character >= '0' && character <= '9';
}

bool isNumber(const char* input, DfaStatus* status)
{
    StateType state = Start;

    for (int i = 0; input[i] != '\0'; i++) {
        char character = input[i];

        switch (state) {

        case Start:
            if (character == '+' || character == '-') {
                state = Sign;
            } else if (isDigit(character)) {
                state = Int;
            } else if (character == '.') {
                state = DotNotInInt;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case Sign:
            if (isDigit(character)) {
                state = Int;
            } else if (character == '.') {
                state = DotNotInInt;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case Int:
            if (isDigit(character)) {
                state = Int;
            } else if (character == '.') {
                state = Dot;
            } else if (character == 'e' || character == 'E') {
                state = Exp;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case Dot:
        case Frac:
            if (isDigit(character)) {
                state = Frac;
            } else if (character == 'e' || character == 'E') {
                state = Exp;
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
                state = Exp;
            } else {
                *status = DfaInvalid;
                return false;
            }
            break;

        case Exp:
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
