#include "graph.h"

struct Vertex *createVertex (struct Graph *graph){
    int x, y, name, flag = 0, i;
    struct Vertex *new = (struct Vertex*)calloc(1, sizeof(struct Vertex));
    do{
        if (flag)
            printf("Такие координаты уже есть\n");
        printf ("Введите координату Х: ");
        getInt(&x);
        printf ("Введите координату Y: ");
        getInt(&y);
        flag = 1;
    } while (findCoord(graph, x, y));
    flag = 0;
    do{
        if (flag)
            printf("Такое имя уже есть\n");
        printf ("Введите имя вершины (число): ");
        getInt(&name);
        flag = 1;
    } while (findName(graph, name, &i));
    new->x = x;
    new->y = y;
    new->name = name;
    return new;
}

void addVertex (struct Graph *graph, struct Vertex *vertex){
    graph->vert[graph->csize] = vertex;
    graph->csize++;
}

bool addEdge (struct Vertex *vertex1, struct Vertex *vertex2){
    if (vertex1 != NULL && vertex2 != NULL){
        struct Adj *ptr1 = vertex1->adjectives;
        struct Adj *ptr2 = vertex2->adjectives;
        struct Adj *new1;
        struct Adj *new2;
        if (ptr1 != NULL){
            if (ptr1->vertex == vertex2){
                return false;
            }
            while (ptr1->next != NULL){
                if (ptr1->vertex == vertex2){
                    return false;
                }
                ptr1 = ptr1->next;
            }
            new1 = (struct Adj*)calloc(1, sizeof(struct Adj));
            ptr1->next = new1;
            new1->vertex = vertex2;
        }
        else{
            new1 = (struct Adj*)calloc(1, sizeof(struct Adj));
            vertex1->adjectives = new1;
            new1->vertex = vertex2;
        }
        if (ptr2 != NULL){
            if (ptr2->vertex == vertex1){
                return false;
            }
            while (ptr2->next != NULL){
                if (ptr2->vertex == vertex1){
                    return false;
                }
                ptr2 = ptr2->next;
            }
            new2 = (struct Adj*)calloc(1, sizeof(struct Adj));
            ptr2->next = new2;
            new2->vertex = vertex1;
        }
        else{
            new2 = (struct Adj*)calloc(1, sizeof(struct Adj));
            vertex2->adjectives = new2;
            new2->vertex = vertex1;
        }
        return true;
        
    }
    return false;
}

bool delEdge (struct Vertex *vertex1, struct Vertex *vertex2){
    if (vertex1 == vertex2){
        vertex1->adjectives = NULL;
        vertex2->adjectives = NULL;
        return true;
    }
    if (vertex1 != NULL && vertex2 != NULL){
        struct Adj *ptr1 = vertex1->adjectives;
        struct Adj *ptr2 = vertex2->adjectives;
        struct Adj *del;
        if (ptr1->vertex != vertex2){
            while (ptr1->next != NULL){
                if (ptr1->next->vertex == vertex2){
                    break;
                }
                ptr1 = ptr1->next;
            }
            if (ptr1->next == NULL)
                return false;
            del = ptr1->next;
            ptr1->next = del->next;
            free(del);
        }
        else {
            del = vertex1->adjectives;
            vertex1->adjectives = vertex1->adjectives->next;
            free(del);
        }
        if (ptr2->vertex != vertex1){
            while (ptr2->next != NULL){
                if (ptr2->next->vertex == vertex1){
                    break;
                }
                ptr2 = ptr2->next;
            }
            if (ptr2->next == NULL)
                return false;
            del = ptr2->next;
            ptr2->next = del->next;
            free(del);
        }
        else {
            del = vertex2->adjectives;
            vertex2->adjectives = vertex2->adjectives->next;
            free(del);
        }
        return true;
        
    }
    return false;
}

