#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MSIZE1 = 20;

struct Item {   // элемент таблицы
    long int offset_num;     // смещение в файле (по отношению к началу файла)
    long int offset_str1;
    long int offset_str2;
    int len_str1;
    int len_str2;
    int release;    // версия элемента
    struct Item *next;  // указатель на следующий элемент
};


struct KeySpace1 {   // первое пространство ключей
    int key;    // ключ
    int par;    // родительский ключ
    struct Item *info;  // указатель на информацию
};

struct Table {   // структура таблицы
    struct KeySpace1 *ks1;  // указатель на первое пространство ключей
    int msize1; // максимальный размер первого пространства ключей
    int csize1; // текущий размер
    FILE *fd;   // дескриптор файла
};


char *getStr() {
    char *ptr = (char *) malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = scanf("%80[^\n]", buf); // n = scanf_s ("%80[^\n]", buf, 81);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0)
            scanf("%*c");
        else {
            len += strlen(buf);
            ptr = (char *) realloc(ptr, len + 1);
            strcat(ptr, buf);

        }
    } while (n > 0);
    return ptr;
}

void getInt(int *a) {
    int n = 0, flag = 0;
    do {
        if (flag == 1) {
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

const char *msgs[] = {"0. Выйти", "1. Добавить элемент по ключу", "2. Поиск по родительскому ключу",
                      "3. Поиск элемента по ключу", "4. Удалить элемент по ключу", "5. Печать таблицы",
                      "6. Сгенерировать случайную таблицу"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int dialog(const char *msgs[], int N) {


    char *errmsg = "";
    int rc;
    int i;
    do {
        puts(errmsg);
        errmsg = "\nYou are wrong. Repeate, please!\n";
        for (i = 0; i < N; ++i)
            puts(msgs[i]);
        printf("\nMake your choice: —> ");
        getInt(&rc);
    } while (rc < 0 || rc >= N);
    return rc;
}

int load(struct Table *table, char *filename) {
    table->fd = fopen(filename, "r+");
    if (table->fd == NULL) {
        return 0;
    }
    fread(&table->msize1, sizeof(int), 1, table->fd);
    fread(&table->csize1, sizeof(int), 1, table->fd);

    long int j;
    fread(&j, sizeof(long int), 1, table->fd);

    table->ks1 = (struct KeySpace1 *) calloc(table->msize1, sizeof(struct KeySpace1));
    fread(table->ks1, sizeof(struct KeySpace1), table->msize1, table->fd);
    fseek(table->fd, j, SEEK_SET);
    for (int i = 0; i < table->csize1; i++) {
        table->ks1[i].info = (struct Item *) calloc(1, sizeof(struct Item));
        struct Item *ptr = table->ks1[i].info;
        fread(table->ks1[i].info, sizeof(struct Item), 1, table->fd);
        while (ptr->next != NULL) {
            ptr->next = (struct Item *) calloc(1, sizeof(struct Item));
            ptr = ptr->next;
            fread(ptr, sizeof(struct Item), 1, table->fd);
        }
    }
    fseek(table->fd, j, SEEK_SET);
    return 1;
}

int create(struct Table *table, char *filename, int sz) {
    table->msize1 = sz;
    table->csize1 = 0;
    table->fd = fopen(filename, "w+");
    if (table->fd == NULL) {
        table->ks1 = NULL;
        return 0;
    }
    table->ks1 = (struct KeySpace1 *) calloc(table->msize1, sizeof(struct KeySpace1));
    fwrite(&table->msize1, sizeof(int), 1, table->fd);
    fwrite(&table->csize1, sizeof(int), 1, table->fd);
    long int j = 0;
    fwrite(&j, sizeof(long int), 1, table->fd);
    fwrite(table->ks1, sizeof(struct KeySpace1), table->msize1, table->fd);
    return 1;
}

int D_Load(struct Table *table) {
    char *fname = NULL;
    printf("Введите имя файла: ");
    fname = getStr();
    if (fname == NULL) {
        return 0;
    }
    if (load(table, fname) == 0) {
        printf("\nFile wasn't found\n");
        create(table, fname, MSIZE1);
    }
    free(fname);
    return 1;
}

int save(struct Table *table) {
    long int j = ftell(table->fd);

    fseek(table->fd, 4, SEEK_SET);
    fwrite(&table->csize1, sizeof(int), 1, table->fd);
    fwrite(&j, sizeof(long int), 1, table->fd);

    fwrite(table->ks1, sizeof(struct KeySpace1), table->msize1, table->fd);

    fseek(table->fd, j, SEEK_SET);
    for (int i = 0; i < table->csize1; i++) {
        struct Item *ptr = table->ks1[i].info;
        fwrite(table->ks1[i].info, sizeof(struct Item), 1, table->fd);
        while (ptr->next != NULL) {
            ptr = ptr->next;
            fwrite(ptr, sizeof(struct Item), 1, table->fd);
        }
    }

    fclose(table->fd);
    table->fd = NULL;
    return 1;
}


int findNum(struct Table *table, int i, int rel) {
    struct Item *ptr = table->ks1[i].info;
    int num = 0;
    if (i >= 0) {
        while (ptr->next != NULL) {
            if (ptr->release == rel) {
                break;
            }
            ptr = ptr->next;
        }
        long int j = ftell(table->fd);
        fseek(table->fd, ptr->offset_num, SEEK_SET);
        fread(&num, sizeof(int), 1, table->fd);
        fseek(table->fd, j, SEEK_SET);
    }
    return num;
}

char *findInfo1(struct Table *table, int i, int rel, char **info1) {
    struct Item *ptr = table->ks1[i].info;
    if (i >= 0) {
        while (ptr->next != NULL) {
            if (ptr->release == rel) {
                break;
            }
            ptr = ptr->next;
        }
        long int j = ftell(table->fd);
        *info1 = (char *)calloc(ptr->len_str1, sizeof (char));
        fseek(table->fd, ptr->offset_str1, SEEK_SET);
        fread(*info1, sizeof(char), ptr->len_str1, table->fd);
        fseek(table->fd, j, SEEK_SET);

    }
    return *info1;
}

char *findInfo2(struct Table *table, int i, int rel, char **info2) {
    struct Item *ptr = table->ks1[i].info;
    if (i >= 0) {
        while (ptr->next != NULL) {
            if (ptr->release == rel) {
                break;
            }
            ptr = ptr->next;
        }
        long int j = ftell(table->fd);
        *info2 = (char *) calloc(ptr->len_str2, sizeof (char));
        fseek(table->fd, ptr->offset_str2, SEEK_SET);
        fread(*info2, sizeof(char), ptr->len_str2, table->fd);
        fseek(table->fd, j, SEEK_SET);
    }
    return *info2;
}


int D_Show(struct Table *table) {
    if (table->csize1 == 0) {
        printf("\nНевозможно вывести таблицу\n");
        return -1;
    }
    char *info1, *info2;
    printf("Индекс     Ключ     Род. ключ   Версия     Число     Инфо1      Инфо2\n");
    for (int i = 0; i < table->csize1; i++) {
        printf("[%d] %10d %10d %10d %10d %10s %10s \n\n", i, table->ks1[i].key, table->ks1[i].par,
               table->ks1[i].info->release, findNum(table, i, table->ks1[i].info->release),
               findInfo1(table, i, table->ks1[i].info->release, &info1),
               findInfo2(table, i, table->ks1[i].info->release, &info2));
        free(info1);
        free(info2);
        struct Item *ptr = table->ks1[i].info;
        while (ptr->next != NULL) {
            ptr = ptr->next;
            printf("%36d %10d %10s %10s\n\n", ptr->release, findNum(table, i, ptr->release),
                   findInfo1(table, i, ptr->release, &info1), findInfo2(table, i, ptr->release, &info2));
            free(info1);
            free(info2);
        }
    }
    return 1;
}


int find_ks1(struct Table *table, int key) {
    for (int i = 0; i < table->csize1; i++) {
        if (table->ks1[i].key != 0) {
            if (table->ks1[i].key == key) {
                return i;
            }
        } else {
            break;
        }
    }
    return -1;
}

int Add(struct Table *table, struct Item *it, int key, char *info1, char *info2, int num) {

    it->len_str1 = strlen(info1) + 1;
    it->len_str2 = strlen(info2) + 1;
    it->offset_num = ftell(table->fd);
    fwrite(&num, sizeof(int), 1, table->fd);
    it->offset_str1 = it->offset_num + 4;
    fwrite(info1, sizeof(char), it->len_str1, table->fd);
    it->offset_str2 = ftell(table->fd);
    fwrite(info2, sizeof(char), it->len_str2, table->fd);

    if (table->csize1 == 0) {
        table->ks1[table->csize1].par = 0;
    } else {
        table->ks1[table->csize1].par = table->ks1[rand() % table->csize1].key;
    }

    int index = find_ks1(table, key);

    int count = 1;
    if (index != -1) {
        struct Item *ptr = table->ks1[index].info;
        while (ptr->next != NULL) {
            ptr = ptr->next;
            count++;
        }
        count++;
        ptr->next = it;
    } else {
        table->ks1[table->csize1].key = key;
        table->ks1[table->csize1].info = it;
        table->csize1++;
    }

    it->release = count;

    free(info1);
    free(info2);

    return 1;
}

int D_Add(struct Table *table) {
    int num;
    char *info1, *info2;
    if (table->csize1 >= table->msize1) {
        printf("Таблица заполнена");
        return -1;
    }
    int key;
    printf("Введите номер ключа: ");
    getInt(&key);
    struct Item *it = (struct Item *) calloc(1, sizeof(struct Item));
    printf("Введите число: ");
    getInt(&num);
    printf("Введите первую строчку: ");
    info1 = getStr();
    printf("Введите вторую строчку: ");
    info2 = getStr();
    Add(table, it, key, info1, info2, num);

    printf("\nЭлемент успешно добавлен\n");
    return 1;
}

int Find_Key(struct Table *table) {
    int key;
    printf("Введите номер ключа: ");
    getInt(&key);
    char *info1, *info2;
    for (int i = 0; i < table->csize1; i++) {
        if (table->ks1[i].key == key) {
            printf("\nНайден элемент с информацией\n");
            printf("Индекс     Ключ     Род. ключ   Версия     Число     Инфо1      Инфо2\n");
            printf("[%d] %10d %10d %10d %10d %10s %10s \n\n", i, table->ks1[i].key, table->ks1[i].par,
                   table->ks1[i].info->release, findNum(table, i, table->ks1[i].info->release),
                   findInfo1(table, i, table->ks1[i].info->release, &info1),
                   findInfo2(table, i, table->ks1[i].info->release, &info2));
            free(info1);
            free(info2);
            if (table->ks1[i].info->next != NULL) {
                struct Item *ptr = table->ks1[i].info;

                while (ptr->next != NULL) {
                    ptr = ptr->next;
                    printf("%36d %10d %10s %10s\n\n", ptr->release, findNum(table, i, ptr->release),
                           findInfo1(table, i, ptr->release, &info1), findInfo2(table, i, ptr->release, &info2));
                    free(info1);
                    free(info2);
                    
                }
            }
        }
    }
    return 1;
}
int D_Find(struct Table *table) {
    int parent;
    printf("Введите номер родительского ключа: ");
    getInt(&parent);
    char *info1, *info2;
    struct Table table1 = {(struct KeySpace1 *) calloc(MSIZE1, sizeof(struct KeySpace1)), MSIZE1, 0, table->fd};
    for (int i = 0; i < table->csize1; i++) {
        if (table->ks1[i].par == parent) {
            Add(&table1, table->ks1[i].info, table->ks1[i].key, findInfo1(table, i, table->ks1[i].info->release, &info1),
                findInfo2(table, i, table->ks1[i].info->release, &info2),
                findNum(table, i, table->ks1[i].info->release));
        }
    }
    D_Show(&table1);
    return 1;
}

int D_Delete(struct Table *table) {
    int key;
    printf("Введите ключ удаляемого элемента: ");
    getInt(&key);
    int search = find_ks1(table, key);
    if (search == -1) {
        printf("\nЭлемента с ключом %d нет в таблице\n", key);
        return -1;
    }
    free(table->ks1[search].info);
    table->ks1[search].info = table->ks1[table->csize1 - 1].info;
    table->ks1[search].key = table->ks1[table->csize1 - 1].key;
    table->ks1[search].par = table->ks1[table->csize1 - 1].par;
    table->ks1[table->csize1 - 1].info = NULL;
    table->csize1--;
    for (int i = 0; i < table->csize1; i++) {
        if (table->ks1[i].par == key) {
            table->ks1[i].par = 0;
        }
    }
    printf("\nЭлемент успешно удалён\n");
    return 1;
}

int D_Generate(struct Table *table) {
    char *info1, *info2;
    int num;
    srand(time(0));
    int number;
    printf("Сколько элементов вы хотите сгенерировать?\n");
    do {
        getInt(&number);
        if (number <= 0 || number > (table->msize1 - table->csize1)) {
            printf("Ошибка. Повторите ввод: ");
        }
    } while (number <= 0 || number > (table->msize1 - table->csize1));
    int key;
    for (int i = 0; i < number; i++) {
        key = rand() % 100 + 1;
        if (find_ks1(table, key) != -1) {
            i--;
        } else {
            if (table->csize1 < table->msize1) {
                struct Item *it = (struct Item *) calloc(1, sizeof(struct Item));
                num = rand() % 1000;
                int randomCount = rand() % 5 + 4;
                info1 = (char *) calloc(randomCount + 1, sizeof(char));
                info2 = (char *) calloc(randomCount + 1, sizeof(char));
                for (int j = 0; j < randomCount; j++) {
                    info1[j] = 'a' + rand() % 10;
                    info2[j] = 'e' + rand() % 25;
                }
                info1[randomCount] = '\0';
                info2[randomCount] = '\0';
                it->release = 1;
                Add(table, it, key, info1, info2, num);
            } else {
                return -1;
            }
        }
    }
    printf("\nТаблица успешно сгенерирована\n");
    return 1;
}

int (*fptr[])(struct Table *) = {NULL, D_Add, D_Find, Find_Key, D_Delete, D_Show, D_Generate};

void delTable(struct Table *table) {
    save(table);
    for (int i = 0; i < table->csize1; i++) {
        free(table->ks1[i].info);
    }
    free(table->ks1);
}

int main() {

    setlocale(LC_ALL, "Rus");

    struct Table table = {(struct KeySpace1 *) calloc(MSIZE1, sizeof(struct KeySpace1)), MSIZE1, 0};
    int rc;
    if (D_Load(&table) == 0)
        return 1;

    while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](&table))


            break;
    printf("\nThat's all folks!\n");
    delTable(&table);


    return 0;
}



