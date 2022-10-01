#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MSIZE1 = 20;


struct Item {   // элемент таблицы
    int num;    // число
    char *info1;    // строчка 1
    char *info2;    // строчка 2
    int release;    // версия элемента
    struct Item *next;  // указатель на следующий элемент
};


struct KeySpace1{   // первое пространство ключей
    int key;    // ключ
    int par;    // родительский ключ
    struct Item *info;  // указатель на информацию
};

struct Table{   // структура таблицы
    struct KeySpace1 *ks1;  // указатель на первое пространство ключей
    int msize1; // максимальный размер первого пространства ключей
    int csize1; // текущий размер
};

char* getStr()
{
    char *ptr = (char *)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do{
        n = scanf("%80[^\n]", buf); // n = scanf_s ("%80[^\n]", buf, 81);
        if(n < 0){
            free(ptr);
            ptr = NULL;
            continue;
        }
        if(n == 0)
            scanf("%*c");
        else {
            len += strlen(buf);
            ptr = (char *) realloc(ptr, len + 1);
            strcat(ptr, buf);
            
        }
    } while(n > 0);
    return ptr;
}

void getInt(int* a)
{
    int n = 0, flag=0;
    do
    {
        if (flag == 1){
            printf("Ошибка. Повторите ввод: ");
            scanf("%*[^\n]");
        }
        n = scanf("%d", a);
        if (*a == -1)
            n = -1;
        flag = 1;
    } while (n <= 0);
    scanf("%*c");
}



const char *msgs[] = {"0. Выйти", "1. Добавить элемент по ключу", "2. Поиск по родительскому ключу", "3. Поиск элемента по ключу", "4. Удалить элемент по ключу", "5. Печать таблицы", "6. Сгенерировать случайную таблицу"};

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
        puts(msgs[i]);
        printf("\nMake your choice: —> ");
        getInt(&rc);
    } while(rc < 0 || rc >= N);
    return rc;
}

int D_Show(struct Table *table){
    if (table->csize1 == 0){
        printf ("\nНевозможно вывести таблицу\n");
        return -1;
    }
    printf("Индекс     Ключ     Род. ключ   Версия     Число     Инфо1      Инфо2\n");
    for (int i = 0; i < table->csize1; i++){
        printf ("[%d] %10d %10d %10d %10d %10s %10s \n\n", i, table->ks1[i].key,    table->ks1[i].par, table->ks1[i].info->release, table->ks1[i].info->num, table->ks1[i].info->info1, table->ks1[i].info->info2);
        if (table->ks1[i].info->next != NULL){
            struct Item *ptr = table->ks1[i].info;
            
            while (ptr->next != NULL){
                ptr = ptr->next;
                printf ("%36d %10d %10s %10s\n\n", ptr->release, ptr->num, ptr->info1, ptr->info2);
            }
        }
    }
    return 1;
}

int find_ks1(struct Table *table, int key){
    for (int i = 0; i < table->csize1; i++){
        if (table->ks1[i].key != 0){
            if (table->ks1[i].key == key){
                return i;
            }
        }
        else {
            break;
        }
    }
    return -1;
}

int Add (struct Table *table, struct Item *it, int key){
    int index = find_ks1(table, key);
    int count = 1;
    if (table->csize1 == 0){
        table->ks1[table->csize1].par = 0;
    }
    else {
        table->ks1[table->csize1].par = table->ks1[rand() % table->csize1].key;
    }
    if (index != -1){
        struct Item *ptr = table->ks1[index].info;
        count++;
        while (ptr->next != NULL){
            ptr = ptr->next;
            count++;
        }
        ptr->next = it;
    }
    else {
        table->ks1[table->csize1].key = key;
        table->ks1[table->csize1].info = it;
        table->csize1++;
    }
    
    it->release = count;
    return 1;
}


int D_Add(struct Table *table){
    if (table->csize1 >= table->msize1){
        printf ("Таблица заполнена");
        return -1;
    }
    int key;
    printf ("Введите номер ключа: ");
    getInt(&key);
    
    struct Item *it = (struct Item*)calloc(1, sizeof(struct Item));
    printf("Введите число: ");
    getInt(&it->num);
    printf ("Введите первую строчку: ");
    it->info1 = getStr();
    printf ("Введите вторую строчку: ");
    it->info2 = getStr();
    Add (table, it, key);
    
    printf ("\nЭлемент успешно добавлен\n");
    return 1;
}

