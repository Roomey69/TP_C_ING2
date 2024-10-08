#include "../dijkstra.h"

int main()
{
    char file_name[50];
    char src,dest;
    int menu = MENU;

    Graphe *g = NULL;

    while(menu != LEAVE){
        switch (menu)
        {
        case MENU:
            file_name[0] = '\0';
            printf("Entrez le nom du fichier : "); scanf("%s",file_name);
            g = load_graphe(file_name);
            printf("-------------------------------------------\n");
            printf("1 - Afficher le graphe\n2 - Dijkstra Astar\n3 - Dijkstra Astar Heap\n4 - Quitter\n");
            printf("-------------------------------------------\n");
            scanf("%d",&menu);
            break;
        case GRAPHE:
            afficher_graphe(g);
            menu = MENU;
            break;    
        case DIJKSTRA_ASTAR:
            printf("-------------------------------------------\n");
            printf("Entrez le sommet de depart : "); scanf(" %c",&src);
            printf("Entrez le sommet d'arrive : "); scanf(" %c",&dest);
            printf("-------------------------------------------\n");
            printf("Choisissez la fonction heuristique :\n1 - Tchebychev\n2 - Euclidienne\n3 - Manhattan\n");
            printf("-------------------------------------------\n");
            int choix_heuristique;
            scanf("%d", &choix_heuristique);
            dijkstra(g,src,dest);
            if (choix_heuristique == 1) {
                astar_no_heap(g, src, dest, heuristique_tchebychev);
            } else if (choix_heuristique == 2) {
                astar_no_heap(g, src, dest, heuristique_euclidienne);
            } else if (choix_heuristique == 3) {
                astar_no_heap(g, src, dest, heuristique_manhattan);
            } else {
                printf("Option invalide.\n");
            }
            menu = MENU;
            break;
        case DIJKSTRA_ASTAR_HEAP:
            printf("-------------------------------------------\n");
            printf("Entrez le sommet de depart : "); scanf(" %c",&src);
            printf("Entrez le sommet d'arrive : "); scanf(" %c",&dest);
            printf("-------------------------------------------\n");
            printf("Choisissez la fonction heuristique :\n1 - Tchebychev\n2 - Euclidienne\n3 - Manhattan\n");
            printf("-------------------------------------------\n");
            int choix_heuristique2;
            scanf("%d", &choix_heuristique2);
            dijkstra_heap(g,src,dest);
            if (choix_heuristique2 == 1) {
                astar(g, src, dest, heuristique_tchebychev);
            } else if (choix_heuristique2 == 2) {
                astar(g, src, dest, heuristique_euclidienne);
            } else if (choix_heuristique2 == 3) {
                astar(g, src, dest, heuristique_manhattan);
            } else {
                printf("Option invalide.\n");
            }
            menu = MENU;
            break;
        default:
            printf("Option invalide. Veuillez reessayer.\n");
            break;
        }
    }

    free_graphe(g);
    return 0;
}

