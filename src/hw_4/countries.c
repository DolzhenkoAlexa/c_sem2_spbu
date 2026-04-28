#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "countries.h"

int graphInit(Graph* graph, int numCities)
{
    graph->numCities = numCities;
    graph->headArray = (Edge**)calloc(numCities + 1, sizeof(Edge*));

    if (graph->headArray == NULL) {
        return -1;
    }

    return 0;
}

void graphAdd(Graph* graph, int fromCity, int toCity, int length)
{
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    if (newEdge == NULL) {
        return;
    }
    newEdge->toCity = toCity;
    newEdge->length = length;
    newEdge->nextEdge = graph->headArray[fromCity];
    graph->headArray[fromCity] = newEdge;

    newEdge = (Edge*)malloc(sizeof(Edge));
    if (newEdge == NULL) {
        return;
    }
    newEdge->toCity = fromCity;
    newEdge->length = length;
    newEdge->nextEdge = graph->headArray[toCity];
    graph->headArray[toCity] = newEdge;
}

void graphFree(Graph* graph)
{
    if (graph == NULL || graph->headArray == NULL) {
        return;
    }

    for (int i = 1; i <= graph->numCities; i++) {
        Edge* currEdge = graph->headArray[i];
        while (currEdge) {
            Edge* tempEdge = currEdge;
            currEdge = currEdge->nextEdge;
            free(tempEdge);
        }
    }
    free(graph->headArray);
    graph->headArray = NULL;
}

int stateInit(State* state)
{
    state->citiesArray = (int*)malloc(10 * sizeof(int));
    if (state->citiesArray == NULL) {
        return -1;
    }
    state->countCities = 0;
    return 0;
}

int stateAdd(State* state, int city)
{
    // Проверка на необходимость расширения массива (каждые 10 элементов)
    if (state->countCities >= 10 && state->countCities % 10 == 0) {
        int* newArray = (int*)realloc(state->citiesArray, (state->countCities + 10) * sizeof(int));
        if (newArray == NULL) {
            return -1;
        }
        state->citiesArray = newArray;
    }

    state->citiesArray[state->countCities++] = city;
    return 0;
}

void stateFree(State* state)
{
    if (state == NULL || state->citiesArray == NULL) {
        return;
    }
    free(state->citiesArray);
    state->citiesArray = NULL;
    state->countCities = 0;
}

int floydWarshall(int** distMatrix, int numCities, Graph* graph)
{
    // Инициализация матрицы расстояний
    for (int i = 1; i <= numCities; i++) {
        for (int j = 1; j <= numCities; j++) {
            distMatrix[i][j] = (i == j) ? 0 : MAX;
        }
    }

    // Заполняем ребра
    for (int i = 1; i <= numCities; i++) {
        Edge* currEdge = graph->headArray[i];
        while (currEdge) {
            distMatrix[i][currEdge->toCity] = currEdge->length;
            currEdge = currEdge->nextEdge;
        }
    }

    // Флойд-Уоршелл
    for (int k = 1; k <= numCities; k++) {
        for (int i = 1; i <= numCities; i++) {
            for (int j = 1; j <= numCities; j++) {
                if (distMatrix[i][k] != MAX && distMatrix[k][j] != MAX) {
                    if (distMatrix[i][k] + distMatrix[k][j] < distMatrix[i][j]) {
                        distMatrix[i][j] = distMatrix[i][k] + distMatrix[k][j];
                    }
                }
            }
        }
    }

    return 0;
}

// Расстояние от государства до города
int distToState(State* state, int city, int** distMatrix)
{
    int minDist = MAX;
    for (int i = 0; i < state->countCities; i++) {
        int currDist = distMatrix[state->citiesArray[i]][city];
        if (currDist < minDist) {
            minDist = currDist;
        }
    }
    return minDist;
}

int expand(Graph* graph, State* statesArray, int* ownerArray, int numCities, int numStates)
{
    // матрица кратчайших расстояний
    int** distMatrix = (int**)malloc((numCities + 1) * sizeof(int*));
    if (distMatrix == NULL) {
        return -1;
    }

    for (int i = 1; i <= numCities; i++) {
        distMatrix[i] = (int*)malloc((numCities + 1) * sizeof(int));
        if (distMatrix[i] == NULL) {
            for (int j = 1; j < i; j++) {
                free(distMatrix[j]);
            }
            free(distMatrix);
            return -1;
        }
    }

    if (floydWarshall(distMatrix, numCities, graph) == -1) {
        for (int i = 1; i <= numCities; i++) {
            free(distMatrix[i]);
        }
        free(distMatrix);
        return -1;
    }

    int occupiedCount = numStates;

    while (occupiedCount < numCities) {
        for (int stateIndex = 1; stateIndex <= numStates; stateIndex++) {
            if (occupiedCount >= numCities) {
                break;
            }

            int bestCity = -1;
            int bestDist = MAX;

            // Ищем ближайший свободный город
            for (int city = 1; city <= numCities; city++) {
                if (ownerArray[city] == 0) {
                    int currDist = distToState(&statesArray[stateIndex], city, distMatrix);
                    if (currDist < bestDist) {
                        bestDist = currDist;
                        bestCity = city;
                    }
                }
            }

            // Захватываем найденный город
            if (bestCity != -1) {
                if (stateAdd(&statesArray[stateIndex], bestCity) == -1) {
                    for (int i = 1; i <= numCities; i++) {
                        free(distMatrix[i]);
                    }
                    free(distMatrix);
                    return -1;
                }
                ownerArray[bestCity] = stateIndex;
                occupiedCount++;
            }
        }
    }

    // Освобождаем матрицу расстояний
    for (int i = 1; i <= numCities; i++) {
        free(distMatrix[i]);
    }
    free(distMatrix);

    return 0;
}
