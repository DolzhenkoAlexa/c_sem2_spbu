#pragma once

#include <stdbool.h>

typedef enum StateType {
    START, // начало
    SIGN, // знак + или -
    INT, // целая часть числа
    DOT, // точка после целой части
    FRAC, // дробная часть после точки
    DotNotInInt, // точка без целой части
    FracNotInInt, // дробная часть без целой части
    EXP, // экспонента
    ExpSign, // знак экспоненты
    NumExp // цифры в экспоненте
} StateType;

typedef enum DfaStatus {
    DfaOk,
    DfaInvalid
} DfaStatus;

bool isNumber(const char* input, DfaStatus* status
