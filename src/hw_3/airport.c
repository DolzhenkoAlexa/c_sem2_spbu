#include "airport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AvlNode {
    char* code;
    char* name;
    struct AvlNode* left;
    struct AvlNode* right;
    int height;
} AvlNode;

struct AvlTree {
    AvlNode* root;
};

static AvlNode* nodeCreate(const char* code, const char* name)
{
    AvlNode* node;

    if (!code || !name) {
        fprintf(stderr, "Error: arguments are NULL\n");
        return NULL;
    }

    node = (AvlNode*)calloc(1, sizeof(AvlNode));
    if (!node) {
        fprintf(stderr, "Error: Failed to allocate memory for AVL node\n");
        return NULL;
    }

    node->code = (char*)calloc(strlen(code) + 1, sizeof(char));
    if (!node->code) {
        fprintf(stderr, "Error: Failed to allocate memory for airport code\n");
        free(node);
        return NULL;
    }

    node->name = (char*)calloc(strlen(name) + 1, sizeof(char));
    if (!node->name) {
        fprintf(stderr, "Error: Failed to allocate memory for airport name\n");
        free(node->code);
        free(node);
        return NULL;
    }

    strcpy(node->code, code);
    strcpy(node->name, name);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return node;
}

static void nodeFree(AvlNode* node)
{
    if (!node)
        return;
    nodeFree(node->left);
    nodeFree(node->right);
    free(node->code);
    free(node->name);
    free(node);
}

static int getHeight(AvlNode* node)
{
    return node ? node->height : 0;
}

