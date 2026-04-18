#pragma once

#include <stdbool.h>

typedef enum StateType {
    START, // начало
    SIGN, // знак + или -
    INT, // целая часть числа
    DOT, // точка после целой части
    FRAC, // дробная часть после точки
    DOT_NOT_IN_INT, // точка без целой части
    FRAC_NOT_IN_INT, // дробная часть без целой части
    EXP, // экспонента
    EXP_SIGN, // знак экспоненты
    NUM_EXP // цифры в экспоненте
} StateType;

typedef enum DfaStatus {
    DFA_OK,
    DFA_INVALID
} DfaStatus;

bool isNumber(const char* input, DfaStatus* status);
