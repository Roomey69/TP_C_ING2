#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

Graphe *init_graphe(int ordre);
void init_arc(Graphe* g, char src, char dest, int poids);
Graphe *load_graphe(char *file_name);
void free_graphe(Graphe* g);
void afficher_graphe(Graphe* g);
void dijkstra(Graphe* g, char src, char dest);