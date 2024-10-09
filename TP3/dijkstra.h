#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

enum{ MENU, GRAPHE ,DIJKSTRA, DIJKSTRA_HEAP, ASTAR, ASTAR_HEAP, LEAVE};

typedef struct Arc{
    char dest;
    int poids;
    struct Arc* next;
} Arc;

typedef struct {
    Arc* head;
} Sommet;

typedef struct {
    int ordre;
    Sommet* sommets;
} Graphe;

typedef struct {
    int sommet;
    int distance;
} Min_heap_node;

typedef struct {
    Min_heap_node *tab;
    int size;
    int capacity;
    int *pos;
} Min_heap;

Min_heap* createMin_heap(int capacity);
void swap_Min_heap_node(Min_heap_node* a, Min_heap_node* b);
void heapifyDown(Min_heap* heap, int idx);
void heapifyUp(Min_heap* heap, int idx);
void insertMin_heap(Min_heap* heap, int sommet, int distance);
Min_heap_node extractMin(Min_heap* heap);
void decreaseKey(Min_heap* heap, int sommet, int distance);
int isInMin_heap(Min_heap* heap, int sommet);
void dijkstra_heap(Graphe* g, char src, char dest);

Graphe *init_graphe(int ordre);
void init_arc(Graphe* g, char src, char dest, int poids);
Graphe *load_graphe(char *file_name);
void free_graphe(Graphe* g);
void afficher_graphe(Graphe* g);
void dijkstra(Graphe* g, char src, char dest);

int heuristique_tchebychev(int x1, int y1, int x2, int y2);
int heuristique_euclidienne(int x1, int y1, int x2, int y2);
int heuristique_manhattan(int x1, int y1, int x2, int y2);
void afficher_chemin_astar(int* parent, int current_index);
void astar(Graphe* g, char start, char goal, int (*heuristique)(int, int, int, int));
void astar_no_heap(Graphe* g, char start, char goal, int (*heuristique)(int, int, int, int));