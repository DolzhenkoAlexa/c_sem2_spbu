#pragma once
#include <stdbool.h>

// Коды ошибок
typedef enum DfaStatus {
    DfaOk, // Правильная строка
    DfaEmptyInput, // Пустая строка
    DfaInvalidSymbol, // Символ не из алфавита
    DfaNoTransition // Нет перехода для допустимого символа
} DfaStatus;

// Описание одного перехода
typedef struct {
    int from;
    char symbol;
    int to;
} Transition;

typedef struct DFA DFA;

// Создание ДКА
DFA* makeDfa(Transition* transitions, int transCount,
    int* accepting, int accCount, int startState);

// Освобождение памяти
void freeDfa(DFA* dfa);

// Проверка соответствует ли строка заданному ДКА
bool checkDfa(DFA* dfa, const char* input, DfaStatus* status);