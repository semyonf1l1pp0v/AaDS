#include "binarytree.h"

int D_Add (BinaryTreeNode **head){
    int key, value;
    printf("Введите ключ: ");
    getInt(&key);
    printf("Введите значение: ");
    getInt(&value);
    if (add(head, key, value)){
        printf("\nУспешно добавлено\n");
    }
    else {
        printf("\nЭлемент с ключом [%d] уже есть\n", key);
    }
    return 1;
}

int D_Find (BinaryTreeNode **head){
    int key;
    printf("\nВведите ключ: ");
    getInt(&key);
    BinaryTreeNode *node = find(*head, key);
    if (!node){
        printf("\nУзла с таким ключом нет\n");
        return -1;
    }
    else {
        printf ("\nНайденный элемент: ");
        printNode(node);
    }
    return 1;
}

int D_FindMin (BinaryTreeNode **head){
    BinaryTreeNode *node = findMin(*head);
    if (!node){
        printf("\nДерево пусто\n");
        return -1;
    }
    else {
        printf ("\nНайденный элемент: ");
        printNode(node);
    }
    return 1;
}

int D_Show (BinaryTreeNode **head){
    printf("\nТекущее состояние дерева:\n");
    Show(*head, 0, NULL);
    return 1;
}

int D_Time (BinaryTreeNode **head){
    srand(time(NULL));
    int count = 100000;
    int found = count * 2;
    int numberOfElements = 0;
    clock_t first, second;

    int found_arr[found];
    BinaryTreeNode *ptr = NULL;
    
    for (int i = 1; i <= 10; i++){
        addStringsGenerate(&ptr, count);
        
        for (int j = 0; j < (found / 10 * i); j++){
            found_arr[j] = 1 + rand() % 1000000;
        }
        numberOfElements = 0;
        first = clock();
        for (int k = 0; k < (found / 10 * i); k++){
            if (find (ptr, found_arr[k])!= NULL){
                numberOfElements++;
            }
        }
        second = clock();
        printf ("[%d] \n",i);
        printf ("Текущее количество вершин: %d\n", count * i);
        printf ("Количество найденных вершин: %d\n", numberOfElements);
        printf ("Время поиска: %lu\n", second - first);
    }
    Show (ptr, 0, freeNode);
    return 1;
}

int D_Delete (BinaryTreeNode **head){
    int key;
    printf("\nВведите ключ удаляемой строки: ");
    getInt (&key);
    if (!delete(head, key)) {      // поиск неуспешен
        printf("\nСтроки с таким ключом нет\n");
        return -1;
    }
    printf("\nУспешно удалено\n");
    return 1;
}

int D_Write(BinaryTreeNode **head){
    FILE *f = fopen(FILENAME, "w");
    bypass(*head, f);
    printf("\nУспешно сохранено в файл\n");
    fclose(f);
    return 1;
}

const char *msgs[] = {"Выйти", "Таймирование", "Добавить элемент", "Найти элемент", "Найти наименьший элемент", "Удалить элемент", "Печать дерева", "Сохранить в файл"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);


int dialog(const char *msgs[], int N)
{
    char *errmsg = "";
    int rc;
    int i;
    do{
        puts(errmsg);
        errmsg = "\nYou are wrong. Repeate, please!\n";
        for(i = 0; i < N; ++i)
        printf ("%d. %s\n", i, msgs[i]);
        printf("\nMake your choice: —> ");
        getInt(&rc);
    } while(rc < 0 || rc >= N);
    return rc;
}
int (*fptr[])(struct BinaryTreeNode**) = {NULL, D_Time, D_Add, D_Find, D_FindMin, D_Delete, D_Show, D_Write};


int main(){
    BinaryTreeNode *head = NULL;
    Load(&head);
    
    int rc;
    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&head))
            break;
    printf("\nThat's all folks!\n");
    Show (head, 0, freeNode);
    return 0;
}
