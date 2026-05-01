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
    if (!code || !name) {
        fprintf(stderr, "Error: arguments are NULL\n");
        return NULL;
    }

    AvlNode* node = (AvlNode*)calloc(1, sizeof(AvlNode));
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
    if (node) {
        return node->height;
    } else {
        return 0;
    }
}

static int getBalance(AvlNode* node)
{
    if (node) {
        return getHeight(node->left) - getHeight(node->right);
    } else {
        return 0;
    }
}

static void updateHeight(AvlNode* node)
{
    if (node) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        if (leftHeight > rightHeight) {
            node->height = leftHeight + 1;
        } else {
            node->height = rightHeight + 1;
        }
    }
}

static AvlNode* rotateRight(AvlNode* oldRoot)
{
    if (!oldRoot->left) {
        return oldRoot;
    }

    AvlNode* newRoot = oldRoot->left;
    AvlNode* subtree = newRoot->right;

    newRoot->right = oldRoot;
    oldRoot->left = subtree;

    updateHeight(oldRoot);
    updateHeight(newRoot);
    return newRoot;
}

static AvlNode* rotateLeft(AvlNode* oldRoot)
{
    if (!oldRoot->right) {
        return oldRoot;
    }

    AvlNode* newRoot = oldRoot->right;
    AvlNode* subtree = newRoot->left;

    newRoot->left = oldRoot;
    oldRoot->right = subtree;

    updateHeight(oldRoot);
    updateHeight(newRoot);
    return newRoot;
}

static AvlNode* balanceNode(AvlNode* node)
{
    if (!node)
        return NULL;

    updateHeight(node);
    int balanceFactor = getBalance(node);

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
    if (!node)
        return NULL;

    AvlNode* current = node;
    while (current->left) {
        current = current->left;
    }
    return current;
}

static AvlNode* insertNode(AvlNode* root, const char* code, const char* name)
{
    if (!root) {
        AvlNode* newNode = nodeCreate(code, name);
        if (!newNode) {
            fprintf(stderr, "Error: Failed to create new node for %s\n", code);
            return NULL;
        }
        return newNode;
    }

    int compareResult = strcmp(code, root->code);

    if (compareResult < 0) {
        root->left = insertNode(root->left, code, name);
        if (!root->left && root->left != insertNode(root->left, code, name)) {
            if (root->left == NULL) {
                return NULL;
            }
        }
    } else if (compareResult > 0) {
        root->right = insertNode(root->right, code, name);
        if (!root->right && root->right != insertNode(root->right, code, name)) {
            if (root->right == NULL) {
                return NULL;
            }
        }
    } else {
        return root;
    }

    AvlNode* balanced = balanceNode(root);
    if (!balanced) {
        fprintf(stderr, "Error: Failed to balance tree\n");
        return root;
    }
    return balanced;
}

static AvlNode* deleteNode(AvlNode* root, const char* code)
{
    if (!root)
        return NULL;

    int compareResult = strcmp(code, root->code);

    if (compareResult < 0) {
        root->left = deleteNode(root->left, code);
    } else if (compareResult > 0) {
        root->right = deleteNode(root->right, code);
    } else {
        if (!root->left || !root->right) {
            AvlNode* temp = NULL;
            if (root->left) {
                temp = root->left;
            } else {
                temp = root->right;
            }

            if (!temp) {
                free(root->code);
                free(root->name);
                free(root);
                return NULL;
            } else {
                AvlNode* nodeToDelete = root;
                root = temp;
                free(nodeToDelete->code);
                free(nodeToDelete->name);
                free(nodeToDelete);
            }
        } else {
            AvlNode* minNode = findMinNode(root->right);
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

    AvlNode* balanced = balanceNode(root);
    if (!balanced && root) {
        fprintf(stderr, "Error: Failed to balance tree\n");
        return root;
    }
    return balanced;
}

static const char* findNode(AvlNode* root, const char* code)
{
    if (!root)
        return NULL;

    int compareResult = strcmp(code, root->code);
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
    if (root) {
        return 1 + countNodes(root->left) + countNodes(root->right);
    } else {
        return 0;
    }
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
    if (!tree) {
        fprintf(stderr, "Error: tree is NULL\n");
        return;
    }
    if (!filename) {
        fprintf(stderr, "Error: filename is NULL\n");
        return;
    }

    FILE* file = fopen(filename, "w");
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