bool delVertex (struct Graph *graph, struct Vertex *vertex){
    int index;
    if (vertex){
        findName(graph, vertex->name, &index);
            while (graph->vert[index]->adjectives != NULL){
                delEdge(graph->vert[index]->adjectives->vertex, vertex);
            }
            free(graph->vert[index]);
            graph->csize--;
            graph->vert[index] = graph->vert[graph->csize];
            return true;
    }
    return false;
}


void BFS(struct Graph* graph, int number1, int number2, int *color, int *colorSize){
    int flag = 0;
    if (color == NULL){         // массив цветов (серый, черный, белый)
        flag = 1;           // белый - вершина не обработана, серый - началась обработка, черный - обработана
        color = (int*)calloc(graph->csize, sizeof(int));
    }
    struct Vertex** Q = (struct Vertex**)calloc(graph->csize, sizeof(struct Vertex));   // очередь
    color[number1] = 1;                     // 1 - серый, -1 - черный, 0 - белый
    Q[0] = graph->vert[number1];            // закидываем первую вершину, начинаем её обработку
    
    while (Q[0] != NULL) {                  // пока очередь не пуста
        struct Vertex* u = Q[0];            // указатель на нашу вершину
        Q[0] = NULL;                        // достаем вершину из очереди
        for (int i = 1; Q[i] != NULL; Q[i - 1] = Q[i], Q[i] = NULL, i++);   //сдвиг элементов в Q
    
        if (*colorSize != 0) {              // выводим вершину (colorsize - кол-во обработанных вершин)
            printf(" %d\n", u->name);
            if (number2 != -1)
                if (u->name == graph->vert[number2]->name){
                    printf ("[Yes]\n");
                    break;
                }
        }
        
        struct Adj* ptr = u->adjectives;      // указатель на список смежных вершин для каждой вершины
        for (int i = 0; ptr!=NULL; i++, ptr = ptr->next){     // начинаем обработку всех смежных вершин
            int ind;
            findName(graph, ptr->vertex->name, &ind);
            if (color[ind] == 0) {                     // если вершина не обработана - добавляем ее в очередь
                color[ind] = 1;
                int l = 0;                             // индекс по которому будем добавлять
                for (; Q[l] != NULL; l++);             //поиск свободного места в Q
                Q[l] = ptr->vertex;                      // добавляем вершину в конец очереди
            }
        }
        int index;
        findName(graph, u->name, &index);
        color[index] = -1;                             // вершина обработана
        (*colorSize)++;                                // одна вершина обработана
    }
    if (flag)                                          // если память была выделена то мы ее очищаем
        free(color);
    (*colorSize)--;
    free(Q);
}

void decomposition (struct Graph *graph){
    int m = 1;
    int finished [graph->csize];
    for (int i = 0; i < graph->csize; finished[i] = 0, i++);
    int *color = (int*)calloc(graph->csize, sizeof(int));
    int colorSize = 0, flag = 0 ;
    while(flag == 0){
        for (int j = 0; j < graph->csize; j++){
            if (finished[j] == 0){
                printf ("Компонента %d:\n %d\n", m, graph->vert[j]->name);
                BFS(graph, j, -1, color, &colorSize);
                for (int i = 0; i < graph->csize; i++){
                    if (color[i] == -1)
                        finished[i] = 1;
                    color[i] = 0;
                }
                colorSize = 0;
                m++;
            }
            flag = 1;
            for (int k = 0; k < graph->csize; k++){
                if (finished[k] == 0)
                    flag = 0;
            }
        }
    }
}

void printVertex (struct Vertex *vertex){
    printf ("%d: {%d, %d}\n", vertex->name, vertex->x, vertex->y);
}

void printGraph (struct Graph *graph){
    printf ("В графе есть вершины:\n");
    for (int i = 0; i < graph->csize; i++){
        printVertex(graph->vert[i]);
        struct Adj *ptr = graph->vert[i]->adjectives;
        while (ptr != NULL){
            printf ("       ");
            printVertex(ptr->vertex);
            ptr = ptr->next;
        }
    }
}

