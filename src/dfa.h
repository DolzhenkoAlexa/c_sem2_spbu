#pragma once

#include <stdbool.h>

typedef enum StateType {
    Start, // начало
    Sign, // знак + или -
    Int, // целая часть числа
    Dot, // точка после целой части
    Frac, // дробная часть после точки
    DotNotInInt, // точка без целой части
    FracNotInInt, // дробная часть без целой части
    Exp, // экспонента
    ExpSign, // знак экспоненты
    NumExp // цифры в экспоненте
} StateType;

typedef enum DfaStatus {
    DfaOk,
    DfaInvalid
} DfaStatus;

bool isNumber(const char* input, DfaStatus* status);
