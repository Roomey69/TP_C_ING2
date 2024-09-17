#include "../header.h"

Sommet init_sommet(int id, char *name){
    Sommet sommet;
    sommet.id = id;
    sommet.name = (char *)malloc(strlen(name)+1);
    strcpy(sommet.name, name);
    return sommet;
}

void init_graphe(int ordre, Graphe *graphe){
    graphe->ordre = ordre;
    graphe->matrice = (int **)malloc(ordre * sizeof(int *));
    for (int i = 0; i < ordre; i++) {
            graphe->matrice[i] = (int *)calloc(ordre, sizeof(int));
        }
    graphe->sommets = (Sommet *)malloc(ordre * sizeof(Sommet));
}

void load_graphe(char *file, Graphe *graphe) {
    char path[MAX_CHAR];
    sprintf(path, "../%s.txt", file);

    FILE* fichier = fopen(path, "r");

    int ordre = 0;
    fscanf(fichier, "%d", &ordre);

    init_graphe(ordre, graphe);

    int i = 0;
    char buffer[MAX_CHAR];
    while(!feof(fichier) && (i < graphe->ordre)) {
        fscanf(fichier, "%100s", buffer);
        graphe->sommets[i] = init_sommet(i+1, buffer);
        i++;
    }

    i = 0;
    while(!feof(fichier) && (i < graphe->ordre)) {
        for(int j = 0; j < graphe->ordre; j++) {
            fscanf(fichier, "%d ", &(graphe->matrice[i][j]));
        }
        i++;
    }

    fclose(fichier);
}

void print_graphe(Graphe graphe) {
    printf("\n----------------------------\n");
    for(int i = 0; i < graphe.ordre; i++) {
        for(int j = 0; j < graphe.ordre; j++) {
            if(graphe.matrice[i][j] != 0) {
                printf("%s influence %s\n", graphe.sommets[i].name, graphe.sommets[j].name);
            }
        }
    }
    printf("----------------------------\n");
}

void free_graphe(Graphe *graphe) {
    for (int i = 0; i < graphe->ordre; i++) {
        free(graphe->matrice[i]);
    }
    free(graphe->matrice);
    for (int i = 0; i < graphe->ordre; i++) {
        free(graphe->sommets[i].name);
    }
    free(graphe->sommets);
    free(graphe);
}