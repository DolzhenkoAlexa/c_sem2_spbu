#include <stdio.h>
#include <stdlib.h>
#include "countries.h"

int main() 
{
    Graph roadGraph;
    State* statesArray;
    int* ownerArray;
    int numCities, numRoads, numStates;

    printf("Enter number of cities and roads: ");
    scanf("%d %d", &numCities, &numRoads);

    graphInit(&roadGraph, numCities);
    statesArray = (State*)calloc(numCities + 1, sizeof(State));
    ownerArray = (int*)calloc(numCities + 1, sizeof(int));

    printf("Enter roads (two cities and len):\n");
    for (int i = 0; i < numRoads; i++) {
        int fromCity, toCity, roadLength;
        scanf("%d %d %d", &fromCity, &toCity, &roadLength);
        graphAdd(&roadGraph, fromCity, toCity, roadLength);
    }

    printf("Enter number of capitals: ");
    scanf("%d", &numStates);

    printf("Enter capitals:\n");
    for (int i = 1; i <= numStates; i++) {
        stateInit(&statesArray[i]);
        int capitalCity;
        scanf("%d", &capitalCity);
        stateAdd(&statesArray[i], capitalCity);
        ownerArray[capitalCity] = i;
    }

    expand(&roadGraph, statesArray, ownerArray, numCities, numStates);

    printf("\nResults:\n");
    for (int i = 1; i <= numStates; i++) {
        printf("State %d: ", i);
        for (int j = 0; j < statesArray[i].countCities; j++) {
            printf("%d ", statesArray[i].citiesArray[j]);
        }
        printf("\n");
        stateFree(&statesArray[i]);
    }

    graphFree(&roadGraph);
    free(statesArray);
    free(ownerArray);

    return 0;
}