Min_heap* create_Min_heap(int capacity) {
    Min_heap* heap = (Min_heap*) malloc(sizeof(Min_heap));
    heap->tab = (Min_heap_node*) malloc(capacity * sizeof(Min_heap_node));
    heap->pos = (int*) malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_Min_heap_node(Min_heap_node* a, Min_heap_node* b) {
    Min_heap_node temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyDown(Min_heap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->tab[left].distance < heap->tab[smallest].distance)
        smallest = left;
    if (right < heap->size && heap->tab[right].distance < heap->tab[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        heap->pos[heap->tab[smallest].sommet] = idx;
        heap->pos[heap->tab[idx].sommet] = smallest;
        swap_Min_heap_node(&heap->tab[smallest], &heap->tab[idx]);
        heapifyDown(heap, smallest);
    }
}

void heapifyUp(Min_heap* heap, int idx) {
    int parent = (idx - 1) / 2;

    if (idx && heap->tab[idx].distance < heap->tab[parent].distance) {
        heap->pos[heap->tab[idx].sommet] = parent;
        heap->pos[heap->tab[parent].sommet] = idx;
        swap_Min_heap_node(&heap->tab[idx], &heap->tab[parent]);
        heapifyUp(heap, parent);
    }
}

void insert_Min_heap(Min_heap* heap, int sommet, int distance) {
    heap->tab[heap->size].sommet = sommet;
    heap->tab[heap->size].distance = distance;
    heap->pos[sommet] = heap->size;
    heap->size++;
    heapifyUp(heap, heap->size - 1);
}

Min_heap_node extractMin(Min_heap* heap) {
    Min_heap_node root = heap->tab[0];
    Min_heap_node lastNode = heap->tab[heap->size - 1];
    heap->tab[0] = lastNode;
    heap->pos[root.sommet] = heap->size - 1;
    heap->pos[lastNode.sommet] = 0;
    heap->size--;
    heapifyDown(heap, 0);
    return root;
}

void decreaseKey(Min_heap* heap, int sommet, int distance) {
    int i = heap->pos[sommet];
    heap->tab[i].distance = distance;
    heapifyUp(heap, i);
}

int is_In_Min_heap(Min_heap* heap, int sommet) {
    if (heap->pos[sommet] < heap->size)
        return 1;
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

    for (int i = 0; i < ordre; i++) {
        fscanf(file, " %c", &id_sommet);
        for (int j = 0; j < sqrt(ordre); j++)
        {
            g->sommets[i].x = i;
            g->sommets[i].y = j;
        }
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
    printf("\n");
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
    printf("\n");
}

void dijkstra(Graphe* g, char src, char dest) {
    int* distances = (int*)malloc(g->ordre * sizeof(int));
    int* visited = (int*)malloc(g->ordre * sizeof(int));
    char* predecessors = (char*)malloc(g->ordre * sizeof(char));
    int n=1;

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
        n++;

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
        printf("-------------------------------------------\n");

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
            printf("Chemin Dijkstra : ");
            for (int k=index-1; k>-1; k--) {
                printf("%c ", chemin[k]);
                if(k)printf("-> ");
            }
            printf("\n");
            printf("Poids total du chemin : %d\n", distances[dest_index]);
            printf("Nombres de sommets decouvert : %d\n",n);
            printf("-------------------------------------------\n");
        }
    }

    free(distances);
    free(visited);
    free(predecessors);
}

void dijkstra_heap(Graphe* g, char src, char dest) {
    int* distances = (int*)malloc(g->ordre * sizeof(int));
    int* predecessors = (int*)malloc(g->ordre * sizeof(int));
    Min_heap* heap = create_Min_heap(g->ordre);
    int n=0;

    for (int i = 0; i < g->ordre; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = -1;
        insert_Min_heap(heap, i, INT_MAX);
    }

    int src_index = src - 'A';
    distances[src_index] = 0;
    decreaseKey(heap, src_index, 0);

    while (heap->size) {
        Min_heap_node minNode = extractMin(heap);
        int u = minNode.sommet;
        n++;

        Arc* arc = g->sommets[u].head;
        while (arc != NULL) {
            int v = arc->dest - 'A';

            if (is_In_Min_heap(heap, v) && distances[u] != INT_MAX &&
                distances[u] + arc->poids < distances[v]) {
                distances[v] = distances[u] + arc->poids;
                predecessors[v] = u + 'A';
                decreaseKey(heap, v, distances[v]);
            }

            arc = arc->next;
        }
    }

    int dest_index = dest - 'A';
    if (distances[dest_index] == INT_MAX) {
        printf("Pas de chemin trouvé entre %c et %c.\n", src, dest);
    } else {
        printf("-------------------------------------------\n");

        char chemin[g->ordre];
        int index = 0;

        for (int v = dest_index; v != -1; v = predecessors[v] == -1 ? -1 : predecessors[v] - 'A') {
            chemin[index++] = v + 'A';
        }

        printf("Chemin Dijkstra Heap : ");
        for (int k = index - 1; k >= 0; k--) {
            printf("%c ", chemin[k]);
            if (k) printf("-> ");
        }
        printf("\nPoids total du chemin : %d\n", distances[dest_index]);
        printf("Nombres de sommets decouvert : %d\n",n);
        printf("-------------------------------------------\n");
    }

    free(distances);
    free(predecessors);
    free(heap->tab);
    free(heap->pos);
    free(heap);
}

int heuristique_tchebychev(int x1, int y1, int x2, int y2) {
    return fmax(abs(x2 - x1), abs(y2 - y1));
}

int heuristique_euclidienne(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int heuristique_manhattan(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

void afficher_chemin_astar(int* parent, int current_index) {
    if (parent[current_index] == -1) {
        printf("%c ", current_index + 'A');
        return;
    }
    afficher_chemin_astar(parent, parent[current_index]);
    printf("-> %c ", current_index + 'A');
}

void astar(Graphe* g, char src, char dest, int (*heuristique)(int, int, int, int)) {
    int* distances = (int*)malloc(g->ordre * sizeof(int));
    int* f_score = (int*)malloc(g->ordre * sizeof(int));
    int* predecessors = (int*)malloc(g->ordre * sizeof(int));  // Stocke les indices comme entiers
    int* visited = (int*)calloc(g->ordre, sizeof(int));  // Tableau pour marquer les sommets visités
    Min_heap* heap = create_Min_heap(g->ordre);
    int n=0;

    for (int i = 0; i < g->ordre; i++) {
        distances[i] = INT_MAX;
        f_score[i] = INT_MAX;
        predecessors[i] = -1;
        insert_Min_heap(heap, i, INT_MAX);
    }

    int src_index = src - 'A';
    int dest_index = dest - 'A';
    distances[src_index] = 0;
    f_score[src_index] = heuristique(g->sommets[src_index].x, g->sommets[src_index].y, g->sommets[dest_index].x, g->sommets[dest_index].y);
    decreaseKey(heap, src_index, f_score[src_index]);

    while (heap->size > 0) {

        Min_heap_node current_node = extractMin(heap);
        int current_index = current_node.sommet;

        if (visited[current_index]) continue;

        visited[current_index] = 1;
        n++;

        if (current_index == dest_index) {
            printf("-------------------------------------------\n");
            printf("Chemin Astar Heap : ");
            afficher_chemin_astar(predecessors, dest_index);
            printf("\nDistance totale : %d\n", distances[dest_index]);
            printf("Nombres de sommets decouvert : %d\n",n);
            printf("-------------------------------------------\n");
            free(distances);
            free(f_score);
            free(predecessors);
            free(visited);
            free(heap->tab);
            free(heap->pos);
            free(heap);
            return;
        }

        Arc* arc = g->sommets[current_index].head;
        while (arc != NULL) {
            int neighbor_index = arc->dest - 'A';

            if (visited[neighbor_index]) {
                arc = arc->next;
                continue;
            }

            if (distances[neighbor_index] == INT_MAX) {
                n++;
            }

            int tentative_g_score = distances[current_index] + arc->poids;

            if (tentative_g_score < distances[neighbor_index]) {
                distances[neighbor_index] = tentative_g_score;
                f_score[neighbor_index] = tentative_g_score + heuristique(g->sommets[neighbor_index].x, g->sommets[neighbor_index].y, g->sommets[dest_index].x, g->sommets[dest_index].y);
                predecessors[neighbor_index] = current_index;

                if (is_In_Min_heap(heap, neighbor_index)) {
                    decreaseKey(heap, neighbor_index, f_score[neighbor_index]);
                }
            }

            arc = arc->next;
        }
    }

    printf("Pas de chemin trouve entre %c et %c.\n", src, dest);
    free(distances);
    free(f_score);
    free(predecessors);
    free(visited);
    free(heap->tab);
    free(heap->pos);
    free(heap);
}

void astar_no_heap(Graphe* g, char start, char goal, int (*heuristique)(int, int, int, int)) {
    int ordre = g->ordre;
    int *dist = (int*) malloc(ordre * sizeof(int));
    int *fScore = (int*) malloc(ordre * sizeof(int));
    int *visited = (int*) calloc(ordre, sizeof(int));
    int *parent = (int*) malloc(ordre * sizeof(int));
    int n=1;

    for (int i = 0; i < ordre; i++) {
        dist[i] = INT_MAX;
        fScore[i] = INT_MAX;
        parent[i] = -1;
    }

    int start_index = start - 'A';
    int goal_index = goal - 'A';
    dist[start_index] = 0;
    fScore[start_index] = heuristique(g->sommets[start_index].x, g->sommets[start_index].y, g->sommets[goal_index].x, g->sommets[goal_index].y);

    while (1) {
        int current_index = -1;
        int min_fScore = INT_MAX;

        for (int i = 0; i < ordre; i++) {
            if (!visited[i] && fScore[i] < min_fScore) {
                min_fScore = fScore[i];
                current_index = i;
            }
        }

        if (current_index == -1) {
            printf("Pas de chemin trouve vers %c.\n", goal);
            break;
        }

        if (current_index == goal_index) {
            printf("-------------------------------------------\n");
            printf("Chemin Astar :");
            afficher_chemin_astar(parent, goal_index);
            printf("\nPoids total du chemin : %d\n", dist[goal_index]);
            printf("Nombre de sommets decouvert : %d\n",n);
            printf("-------------------------------------------\n");
            free(dist);
            free(fScore);
            free(visited);
            free(parent);
            return;
        }

        visited[current_index] = 1;
        n++;

        Arc* adj = g->sommets[current_index].head;
        while (adj != NULL) {
            int neighbor_index = adj->dest - 'A';
            if (dist[neighbor_index] == INT_MAX) {
                n++;
            }
            if (!visited[neighbor_index]) {
                int tentative_gScore = dist[current_index] + adj->poids;
                if (tentative_gScore < dist[neighbor_index]) {
                    dist[neighbor_index] = tentative_gScore;
                    fScore[neighbor_index] = tentative_gScore + heuristique(g->sommets[neighbor_index].x, g->sommets[neighbor_index].y, g->sommets[goal_index].x, g->sommets[goal_index].y);
                    parent[neighbor_index] = current_index;
                }
            }
            adj = adj->next;
        }
    }

    free(dist);
    free(fScore);
    free(visited);
    free(parent);
}
