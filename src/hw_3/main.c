#include "airport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_FIND "find "
#define CMD_ADD "add "
#define CMD_DELETE "delete "

// Динамическое чтение строки
char* readDynamicLine(FILE* file)
{
    size_t capacity = 64;
    size_t length = 0;
    char* buffer;
    int character;
    char* newBuffer;

    buffer = (char*)calloc(capacity, sizeof(char));
    if (!buffer)
        return NULL;

    while ((character = fgetc(file)) != EOF && character != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            newBuffer = (char*)realloc(buffer, capacity);
            if (!newBuffer) {
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }
        buffer[length++] = (char)character;
    }

    if (length == 0 && character == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

void trimString(char* string)
{
    size_t length;
    char* end;
    char* start;

    if (!string)
        return;

    length = strlen(string);
    if (length == 0)
        return;

    // Убирание пробелов с конца
    end = string + length - 1;
    while (end >= string && (*end == '\n' || *end == '\r' || *end == ' ' || *end == '\t')) {
        *end = '\0';
        end--;
    }

    // Убирание пробелов с начала
    start = string;
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    if (start != string) {
        memmove(string, start, strlen(start) + 1);
    }
}

// Динамическое копирование строки
char* duplicateString(const char* source)
{
    size_t length;
    char* destination;

    if (!source)
        return NULL;

    length = strlen(source);
    destination = (char*)calloc(length + 1, sizeof(char));
    if (!destination)
        return NULL;

    strcpy(destination, source);
    return destination;
}

// загрузка файла
AvlTree* loadFile(const char* filename)
{
    FILE* file;
    AvlTree* tree;
    int airportsCount;
    char* line;
    char* separator;
    size_t codeLength;
    char* airportCode;
    char* airportName;

    if (!filename)
        return NULL;

    file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open %s\n", filename);
        return NULL;
    }

    tree = avlCreate();
    if (!tree) {
        fclose(file);
        return NULL;
    }

    airportsCount = 0;

    while ((line = readDynamicLine(file)) != NULL) {
        trimString(line);

        if (strlen(line) > 0) {
            separator = strchr(line, ':');
            if (!separator) {
                free(line);
                continue;
            }

            codeLength = separator - line;

            if (codeLength == 0) {
                free(line);
                continue;
            }

            airportCode = (char*)calloc(codeLength + 1, sizeof(char));
            if (!airportCode) {
                printf("Error: Failed to allocate memory for airport code\n");
                free(line);
                continue;
            }

            strncpy(airportCode, line, codeLength);
            airportCode[codeLength] = '\0';

            airportName = duplicateString(separator + 1);
            if (!airportName) {
                printf("Error: Failed to allocate memory for airport name\n");
                free(airportCode);
                free(line);
                continue;
            }

            if (strlen(airportName) == 0) {
                free(airportCode);
                free(airportName);
                free(line);
                continue;
            }

            insertAirport(tree, airportCode, airportName);
            free(airportCode);
            free(airportName);
            airportsCount++;
        }
        free(line);
    }

    fclose(file);
    printf("Loaded %d airports. System ready.\n", airportsCount);
    return tree;
}

int main(int argc, char* argv[])
{
    AvlTree* tree;
    char* command;
    const size_t find_len = strlen(CMD_FIND);
    const size_t add_len = strlen(CMD_ADD);
    const size_t delete_len = strlen(CMD_DELETE);

    if (argc != 2) {
        printf("Error: No filename\n");
        return 1;
    }

    tree = loadFile(argv[1]);
    if (!tree)
        return 1;

    command = NULL;

    while (true) {
        printf("> ");

        command = readDynamicLine(stdin);
        if (!command) {
            break;
        }

        trimString(command);
        if (strlen(command) == 0) {
            free(command);
            continue;
        }

        if (strncmp(command, CMD_FIND, find_len) == 0) {
            char* airportCode = command + find_len;
            const char* airportName;

            if (strlen(airportCode) == 0) {
                printf("Error: mistake in a command. Right usage: find <code>\n");
                free(command);
                continue;
            }

            airportName = findAirport(tree, airportCode);
            if (airportName) {
                printf("%s -> %s\n", airportCode, airportName);
            } else {
                printf("Airport '%s' not found.\n", airportCode);
            }
        } else if (strncmp(command, CMD_ADD, add_len) == 0) {
            char* argument = command + add_len;
            char* delimiter;
            size_t codeLength;
            size_t nameLength;
            char* airportCode;
            char* airportName;

            if (strlen(argument) == 0) {
                printf("Error: mistake in a command. Right usage: add <code>:<name>\n");
                free(command);
                continue;
            }

            delimiter = strchr(argument, ':');
            if (!delimiter) {
                printf("Error: mistake in a command. Right usage: add <code>:<name>\n");
                free(command);
                continue;
            }

            codeLength = delimiter - argument;
            nameLength = strlen(delimiter + 1);

            if (codeLength == 0 || nameLength == 0) {
                printf("Error: code and name must be not NULL\n");
                free(command);
                continue;
            }

            airportCode = (char*)calloc(codeLength + 1, sizeof(char));
            if (!airportCode) {
                printf("Error: Memory allocation failed\n");
                free(command);
                continue;
            }

            strncpy(airportCode, argument, codeLength);
            airportCode[codeLength] = '\0';

            airportName = duplicateString(delimiter + 1);
            if (!airportName) {
                printf("Error: Memory allocation failed\n");
                free(airportCode);
                free(command);
                continue;
            }

            if (findAirport(tree, airportCode)) {
                printf("Airport '%s' already exists.\n", airportCode);
                free(airportCode);
                free(airportName);
                free(command);
                continue;
            }

            insertAirport(tree, airportCode, airportName);
            free(airportCode);
            free(airportName);
            printf("Airport '%s' added.\n", airportCode);
        } else if (strncmp(command, CMD_DELETE, delete_len) == 0) {
            char* airportCode = command + delete_len;

            if (strlen(airportCode) == 0) {
                printf("Error: mistake in a command. Right usage: delete <code>\n");
                free(command);
                continue;
            }

            if (!findAirport(tree, airportCode)) {
                printf("Airport '%s' not found.\n", airportCode);
                free(command);
                continue;
            }

            deleteAirport(tree, airportCode);
            printf("Airport '%s' deleted.\n", airportCode);
        } else if (strcmp(command, "save") == 0) {
            saveAirport(tree, "airports.txt");
            printf("Saved %d airports.\n", countAirport(tree));
        } else if (strcmp(command, "quit") == 0) {
            printf("System stopped\n");
            free(command);
            break;
        } else {
            printf("Error: Unknown command\n");
            printf("Use only find <code>, add <code>:<name>, delete <code>, save, quit\n");
        }

        free(command);
        command = NULL;
    }

    free(command);
    avlDestroy(tree);
    return 0;
}