int Find_Key (struct Table *table){
    int key;
    printf("Введите номер ключа: ");
    getInt(&key);
    int i = find_ks1(table, key);
    if (i != -1){
        printf ("\nНайден элемент с информацией: \n");
        printf("Индекс     Ключ     Род. ключ   Версия     Число     Инфо1      Инфо2\n");
        printf ("[%d] %10d %10d %10d %10d %10s %10s \n\n", i, table->ks1[i].key,    table->ks1[i].par, table->ks1[i].info->release, table->ks1[i].info->num, table->ks1[i].info->info1, table->ks1[i].info->info2);
    }
    if (table->ks1[i].info->next != NULL){
        struct Item *ptr = table->ks1[i].info;
        
        while (ptr->next != NULL){
            ptr = ptr->next;
            printf ("%36d %10d %10s %10s\n\n", ptr->release, ptr->num, ptr->info1, ptr->info2);
        }
    }
    else {
        printf ("\nЭлемента с ключом %d нет в таблице\n", key);
    }
    return 1;
}

int D_Find(struct Table *table){
    int parent;
    printf("Введите номер родительского ключа: ");
    getInt(&parent);
    struct Table table1 = {(struct KeySpace1*)calloc(MSIZE1, sizeof(struct KeySpace1)), MSIZE1, 0};
    for (int i = 0; i < table->csize1; i++){
        if (table->ks1[i].par == parent){
            Add(&table1, table->ks1[i].info, table->ks1[i].key);
        }
    }
    D_Show(&table1);
    return 1;
}
int D_Delete(struct Table *table){
    int key;
    printf ("Введите ключ удаляемого элемента: ");
    getInt(&key);
    int search = find_ks1(table, key);
    if (search == -1){
        printf ("\nЭлемента с ключом %d нет в таблице\n", key);
        return  -1;
    }
    free (table->ks1[search].info->info1);
    free (table->ks1[search].info->info2);
    free (table->ks1[search].info);
    table->ks1[search].info = table->ks1[table->csize1 - 1].info;
    table->ks1[search].key = table->ks1[table->csize1 - 1].key;
    table->ks1[search].par = table->ks1[table->csize1 - 1].par;
    table->ks1[table->csize1 - 1].info = NULL;
    table->csize1 --;
    for (int i = 0; i < table->csize1; i++){
        if (table->ks1[i].par == key){
            table->ks1[i].par = 0;
        }
    }
    printf ("\nЭлемент успешно удалён\n");
    return 1;
}


int D_Generate(struct Table *table){
    srand(time(0));
    int number;
    printf ("Сколько элементов вы хотите сгенерировать?\n");
    do{
        getInt(&number);
        if (number <= 0 || number > (table->msize1 - table->csize1)){
            printf ("Ошибка. Повторите ввод: ");
        }
    } while (number <= 0 || number > (table->msize1 - table->csize1));
    int key;
    for (int i = 0; i < number; i++){
        key = rand() % 100 + 1;
        if (find_ks1(table, key) != -1){
            i--;
        }
        else{
            if (table->csize1 < table->msize1){
                struct Item *it = (struct Item*)calloc(1, sizeof(struct Item));
                it->num = rand() % 1000;
                int randomCount = rand() % 5 + 4;
                it->info1 = (char*)calloc(randomCount+1, sizeof(char));
                it->info2 =  (char*)calloc(randomCount+1, sizeof(char));
                for (int j = 0; j < randomCount; j++){
                    it->info1[j] = 'a'  + rand() % 10;
                    it->info2[j] = 'e'  + rand() % 25;
                }
                it->info1[randomCount] = '\0';
                it->info2[randomCount] = '\0';
                it->release = 1;
                Add(table, it, key);
            }
            else {
                return -1;
            }
        }
    }
    printf ("\nТаблица успешно сгенерирована\n");
    return 1;
}


int (*fptr[])(struct Table *) = {NULL, D_Add, D_Find, Find_Key , D_Delete, D_Show, D_Generate};

void delTable (struct Table *table){
    for (int i = 0; i < table->csize1; i++){
        free (table->ks1[i].info->info1);
        free (table->ks1[i].info->info2);
        free (table->ks1[i].info);
    }
    free (table->ks1);
}

int main() {
    struct Table table = {(struct KeySpace1*)calloc(MSIZE1, sizeof(struct KeySpace1)), MSIZE1, 0};
    int rc;
    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&table))
            
            
            
            break;
    printf("\nThat's all folks!\n");
    delTable (&table);
    
    
    return 0;
}


