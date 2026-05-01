#pragma once
#include <stdbool.h>

// Коды ошибок
typedef enum DfaStatus {
    DfaOk, // Правильная строка
    DfaEmptyInput, // Пустая строка
    DfaInvalidSymbol, // Символ не из алфавита
    DfaNoTransition // Нет перехода для допустимого символа
} DfaStatus;

// Enum для разных видов char
typedef enum CharClass {
    Digit,
    Sign,
    Dot,
    Exponent,
    Invalid
} CharClass;

// Описание одного перехода
typedef struct {
    int from;
    CharClass symbol;
    int to;
} Transition;

typedef struct DFA DFA;

// Создание ДКА
DFA* makeDfa(Transition* transitions, int transitionCount,
    int* accepting, int accCount, int startState);

// Освобождение памяти
void freeDfa(DFA* dfa);

// Проверка соответствует ли строка заданному ДКА
bool checkDfa(DFA* dfa, const char* input, DfaStatus* status);