static int getBalance(AvlNode* node)
{
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

static void updateHeight(AvlNode* node)
{
    int leftHeight;
    int rightHeight;

    if (!node)
        return;

    leftHeight = getHeight(node->left);
    rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

static AvlNode* rotateRight(AvlNode* oldRoot)
{
    AvlNode* newRoot;
    AvlNode* subtree;

    if (!oldRoot->left) {
        return oldRoot;
    }

    newRoot = oldRoot->left;
    subtree = newRoot->right;

    newRoot->right = oldRoot;
    oldRoot->left = subtree;

    updateHeight(oldRoot);
    updateHeight(newRoot);

    return newRoot;
}

static AvlNode* rotateLeft(AvlNode* oldRoot)
{
    AvlNode* newRoot;
    AvlNode* subtree;

    if (!oldRoot->right) {
        return oldRoot;
    }

    newRoot = oldRoot->right;
    subtree = newRoot->left;

    newRoot->left = oldRoot;
    oldRoot->right = subtree;

    updateHeight(oldRoot);
    updateHeight(newRoot);

    return newRoot;
}

static AvlNode* balanceNode(AvlNode* node)
{
    int balanceFactor;

    if (!node)
        return NULL;

    updateHeight(node);
    balanceFactor = getBalance(node);

    if (balanceFactor > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }

    if (balanceFactor > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        if (!node->left) {
            fprintf(stderr, "Error: Failed to balance node\n");
            return NULL;
        }
        return rotateRight(node);
    }

    if (balanceFactor < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }

    if (balanceFactor < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        if (!node->right) {
            fprintf(stderr, "Error: Failed to balance node\n");
            return NULL;
        }
        return rotateLeft(node);
    }

    return node;
}

static AvlNode* findMinNode(AvlNode* node)
{
    AvlNode* current;

    if (!node)
        return NULL;

    current = node;
    while (current->left) {
        current = current->left;
    }
    return current;
}

static AvlNode* deleteNodeWithoutChildren(AvlNode* node)
{
    if (!node)
        return NULL;

    free(node->code);
    free(node->name);
    free(node);
    return NULL;
}

static AvlNode* replaceNodeWithChild(AvlNode* node, AvlNode* child)
{
    if (!node)
        return child;

    free(node->code);
    free(node->name);
    free(node);
    return child;
}

static AvlNode* insertNode(AvlNode* root, const char* code, const char* name)
{
    int compareResult;
    AvlNode* balanced;

    if (!root) {
        AvlNode* newNode = nodeCreate(code, name);
        if (!newNode) {
            fprintf(stderr, "Error: Failed to create new node for %s\n", code);
            return NULL;
        }
        return newNode;
    }

    compareResult = strcmp(code, root->code);
    if (compareResult < 0) {
        root->left = insertNode(root->left, code, name);
    } else if (compareResult > 0) {
        root->right = insertNode(root->right, code, name);
    } else {
        return root;
    }

    balanced = balanceNode(root);
    if (!balanced) {
        fprintf(stderr, "Error: Failed to balance tree\n");
        return root;
    }
    return balanced;
}

static AvlNode* deleteNode(AvlNode* root, const char* code)
{
    int compareResult;
    AvlNode* balanced;
    AvlNode* child;
    AvlNode* minNode;

    if (!root)
        return NULL;

    compareResult = strcmp(code, root->code);

    if (compareResult < 0) {
        root->left = deleteNode(root->left, code);
    } else if (compareResult > 0) {
        root->right = deleteNode(root->right, code);
    } else {
        if (!root->left || !root->right) {
            child = root->left ? root->left : root->right;

            if (!child) {
                return deleteNodeWithoutChildren(root);
            } else {
                return replaceNodeWithChild(root, child);
            }
        } else {
            minNode = findMinNode(root->right);
            if (!minNode) {
                fprintf(stderr, "Error: Failed to find minimum node\n");
                return balanceNode(root);
            }

            free(root->code);
            free(root->name);

            root->code = (char*)calloc(strlen(minNode->code) + 1, sizeof(char));
            if (!root->code) {
                fprintf(stderr, "Error: Failed to allocate memory for code\n");
                free(root);
                return NULL;
            }

            root->name = (char*)calloc(strlen(minNode->name) + 1, sizeof(char));
            if (!root->name) {
                fprintf(stderr, "Error: Failed to allocate memory for name\n");
                free(root->code);
                free(root);
                return NULL;
            }

            strcpy(root->code, minNode->code);
            strcpy(root->name, minNode->name);

            root->right = deleteNode(root->right, minNode->code);
        }
    }

    balanced = balanceNode(root);
    if (!balanced && root) {
        fprintf(stderr, "Error: Failed to balance tree\n");
        return root;
    }
    return balanced;
}

static const char* findNode(AvlNode* root, const char* code)
{
    int compareResult;

    if (!root)
        return NULL;

    compareResult = strcmp(code, root->code);
    if (compareResult < 0) {
        return findNode(root->left, code);
    }
    if (compareResult > 0) {
        return findNode(root->right, code);
    }
    return root->name;
}

static void saveNode(AvlNode* root, FILE* file)
{
    if (!root || !file)
        return;

    saveNode(root->left, file);
    if (fprintf(file, "%s:%s\n", root->code, root->name) < 0) {
        fprintf(stderr, "Error: Failed to write to file\n");
    }
    saveNode(root->right, file);
}

static int countNodes(AvlNode* root)
{
    return root ? 1 + countNodes(root->left) + countNodes(root->right) : 0;
}

AvlTree* avlCreate(void)
{
    AvlTree* tree = (AvlTree*)calloc(1, sizeof(AvlTree));
    if (!tree) {
        fprintf(stderr, "Error: Failed to create AVL tree\n");
    }
    return tree;
}

void avlDestroy(AvlTree* tree)
{
    if (tree) {
        nodeFree(tree->root);
        free(tree);
    }
}

void insertAirport(AvlTree* tree, const char* code, const char* name)
{
    if (!tree) {
        fprintf(stderr, "Error: tree is NULL\n");
        return;
    }
    if (!code) {
        fprintf(stderr, "Error: code is NULL\n");
        return;
    }
    if (!name) {
        fprintf(stderr, "Error: name is NULL\n");
        return;
    }
    tree->root = insertNode(tree->root, code, name);
}

void deleteAirport(AvlTree* tree, const char* code)
{
    if (!tree) {
        fprintf(stderr, "Error: tree is NULL\n");
        return;
    }
    if (!code) {
        fprintf(stderr, "Error: code is NULL\n");
        return;
    }
    tree->root = deleteNode(tree->root, code);
}

const char* findAirport(AvlTree* tree, const char* code)
{
    if (!tree) {
        fprintf(stderr, "Error: tree is NULL\n");
        return NULL;
    }
    if (!code) {
        fprintf(stderr, "Error: code is NULL\n");
        return NULL;
    }
    return findNode(tree->root, code);
}

void saveAirport(AvlTree* tree, const char* filename)
{
    FILE* file;

    if (!tree) {
        fprintf(stderr, "Error: tree is NULL\n");
        return;
    }
    if (!filename) {
        fprintf(stderr, "Error: filename is NULL\n");
        return;
    }

    file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
        return;
    }

    saveNode(tree->root, file);

    if (fclose(file) != 0) {
        fprintf(stderr, "Error: Failed to close file '%s'\n", filename);
    }
}

int countAirport(AvlTree* tree)
{
    if (!tree) {
        fprintf(stderr, "Error: tree is NULL\n");
        return 0;
    }
    return countNodes(tree->root);
}