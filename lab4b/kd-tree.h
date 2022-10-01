#ifndef kd_tree_h
#define kd_tree_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X_MAX 10000
#define X_MIN -10000
#define Y_MAX 10000
#define Y_MIN -10000

#define FILENAME  "TABLE.txt"

typedef struct KDTreeNode{
    int x;
    int y;
    int data;
    struct KDTreeNode *right;
    struct KDTreeNode *left;
    struct KDTreeNode *parent;
} KDTreeNode;

KDTreeNode *newKDTreeNode (int x, int y, int value);

int add (KDTreeNode **head, int x, int y, int value);

KDTreeNode *find (KDTreeNode *head, int x, int y, int lvl);

void printNode(KDTreeNode* node);
void Show (KDTreeNode* ptr, int lvl, void (*otherFunction)(KDTreeNode*));

int addStringsGenerate (KDTreeNode** head, int count);

int del (KDTreeNode **head, int x, int y);

void freeNode(KDTreeNode* node);

void bypass(KDTreeNode* node, FILE *f);
void fileWriteNode(KDTreeNode* node, FILE *f);
int fileRead(KDTreeNode** head);
int D_Write(KDTreeNode** head);

void getInt(int *a);

#endif


