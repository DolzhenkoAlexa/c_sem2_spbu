#pragma once

#include <limits.h>
#include <stdlib.h>

#define MAX INT_MAX

// Edge - дорога между городами
typedef struct Edge {
    int toCity;
    int length;
    struct Edge* nextEdge;
} Edge;

// Graph - граф дорог
typedef struct Graph {
    Edge** headArray;
    int numCities;
} Graph;

// State - государство
typedef struct State {
    int* citiesArray;
    int countCities;
} State;

// Инициализация графа
// выделяет память под массив размером n+1
int graphInit(Graph* graph, int numCities);

// Добавление неориентированного ребра (дороги)
void graphAdd(Graph* graph, int fromCity, int toCity, int length);

// Освобождение памяти графа
void graphFree(Graph* graph);

// Инициализация государства
// Выделяет начальную память под массив cities
int stateInit(State* state);

// Добавление города в государство
int stateAdd(State* state, int city);

// Освобождение памяти государства
void stateFree(State* state);

// Алгоритм Флойда-Уоршелла для поиска кратчайших путей между всеми городами
int floydWarshall(int** distMatrix, int numCities, Graph* graph);

// Расстояние от государства до города
// вычисляет минимальное расстояние от любого города государства до указанного города
int distToState(State* state, int city, int** distMatrix);

// Основная функция расширения государств
// Алгоритм:
//  1. строит матрицу кратчайших расстояний между всеми городами
//  2. пока все города не распределены:
//  - Для каждого государства по очереди (от 1 до k) находит ближайший город
//  - захватывает его (добавляет в свое государство)
int expand(Graph* graph, State* statesArray, int* ownerArray, int numCities, int numStates);
