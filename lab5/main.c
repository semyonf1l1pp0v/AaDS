#include "graph.h"


int D_Print (struct Graph* graph){
    printGraph(graph);
    return 1;
}

int D_AddVertex (struct Graph* graph){
    addVertex(graph, createVertex(graph));
    printf ("Успешно добавлено\n");
    return 1;
}

int D_AddEdge (struct Graph* graph){
    int name1, name2, i;
    printf ("Между какими вершинами добавить связь?\n");
    printf ("Введите первую вершину: ");
    getInt(&name1);
    printf ("Введите вторую вершину: ");
    getInt(&name2);
    if (addEdge(findName(graph, name1, &i), findName(graph, name2, &i)))
        printf ("Успешно добавлено\n");
    else {
        printf("Такая связь уже есть или такой пары вершин нет\n");
    }
    return 1;
}

int D_DeleteEdge (struct Graph* graph){
    int name1, name2, i;
    printf ("Между какими вершинами удалить связь?\n");
    printf ("Введите первую вершину: ");
    getInt(&name1);
    printf ("Введите вторую вершину: ");
    getInt(&name2);
    if (delEdge(findName(graph, name1, &i), findName(graph, name2, &i)))
        printf ("Успешно удалено\n");
    else {
        printf("Такой связи нет или нет такой пары вершин\n");
    }
    return 1;
}

int D_DeleteVertex (struct Graph* graph){
    int name1, i;
    printf ("Какую вершину удалить?\n");
    getInt(&name1);
    if (delVertex(graph, findName(graph, name1, &i)))
        printf ("Успешно удалено\n");
    else {
        printf("Такой вершины нет\n");
    }
    return 1;
}

int D_BFS (struct Graph* graph){
    int name1, name2;
    printf ("Между какими вершинами проверить достижимость?\n");
    printf ("Введите первую вершину: ");
    getInt(&name1);
    printf ("Введите вторую вершину: ");
    getInt(&name2);
    int index1, index2;
    findName(graph, name1, &index1);
    findName(graph, name2, &index2);
    BFS(graph, index1, index2, NULL, &name1);
    return 1;
}

int D_Decomp(struct Graph* graph){
    decomposition(graph);
    return 1;
}

int D_Deikstra (struct Graph* graph){
    int name1, name2, i;
    printf ("Между какими вершинами искать расстояние?\n");
    printf ("Введите первую вершину: ");
    getInt(&name1);
    printf ("Введите вторую вершину: ");
    getInt(&name2);
    deikstra(graph, findName(graph, name1, &i), findName(graph, name2, &i));
    return 1;
}

int D_Write(struct Graph* graph){
    FILE *f = fopen(FILENAME, "w");
    saveGraph(graph, f);
    printf("Успешно сохранено в файл\n");
    fclose(f);
    return 1;
}

const char *msgs[] = {"Выйти", "Добавить вершину", "Добавить связь", "Удалить вершину", "Удалить связь", "Проверить достижимость", "Печать графа", "Разложить на компоненты связности", "Минимальное расстояние между вершинами", "Сохранить в файл"};

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
int (*fptr[])(struct Graph*) = {NULL, D_AddVertex, D_AddEdge, D_DeleteVertex, D_DeleteEdge, D_BFS, D_Print, D_Decomp, D_Deikstra, D_Write};


int main(){
    struct Graph graph = {{0}, 0};
    readGraph(&graph);
    int rc;
    while(rc = dialog(msgs, NMsgs))
        if(!fptr[rc](&graph))
            break;
    printf("\nThat's all folks!\n");
    return 0;
}

