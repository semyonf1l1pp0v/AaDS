#include "kd-tree.h"

int D_Add (KDTreeNode **head){
    int x, y, value;
    
    do {
        printf("Введите х: ");
        getInt(&x);
        if (x < X_MIN || x > X_MAX){
            printf("\nНедопустимое значение координаты\n");
            return -1;
        }
    } while (x < X_MIN || x > X_MAX);
    
    do {
        printf("Введите y: ");
        getInt(&y);
        if (y < Y_MIN || y > Y_MAX){
            printf("\nНедопустимое значение координаты\n");
            return -1;
        }
    } while (y < Y_MIN || y > Y_MAX);
    
    printf("Введите значение: ");
    getInt(&value);
    if (add(head, x, y, value)){
        printf("\nУспешно добавлено\n");
    }
    else {
        printf("\nВ координатах [%d, %d] уже имеется запись\n", x, y);
    }
    return 1;
}

int D_Find (KDTreeNode **head){
    int x, y;
    printf("Введите х: ");
    getInt(&x);
    printf("Введите y: ");
    getInt(&y);
    KDTreeNode *node = find(*head, x, y, 0);
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

int D_Show (KDTreeNode** head){
    int lvl = 0;
    printf("\nТекущее состояние дерева:\n");
    Show(*head, lvl, NULL);
    return 1;
}

int Time (KDTreeNode** head){
    int lvl = 0;
    srand(time(NULL));
    int count = 100000;
    int found = count * 2;
    int numberOfElements = 0;
    clock_t first, second;

    int found_arr[found][2];
    KDTreeNode* ptr = NULL;
    
    for (int i = 1; i <= 10; i++){
        addStringsGenerate(&ptr, count);
        
        for (int j = 0; j < (found / 10 * i); j++){
            found_arr[j][0] = X_MIN + rand() % (2 * X_MAX);
            found_arr[j][1] = Y_MIN + rand() % (2 * Y_MAX);
        }
        numberOfElements = 0;
        first = clock();
        for (int k = 0; k < (found / 10 * i); k++){
            if (find (ptr, found_arr[k][0], found_arr[k][1], 0)!= NULL){
                numberOfElements++;
            }
        }
        second = clock();
        printf ("[%d] \n",i);
        printf ("Текущее количество вершин: %d\n", count * i);
        printf ("Количество найденных вершин: %d\n", numberOfElements);
        printf ("Время поиска: %lu\n", second - first);
    }
    Show (ptr, lvl, freeNode);
    return 1;
}

int D_Delete (KDTreeNode **head){
    int x, y;
    printf("Введите х: ");
    getInt(&x);
    printf("Введите y: ");
    getInt(&y);
    if (!del(head, x, y)){
        printf("\nЭлемент с координатами [%d, %d] не найден\n", x, y);
        return -1;
    }
    printf("\nУспешно удалено\n");
    return 1;
}

int D_Write(KDTreeNode** head){
    FILE *f = fopen(FILENAME, "w");
    bypass(*head, f);
    printf ("\nУспешно сохранено в файл\n");
    fclose(f);
    return 1;
}

const char *msgs[] = {"Выйти", "Таймирование", "Добавить элемент", "Найти элемент", "Удалить элемент", "Печать дерева", "Сохранить в файл"};

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
int (*fptr[])(struct KDTreeNode** ) = {NULL, Time, D_Add, D_Find, D_Delete, D_Show, D_Write};


int main(){
     KDTreeNode *head = NULL;
     fileRead(&head);
    
    int rc;
    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&head))
            break;
    printf("\nThat's all folks!\n");
    return 0;
}