void saveGraph (struct Graph *graph, FILE *f){
    fprintf(f, "%d\n", graph->csize);
    for (int i = 0; i < graph->csize; i++){
        fprintf (f, "%d %d %d ", graph->vert[i]->name, graph->vert[i]->x, graph->vert[i]->y);
        fprintf(f, "\n");
    }
    for (int i = 0; i < graph->csize; i++){
        struct Adj *ptr = graph->vert[i]->adjectives;
        int k = 0;
        while (ptr != NULL){
            k++;
            ptr = ptr->next;
        }
        ptr = graph->vert[i]->adjectives;
        fprintf(f, "%d ", k);
        while (ptr != NULL){
            fprintf (f, "%d ", ptr->vertex->name);
            ptr = ptr->next;
        }
        fprintf(f, "\n");
    }
}


void readGraph (struct Graph *graph){
    int csize;
    FILE *f = fopen(FILENAME, "r");
    if (f){
        fscanf(f, "%d", &csize);
        for (int i = 0; i < csize; i++){
            struct Vertex *ptr = (struct Vertex*)calloc(1, sizeof(struct Vertex));
            fscanf(f, "%d %d %d ", &ptr->name, &ptr->x, &ptr->y);
            addVertex(graph, ptr);
        }
        
        for (int i = 0; i < graph->csize; i++){
            int k = 0;
            fscanf(f, "%d ", &k);
            for (int j = 0; j < k; j++){
                int name;
                fscanf(f, "%d ", &name);
                addEdge(graph->vert[i], findName(graph, name, &csize));
            }
        }
        fclose(f);
    }
    printf ("Прочитано из файла: \n");
    printGraph(graph);
}

int deikstra(struct Graph *graph, struct Vertex *vertex1, struct Vertex *vertex2) {
    int this, other;
    
    int dist[graph->csize];
    int finished[graph->csize];
    struct Vertex *Q[graph->csize];
    
    for (int i = 0; i < graph->csize; finished[i] = 0, dist[i] = INT_MAX, Q[i] = NULL, i++);
    
    findName(graph, vertex1->name, &this);
    findName(graph, vertex2->name, &other);
    
    dist[this] = 0;
    finished[this] = 1;
    
    Q[0] = vertex1;
    
    while (Q[0] != NULL) {
        struct Vertex *u = Q[0];            // указатель на нашу вершину
        findName(graph, Q[0]->name, &this);
        Q[0] = NULL;
        for (int i = 1; Q[i] != NULL; Q[i - 1] = Q[i], Q[i] = NULL, i++);   //сдвиг элементов в Q
        
        struct Adj *ptr = u->adjectives;      // указатель на список смежных вершин для каждой вершины
        for (int i = 0; ptr != NULL; i++, ptr = ptr->next) {     // начинаем обработку всех смежных вершин
            int ind;
            findName(graph, ptr->vertex->name, &ind);
            if (finished[ind] == 0) {                  // если вершина не обработана - добавляем ее в очередь
                finished[ind] = 1;
                
                int l = 0;                             // индекс по которому будем добавлять
                for (; Q[l] != NULL; l++);             //поиск свободного места в Q
                Q[l] = ptr->vertex;                      // добавляем вершину в конец очереди
                
                if (dist[this] + 1 < dist[ind])
                    dist[ind] = dist[this] + 1;
            }
        }
        
        int index;
        findName(graph, u->name, &index);
        finished[index] = -1;
    }
    printf("Dist:\n");
    for(int i=0; i<graph->csize; printf("name:%d \n dist: %d \n\n", graph->vert[i]->name, dist[i]), i++);
    
    printf("Dist to vertex2 = %d", dist[other]);
    return 1;
}

struct Vertex* findCoord (struct Graph *graph, int x, int y){
    for (int i = 0; i < graph->csize; i++){
        if (graph->vert[i]->x == x && graph->vert[i]->y == y)
            return graph->vert[i];
    }
    return NULL;
}

struct Vertex *findName (struct Graph *graph, int name, int *i){
    for (*i = 0; *i < graph->csize; (*i)++){
        if (graph->vert[*i]->name == name)
            return graph->vert[*i];
    }
    *i = -1;
    return NULL;
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
