#ifndef graph_h
#define graph_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MSIZE 10
#define FILENAME "GRAPH.TXT"

struct Vertex{                      // вершина графа
    int x, y;                       // координаты
    int name;                       // имя (число)
    struct Adj *adjectives;         // указатель на список смежности данной вершины
};

struct Graph{                       // граф
    struct Vertex *vert [MSIZE];    // массив вершин графа
    int csize;                      // текущий размер графа
};

struct Adj{                         // список смежностей
    struct Vertex *vertex;          // указатель на вершину
    struct Adj *next;               // указатель на следующий элемент списка
};

void addVertex (struct Graph *graph, struct Vertex *vertex);
bool addEdge (struct Vertex *vertex1, struct Vertex *vertex2);
bool delVertex (struct Graph *graph, struct Vertex *vertex);
bool delEdge (struct Vertex *vertex1, struct Vertex *vertex2);
void printVertex (struct Vertex *vertex);
void printGraph (struct Graph *graph);
void BFS (struct Graph* graph, int number1, int number2, int *color, int *colorSize);
int deikstra (struct Graph *graph, struct Vertex *vertex1, struct Vertex *vertex2);
void decomposition (struct Graph *graph);
void saveGraph (struct Graph *graph, FILE *f);
void readGraph (struct Graph *graph);
struct Vertex *createVertex (struct Graph *graph);
struct Vertex *findName (struct Graph *graph, int name, int *i);
struct Vertex *findCoord (struct Graph *graph, int x, int y);
void getInt (int *a);



#endif /* graph_h */
