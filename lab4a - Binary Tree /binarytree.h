#ifndef binarytree_h
#define binarytree_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FILENAME  "TABLE.txt"

typedef struct BinaryTreeNode {         // узел дерева
    int key;                            // ключ
    int data;                           // информация
    struct BinaryTreeNode *left;       // указатель на левое поддерево
    struct BinaryTreeNode *right;      // указатель на правое поддерево
    struct BinaryTreeNode *parent;     // указатель на родительскую вершину. NULL, если вершина - корень
} BinaryTreeNode;

BinaryTreeNode *newBinaryTreeNode(int key, int value);

int add (BinaryTreeNode **head, int key, int value);

BinaryTreeNode *find (BinaryTreeNode *head, int key);
BinaryTreeNode *findNext (BinaryTreeNode *node);
BinaryTreeNode *findMin (BinaryTreeNode *head);

void printNode(BinaryTreeNode *node);
void Show (BinaryTreeNode *ptr, int lvl, void (*otherFunction)(BinaryTreeNode*));

int addStringsGenerate (BinaryTreeNode **head, int count);

int delete (BinaryTreeNode **head, int key);

void freeNode(BinaryTreeNode *node);

void fileWriteNode(BinaryTreeNode *node, FILE *f);
void bypass(BinaryTreeNode *node, FILE *f);
int Load(BinaryTreeNode **head);

void getInt(int *a);



#endif
