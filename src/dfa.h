#pragma once
#include <stdbool.h>

typedef enum {
    DFA_OK,
    DFA_INVALID
} DfaStatus;

bool isNumber(const char* input, DfaStatus* status);
