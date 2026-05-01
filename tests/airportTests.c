#include "../src/hw_3/airport.h"
#include <stdio.h>
#include <string.h>

int compareTest(AvlTree* tree, const char* code, const char* expected)
{
    const char* result = findAirport(tree, code);
    if (result == NULL && expected == NULL) {
        return 1;
    }
    if (result != NULL && expected != NULL && strcmp(result, expected) == 0) {
        return 1;
    }
    return 0;
}

// Тест 1: Создание дерева и добавление одного элемента
int test1(void)
{
    printf("Test 1: Create tree and add one airport\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    int passed = compareTest(tree, "SVX", "Koltsovo");
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 2: Добавление нескольких элементов и поиск
int test2(void)
{
    printf("Test 2: Add some airports and search\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    insertAirport(tree, "LED", "Pulkovo");
    insertAirport(tree, "SVO", "Sheremetyevo");
    
    int passed = 1;
    passed &= compareTest(tree, "SVX", "Koltsovo");
    passed &= compareTest(tree, "LED", "Pulkovo");
    passed &= compareTest(tree, "SVO", "Sheremetyevo");
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 3: Поиск несуществующего элемента
int test3(void)
{
    printf("Test 3: Search airport, that is not in a list\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    int passed = compareTest(tree, "XXX", NULL);
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 4: Удаление элемента
int test4(void)
{
    printf("Test 4: Delete airport\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    insertAirport(tree, "LED", "Pulkovo");
    deleteAirport(tree, "SVX");
    
    int passed = 1;
    passed &= compareTest(tree, "SVX", NULL);
    passed &= compareTest(tree, "LED", "Pulkovo");
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 5: Удаление корня
int test5(void)
{
    printf("Test 5: Delete root\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    insertAirport(tree, "LED", "Pulkovo");
    insertAirport(tree, "SVO", "Sheremetyevo");
    deleteAirport(tree, "SVX");
    
    int passed = compareTest(tree, "SVX", NULL);
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 6: Подсчёт количества элементов
int test6(void)
{
    printf("Test 6: Count airports\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    insertAirport(tree, "LED", "Pulkovo");
    insertAirport(tree, "SVO", "Sheremetyevo");
    
    int count = countAirport(tree);
    int passed = (count == 3);
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 7: Добавление дубликата (не должно добавиться)
int test7(void)
{
    printf("Test 7: Add duplicate airport\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    insertAirport(tree, "SVX", "Surgut");  // Дубликат
    
    const char* name = findAirport(tree, "SVX");
    int passed = (name != NULL && strcmp(name, "Koltsovo") == 0);
    passed &= (countAirport(tree) == 1);
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 8: Удаление несуществующего элемента
int test8(void)
{
    printf("Test 8: Delete airport, that is not in a list\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;
    
    insertAirport(tree, "SVX", "Koltsovo");
    insertAirport(tree, "LED", "Pulkovo");
    deleteAirport(tree, "XXX");
    
    int passed = 1;
    passed &= compareTest(tree, "SVX", "Koltsovo");
    passed &= compareTest(tree, "LED", "Pulkovo");
    passed &= (countAirport(tree) == 2);
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

// Тест 9: NULL аргументы (проверка, что не падает)
int test9(void)
{
    printf("Test 9: NULL arguments\n");
    AvlTree* tree = avlCreate();
    if (!tree) return 0;

    insertAirport(NULL, "SVX", "Koltsovo");
    insertAirport(tree, NULL, "Koltsovo");
    insertAirport(tree, "SVX", NULL);
    deleteAirport(NULL, "SVX");
    deleteAirport(tree, NULL);
    findAirport(NULL, "SVX");
    findAirport(tree, NULL);
    countAirport(NULL);
    insertAirport(tree, "SVX", "Koltsovo");
    int passed = compareTest(tree, "SVX", "Koltsovo");
    
    avlDestroy(tree);
    printf(passed ? "PASSED\n" : "FAILED\n");
    return passed;
}

int main(void)
{
    printf("Running tests:\n");
    int total = 0;
    
    total += test1();
    total += test2();
    total += test3();
    total += test4();
    total += test5();
    total += test6();
    total += test7();
    total += test8();
    total += test9();

    printf("Result: %d out of 9 tests passed\n", total);
    
    return 0;
}