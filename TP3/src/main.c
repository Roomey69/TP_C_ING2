#include "../dijkstra.h"

int main()
{
    char file_name[50];
    char src,dest;
    printf("Entrez le nom du fichier : "); scanf("%s",file_name);
    Graphe *g = load_graphe(file_name);
    afficher_graphe(g);
    printf("Entrez le sommet de depart : "); scanf(" %c",&src);
    printf("Entrez le sommet d'arrive : "); scanf(" %c",&dest);
    dijkstra(g,src,dest);
    free_graphe(g);
    return 0;
}

Graphe *init_graphe(int ordre){
    Graphe *g = (Graphe*)malloc(sizeof(Graphe));
    g->ordre = ordre;
    g->sommets = (Sommet*)malloc(sizeof(Sommet)*ordre);
    for (int i = 0; i < ordre; i++) {
        g->sommets[i].head = NULL;
    }
    return g;
}

void init_arc(Graphe* g, char src, char dest, int poids) {

    int src_index = src-'A';
    Arc* nouvel_arc = (Arc*) malloc(sizeof(Arc));

    nouvel_arc->dest = dest;
    nouvel_arc->poids = poids;
    nouvel_arc->next = g->sommets[src_index].head;
    g->sommets[src_index].head = nouvel_arc;

}

Graphe *load_graphe(char *file_name){
    char path[50];
    sprintf(path, "../%s.txt", file_name);
    FILE *file = fopen(path, "r");

    int ordre,taille,poids;
    char src,dest,id_sommet;
    
    fscanf(file,"%d",&ordre);
    Graphe *g = init_graphe(ordre);
    g->ordre = ordre;

    for (int i = 0; i < ordre; i++) {
        fscanf(file, " %c", &id_sommet);
    }
    
    fscanf(file,"%d",&taille);
    for (int i = 0; i < taille ; i++)
    {
        fscanf(file," %c %c %d",&src,&dest,&poids);
        init_arc(g,src,dest,poids);
    }

    fclose(file);
    return g;
}

void free_graphe(Graphe* g) {
    if (g == NULL) return;
    for (int i = 0; i < g->ordre; i++) {
        Arc* arc = g->sommets[i].head;
        while (arc != NULL) {
            Arc* temp = arc;
            arc = arc->next;
            free(temp);
        }
    }
    free(g->sommets);
    free(g);
}

void afficher_graphe(Graphe* g) {
    for (int i = 0; i < g->ordre; i++) {
        char sommet = 'A' + i;
        printf("%c :", sommet);
        Arc* arc = g->sommets[i].head;
        while (arc != NULL) {
            printf(" -> [ %c | %d ]", arc->dest, arc->poids);
            arc = arc->next;
        }
        printf("\n");
    }
}

void dijkstra(Graphe* g, char src, char dest) {
    int* distances = (int*)malloc(g->ordre * sizeof(int));
    int* visited = (int*)malloc(g->ordre * sizeof(int));
    char* predecessors = (char*)malloc(g->ordre * sizeof(char));

    for (int i=0 ; i<g->ordre ; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
        predecessors[i] = -1;
    }

    int src_index = src-'A';
    distances[src_index] = 0;

    for (int i=0 ; i < g->ordre-1 ; i++) {

        int min_dist = INT_MAX;
        int min_index = -1;

        for (int j=0 ; j<g->ordre ; j++) {
            if (!visited[j] && distances[j] < min_dist) {
                min_dist = distances[j];
                min_index = j;
            }
        }

        if (min_index == -1) {
            break; 
        }

        visited[min_index] = 1;

        Arc* arc = g->sommets[min_index].head;
        while (arc != NULL) {
            int dest_index = arc->dest-'A';
            if (!visited[dest_index] && 
                distances[min_index] != INT_MAX && 
                distances[min_index] + arc->poids < distances[dest_index]) {
                
                distances[dest_index] = distances[min_index] + arc->poids;
                predecessors[dest_index] = min_index+'A';
            }
            arc = arc->next;
        }
    }

    int dest_index = dest - 'A';
    if (distances[dest_index] == INT_MAX) {
        printf("Pas de chemin trouvé entre %c et %c.\n", src, dest);
    } else {
        printf("-----------------------------------\n");

        char chemin[g->ordre];
        int index = 0;

        for (int v=dest_index ; v!=-1 ; v=predecessors[v]==-1 ? -1 : predecessors[v]-'A') {
            if (v < 0 || v >= g->ordre) {
                printf("Sommet invalide : %c\n", v+'A');
                break;
            }
            chemin[index++] = v+'A';
        }

        if (index == 0) {
            printf("Pas de chemin valide trouve\n");
            return;
        } else {
            printf("Chemin : ");
            for (int k=index-1; k>-1; k--) {
                printf("%c ", chemin[k]);
                if(k)printf("-> ");
            }
            printf("\n");
            printf("Distance minimale de %c vers %c : %d\n", src, dest, distances[dest_index]);
            printf("-----------------------------------\n");
        }
    }

    free(distances);
    free(visited);
    free(predecessors);
}