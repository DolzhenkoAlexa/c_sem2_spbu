#pragma once

typedef struct AvlTree AvlTree;

// Создание нового дерева
AvlTree* avlCreate(void);

// Удаление дерева и освобождение всей памяти
void avlDestroy(AvlTree* tree);

// Добавление аэропорта в дерево (по коду и названию)
void insertAirport(AvlTree* tree, const char* code, const char* name);

// Удаление аэропорта из дерева (по коду)
void deleteAirport(AvlTree* tree, const char* code);

// Поиск аэропорта в дереве (возвращает название или NULL)
const char* findAirport(AvlTree* tree, const char* code);

// Сохранение всех записей в файл
void saveAirport(AvlTree* tree, const char* filename);

// Подсчёт общего количества аэропортов в дереве
int countAirport(AvlTree* tree);
