#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 100

typedef struct
{
    char *name;
    int id;
}Sommet;

typedef  struct
{
    int **matrice;
    int ordre;
    Sommet *sommets;
}Graphe;

Sommet init_sommet(int id, char *name);

void init_graphe(int ordre, Graphe *graphe);

void load_graphe(char *file_name, Graphe *graphe);

void print_graphe(Graphe graphe);

void free_graphe(Graphe *graphe);

#endif