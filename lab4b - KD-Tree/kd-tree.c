#include "kd-tree.h"

KDTreeNode *newKDTreeNode (int x, int y, int value){
    KDTreeNode* new = (KDTreeNode*)calloc(1, sizeof(KDTreeNode));
    new->x = x;
    new->y = y;
    new->data = value;
    return new;
}

int add (KDTreeNode **head, int x, int y, int value){
    if (*head == NULL){
        *head = newKDTreeNode(x, y, value);
        return 1;
    }
    KDTreeNode *node = *head;
    KDTreeNode *parent = NULL;
    int lvl = 0;
    
    for (int i = 0; node != NULL; i++){
        if (node->x == x && node->y == y){
            return 0;
        }
        if (i % 2 == 0){
            if (x < node->x){
                parent = node;
                node = node->left;
            }
            else {
                parent = node;
                node = node->right;
            }
        }
        else {
            if (y < node->y){
                parent = node;
                node = node->left;
            }
            else {
                parent = node;
                node = node->right;
            }
        }
        lvl++;
    }
    node = newKDTreeNode(x, y, value);
    node->parent = parent;
    if ((lvl-1) % 2 == 0){
        if (node->x < parent->x){
            parent->left = node;
        }
        else {
            parent->right = node;
        }
    }
    else {
        if (node->y < parent->y){
            parent->left = node;
        }
        else {
            parent->right = node;
        }
    }
    return 1;
}

KDTreeNode *find (KDTreeNode *node, int x, int y, int lvl){
    for (int i = 0; node != NULL; i++){
        if (node->x == x && node->y == y){
            return node;
        }
        if (i % 2 == 0){
            if (x < node->x){
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        else {
            if (y < node->y){
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        lvl++;
    }
    return NULL;
}

void printNode (KDTreeNode* node){
    printf("[%d, %d]: ", node->x, node->y);
    printf("%d", node->data);
    printf("\n");
}

void Show (KDTreeNode* ptr, int lvl, void (*otherFunction)(KDTreeNode*)){
    if (ptr != NULL){
        Show (ptr->right, lvl + 1, otherFunction);
        if(otherFunction == NULL){
            for (int j = 0; j < lvl; j++)
            printf("        ");
            printNode(ptr);
        }
        Show (ptr->left, lvl + 1, otherFunction);
        if(otherFunction != NULL)
            otherFunction(ptr);
    }
}

int del (KDTreeNode **head, int x, int y){
    if (*head == NULL){
        return 0;
    }
    KDTreeNode **node = head;
    KDTreeNode *del = find(*head, x, y, 0);
    if (!del){
        return 0;
    }
    if (del->right == NULL && del->left == NULL){
        
        if (del->parent == NULL){               // если удаляем корень дерева (=> дерево из одной вершины)
            *head = NULL;
            return 1;
        }
        if (del->parent->right == del)          // обрываем связь родителя с вершиной
            del->parent->right = NULL;
        if (del->parent->left == del)
            del->parent->left = NULL;
        del->parent = NULL;                      // обрываем связь с родителем
        return 1;
    }
    
    if (del->right == NULL || del->left == NULL){
        if (del->parent == NULL){               // если удаляем корень
            if (del->left == NULL){
                *head = del->right;
                return 1;
            }                                   // ищем замену корню, им станет его непустое поддерево
            if (del->right == NULL){
                *head = del->left;
                return 1;
            }
        }
        else {
            if (del->left == NULL){                     // если у вершины нет левого поддерева
                if (del->parent->right == del){         // переопределяем связи
                    del->parent->right = del->right;    // в зависимости от того где находится вершина
                    del->right->parent = del->parent;   // относительно своего родителя
                }
                if (del->parent->left == del){
                    del->parent->left = del->right;
                    del->right->parent = del->parent;
                }
            }
            if (del->right == NULL){                    // если у вершины нет правого поддерева
                if (del->parent->right == del){         // делаем все то же самое
                    del->parent->right = del->left;
                    del->left->parent = del->parent;
                }
                if (del->parent->left == del){
                    del->parent->left = del->left;
                    del->left->parent = del->parent;
                }
            }
        }
    }
    if (del->parent != NULL){
        if (del->parent->right == del){
            del->parent->right = del->right;
        }
        if (del->parent->left == del){
            del->parent->left = del->right;
        }
        del->right->left = del->left;
        del->left->parent = del->right;
        del->right->parent = del->parent;
        free (del);
        return 1;
    }
    if (del->parent == NULL){
        del->right->left = del->left;
        del->right->parent = NULL;
        del->left->parent = del->right;
        *node = del->right;
        free(del);
    }
    return 1;
    
    
}

int addStringsGenerate (KDTreeNode** head, int count){
    srand(time(0));
    int x, y, val;
    for (int i = 0; i < count; i++){
        x = X_MIN + rand() % (2 * X_MAX);
        y = Y_MIN + rand() % (2 * Y_MAX);
        val = 1 + rand() % 1000000;
        add (head, x, y, val);
    }
    return 1;
}

FILE* f;
void bypass (KDTreeNode* node, FILE *f){
    if (node != NULL){
        fileWriteNode(node, f);
        bypass(node->left, f);
        bypass(node->right, f);
    }
}

void fileWriteNode(KDTreeNode* node, FILE *f){
    fprintf(f, "[%d, %d]: %d\n", node->x, node->y ,node->data);
}

int fileRead(KDTreeNode** head){        // чтение данных из файла
    int x, y;
    int val;
    if (f = fopen(FILENAME, "r"))
    while (!feof(f)) {
        fscanf(f, "[%d, %d]: %d\n", &x, &y, &val);
        add(head, x, y, val);
    }
    printf("Прочитано из файла:\n");
    Show(*head, 0, NULL);
    fclose(f);
    return 1;
}

void freeNode(KDTreeNode* node){                 // очистка памяти, занимаемой узлом
    free(node);
}

void getInt(int *a){
    int n = 0, flag=0;
    do
    {
        if (flag == 1){
            printf("Ошибка. Повторите ввод: ");
            scanf("%*[^\n]");
        }
        n = scanf("%d", a);
        flag = 1;
    } while (n <= 0);
    scanf("%*c");
}
