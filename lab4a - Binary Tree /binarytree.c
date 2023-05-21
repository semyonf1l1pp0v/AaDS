#include "binarytree.h"

BinaryTreeNode *newBinaryTreeNode(int key, int value){
    BinaryTreeNode *new = (BinaryTreeNode*)calloc(1, sizeof(BinaryTreeNode));
    new->data = value;
    new->key = key;
    new->left = new->right = new->parent = NULL;
    return new;
}

int add (BinaryTreeNode **head, int key, int value){
    if (*head == NULL){                             // если дерево пусто
        *head = newBinaryTreeNode(key, value);
        return 1;
    }
    BinaryTreeNode *node = *head;
    BinaryTreeNode *parent = NULL;
    
    while (node != NULL){                           // проходимся по дереву
        if (node->key == key){                      //если узел с таким ключом уже есть
            return 0;
        }
        if (key < node->key){
            parent = node;
            node = node->left;
        }
        else {
            parent = node;
            node = node->right;
        }
    }
    node = newBinaryTreeNode(key, value);       // создали новый узел
    node->parent = parent;                      // выстроили связь с родителем
    if (node->key < parent->key){   // в зависимости от значения ключа добавили элемент влево/вправо
        parent->left = node;
    }
    else {
        parent->right = node;
    }
    return 1;
}


BinaryTreeNode *find (BinaryTreeNode *node, int key){
    while (node != NULL){       // просто проходимся по дереву
        if (node->key == key){  // нашли узел с данным ключом
            return node;
        }
        if (key < node->key){
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
    return NULL;                // не нашли узел - вернули null
}


BinaryTreeNode *findMin (BinaryTreeNode *head){
    if (head == NULL)   // дерево пусто
        return NULL;
    BinaryTreeNode *node = head;
    if (node->left != NULL){                // проходимся по левому поддереву, если оно есть
        while (node->left != NULL) {
            node = node->left;
        }
    }
    else {
        return node;
    }
    return node;
}


BinaryTreeNode *findNext (BinaryTreeNode *node){ // поиск след. элемента (алгоритм из лекции)
    if (node->right != NULL){       // если у элемента есть правое поддерево, то следующим элементом для него
        node = node->right;         // будет минимальный элемент правого поддерева
        if (node->left != NULL){
            while (node->left != NULL){
                node = node->left;
            }
            return node;
        }
        return node;
    }
    BinaryTreeNode *ptr = node->parent;         // родительский зел вершины
    while (ptr != NULL && node == ptr->right){  // пока есть ptr и node явл. его правым поддеревом
        node = ptr;                             // поднимаемся вверх по дереву
        ptr = node->parent;
    }
    return ptr;
}

int delete (BinaryTreeNode **head, int key){
    if (*head == NULL)     // проверка на пустоту
        return 0;
    BinaryTreeNode **node = head;
    BinaryTreeNode *del = find(*node, key);     // ищем вершину которую будем удалять
    if (!del)                                   // не нашли - вернули 0
        return 0;
    if (del->right == NULL && del->left == NULL){   // случай когда удаляемая вершина - лист
        if (del->parent == NULL){               // если удаляем корень дерева (=> дерево из одной вершины)
            *head = NULL;
            free(*head);
            return 1;
        }
        if (del->parent->right == del)          // обрываем связь родителя с вершиной
            del->parent->right = NULL;
        if (del->parent->left == del)
            del->parent->left = NULL;
        del->parent = NULL;                     // обрываем связь с родителем
        free(del);
        return 1;
    }
    if (del->right == NULL || del->left == NULL){   // если удаляем вершину с левым или правым поддеревом
        if (del->parent == NULL){               // если удаляем корень
            if (del->left == NULL){
                del->right->parent = NULL;
                *head = del->right;
                free(del);
                return 1;
            }                               // ищем замену корню, им станет его непустое поддерево
            if (del->right == NULL){
                del->left->parent = NULL;
                *head = del->left;
                free(del);
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
            if (del->right == NULL){                // если у вершины нет правого поддерева
                if (del->parent->right == del){     // делаем все то же самое
                    del->parent->right = del->left;
                    del->left->parent = del->parent;
                }
                if (del->parent->left == del){
                    del->parent->left = del->left;
                    del->left->parent = del->parent;
                }
            }
            free(del);
        }
        return 1;
    }                             // теперь рассмотрим случай удаления вершины, у к-ой оба поддерева не пусты
    BinaryTreeNode *ptr = findNext(del);  // ищем последующую вершину
    if (ptr->left != NULL){             // если левое поддерево последующего элемента не пусто
        if (ptr->parent->left == ptr){ // формируем новые связи между потомком и родителем послед. вершины
            ptr->parent->left = ptr->left;
        }
        if (ptr->parent->right == ptr){
            ptr->parent->right = ptr->left;
        }
    }
    if (ptr->right != NULL){           // аналогично если правое поддерево последующего элемента не пусто
        if (ptr->parent->left == ptr){
            ptr->parent->left = ptr->right;
        }
        if (ptr->parent->right == ptr){
            ptr->parent->right = ptr->right;
        }
    }
    if (del->parent != NULL){               // если удаляем НЕ корень
        ptr->parent = del->parent;     // родителем последюущей вершины становится родитель удаляемой вершины
        if (del->parent->right == del){ // последующая вершина становится потомком
            del->parent->right = ptr;   // родителя удаляемой вершины
        }
        if (del->parent->left == del){
            del->parent->left = ptr;
        }
    }
    
    if (del->right != NULL && del->right != ptr){   // потомками последующей вершины становятся
        ptr->right = del->right;                    // потомки удаляемой вершины
    }
    if (del->left != NULL && del->left != ptr){
        ptr->left = del->left;
    }
    if (del->left->parent != ptr){      // последующая вершина становится родителем для левого и правого
        if (del->left != ptr)           // поддерева удаляемой вершины
            del->left->parent = ptr;
    }
    if (del->right->parent != ptr){
        if (del->right != ptr)
            del->right->parent = ptr;
    }
    if (del->parent == NULL){               // если удаляем корень
        if (ptr->parent->right == ptr){     // обрываем старые связи последующей вершины
            ptr->parent->right = NULL;
        }
        if (ptr->parent->left == ptr){
            ptr->parent->left = NULL;
        }
        ptr->parent = NULL;
        *node = ptr;                    // и делаем последующую вершину корнем
    }
    free(del);
    return 1;
}

void printNode (BinaryTreeNode *node){  // печать узла дерева
    printf("[%d]: ", node->key);
    printf("%d", node->data);
    printf("\n");
}

void Show (BinaryTreeNode *ptr, int lvl, void (*otherFunction)(BinaryTreeNode*)){
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

int addStringsGenerate (BinaryTreeNode **head, int count){
    srand(time(0));
    int key, val;
    for (int i = 0; i < count; i++){    // создаем count рандомных узлов и добавляем их в дерево
        key = 1 + rand() % 1000000;
        val = 1 + rand() % 1000000;
        add (head, key, val);
    }
    return 1;
}

void freeNode(BinaryTreeNode *node) {                 // очистка памяти, занимаемой узлом
    free(node);
}

void fileWriteNode(BinaryTreeNode *node, FILE *f){    // запись узла в файл
    fprintf(f, "[%d]: %d\n", node->key, node->data);
}

void bypass (BinaryTreeNode *node, FILE *f){    // прямой обход для сохранения дерева в файл
    if (node != NULL){
        fileWriteNode(node, f);
        bypass(node->left, f);
        bypass(node->right, f);
    }
}

int Load(BinaryTreeNode **head){
    int key, val;
    FILE *f;
    if (f = fopen(FILENAME, "r")){
        while (!feof(f)){
            fscanf(f, "[%d]: %d\n", &key, &val);
            add(head, key, val);
        }
        fclose(f);
    }
    printf("Прочитано из файла: \n");
    Show(*head, 0, NULL);
    return 1;
}

void getInt(int *a){
    int n = 0, flag = 0;
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
