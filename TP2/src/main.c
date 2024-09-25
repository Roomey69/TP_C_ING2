
#include "../Graphe.h"

/* affichage des successeurs du sommet num*/
void afficher_successeurs(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet *CreerArete(Graphe *graphe, int s1, int s2) {
    pSommet *sommet = graphe->pSommet;

    if (sommet[s1 - graphe->sommet_init]->arc == NULL) {
        pArc Newarc = (pArc) malloc(sizeof(struct Arc));
        Newarc->sommet = s2;
        Newarc->arc_suivant = NULL;
        sommet[s1 - graphe->sommet_init]->arc = Newarc;
        return sommet;
    } else {
        pArc temp = sommet[s1 - graphe->sommet_init]->arc;
        while (temp->arc_suivant != NULL) {
            temp = temp->arc_suivant;
        }
        pArc Newarc = (pArc) malloc(sizeof(struct Arc));
        Newarc->sommet = s2;
        Newarc->arc_suivant = NULL;

        if (temp->sommet > s2) {
            Newarc->arc_suivant = temp->arc_suivant;
            Newarc->sommet = temp->sommet;
            temp->sommet = s2;
            temp->arc_suivant = Newarc;
            return sommet;
        }

        temp->arc_suivant = Newarc;
        return sommet;
    }
}

// créer le graphe
Graphe* CreerGraphe(int ordre)
{
    Graphe * Newgraphe=(Graphe*)malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));

    for(int i=0; i<ordre; i++)
    {
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->valeur=i;
        Newgraphe->pSommet[i]->arc=NULL;
    }
    return Newgraphe;
}

/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
Graphe *lire_graphe(char *nomFichier) {
    Graphe *graphe;

    char path[50];
    sprintf(path, "../%s.txt", nomFichier);

    FILE *ifs = fopen(path, "r");
    int taille, orientation, ordre, s1, s2;

    if (!ifs) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    fscanf(ifs, "%d", &ordre);

    graphe = CreerGraphe(ordre); // créer le graphe d'ordre sommets

    fscanf(ifs, "%d", &taille);
    fscanf(ifs, "%d", &orientation);

    graphe->orientation = orientation;
    graphe->ordre = ordre;
    graphe->taille = taille;

    // créer les arêtes du graphe
    for (int i = 0; i < taille; ++i) {
        fscanf(ifs, "%d%d", &s1, &s2);
        if (i == 0) graphe->sommet_init = s1;
        graphe->pSommet = CreerArete(graphe, s1, s2);

        if (!orientation)
            graphe->pSommet = CreerArete(graphe, s2, s1);
    }

    return graphe;
}

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe)
{
    printf("graphe\n");

    if(graphe->orientation)
        printf("oriente\n");
    else
        printf("non oriente\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'adjacence :\n");

    for (int i=0; i<graphe->ordre; i++)
    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }
    printf("%d",graphe);
}

int *BFS(Graphe *graphe, int start) {

    int *BFS = (int *)malloc(sizeof(int) * graphe->ordre);
    int *visite = (int *)calloc(graphe->ordre, sizeof(int));

    File F = fileVide();

    visite[start-graphe->sommet_init] = 1;
    enfiler(F, start);

    int index = 0;

    while (longueur(F) > 0) {
        int sommet_current = defilement(F);

        BFS[index++] = sommet_current;

        pArc arc_current = graphe->pSommet[sommet_current - graphe->sommet_init]->arc;

        while (arc_current != NULL) {
            int sommet_voisin = arc_current->sommet - graphe->sommet_init;

            if (!visite[sommet_voisin]) {
                visite[sommet_voisin] = 1;
                enfiler(F, arc_current->sommet);
            }

            arc_current = arc_current->arc_suivant;
        }
    }

    free(visite);

    printf("Ordre de visite des sommets BFS : ");
    for (int i = 0; i < index; i++) {
        printf("%d ", BFS[i]);
    }
    printf("\n");

    return BFS;
}

void DFS_annexe(Graphe *graphe, int sommet, int *visite, int *DFS, int *index) {

    visite[sommet-graphe->sommet_init] = 1;
    DFS[(*index)++] = sommet;
    pArc arc_current = graphe->pSommet[sommet-graphe->sommet_init]->arc;

    while (arc_current != NULL) {
        int sommet_voisin = arc_current->sommet - graphe->sommet_init;
        if (!visite[sommet_voisin]) DFS_annexe(graphe, arc_current->sommet, visite, DFS, index);
        arc_current = arc_current->arc_suivant;
    }

}

int *DFS(Graphe *graphe, int start) {

    int *DFS = (int *)malloc(sizeof(int) * graphe->ordre);
    int *visite = (int *)calloc(graphe->ordre, sizeof(int));
    int index = 0;

    DFS_annexe(graphe, start, visite, DFS, &index);

    free(visite);

    printf("Ordre de visite des sommets DFS : ");
    for (int i = 0; i < index; i++) {
        printf("%d ", DFS[i]);
    }
    printf("\n");

    return DFS;
}

int **search_cc(Graphe *graphe, int *composante_num) {

    int **tab_cc = NULL;

    int *visite = (int *)calloc(graphe->ordre, sizeof(int));
    *composante_num = 0;

    for (int start = graphe->sommet_init ; start < graphe->ordre+graphe->sommet_init ; start++) {
        if (!visite[start]) {
            (*composante_num)++;

            tab_cc = (int**)realloc(tab_cc,sizeof(int*)*(*composante_num));
            tab_cc[*composante_num-1] = (int*)malloc(sizeof(int)*graphe->ordre);
            for (int i = 0; i < graphe->ordre; i++)
            {
                tab_cc[*composante_num-1][i] = -1;
            }

            printf("Composante connexe %d : ", *composante_num);

            int *composante = (int *)malloc(sizeof(int) * graphe->ordre);
            int index = 0;

            DFS_annexe(graphe, start, visite, composante, &index);

            for (int i = 0; i < index; i++) {
                printf("%d ",composante[i]);
                tab_cc[*composante_num-1][composante[i]] = composante[i];
            }

            printf("\n");
            free(composante);
        }
    }
    printf("\n");
    free(visite);
    
    return tab_cc;
}

void print_cc(Graphe *graphe){

    int composante_num;
    int **tab = search_cc(graphe,&composante_num);

    for (int i = 0; i < composante_num; i++){
        for (int j = 0; j < graphe->ordre; j++)
        {
            printf("%d ",tab[i][j]);
        }   
        printf("\n");
    }
    
}

int *calcul_degre(Graphe graphe){

    int *tab = (int*)malloc(sizeof(int)*graphe.ordre);

    for (int i = 0; i < graphe.ordre; i++)
    {
        int current_degre = 0;
        pArc arc = graphe.pSommet[i]->arc;
        while ( arc != NULL )
        {
            arc = arc->arc_suivant;
            current_degre++;
        }
        tab[i] = current_degre;
    }
    
    return tab;
}

void search_cycle_chaine(Graphe *g){

    int composante_num;
    int *tab_deg = calcul_degre(*g);
    int **matrice = search_cc(g,&composante_num);

    int *num_impair = (int*)calloc(composante_num,sizeof(int));

    for (int i = 0; i < composante_num; i++){
        for (int j = 0; j < g->ordre; j++)
        {
            if ( matrice[i][j]!=-1 && tab_deg[matrice[i][j]]%2 )
            {
                num_impair[i]++;
            }   
        }  
    }
    
    for (int i = 0; i < composante_num; i++)
    {
        if ( !num_impair[i] )
        {
            printf("Composante connexe %d : Cycle eulerien\n",i+1);
        } else if ( num_impair[i] == 2 ){
            printf("Composante connexe %d : Chaine eulerienne\n",i+1);
        } else {
            printf("Composante connexe %d : Pas de chaine ni cycle\n",i+1);
        }
    } 
}

int main()
{
    Graphe * g;
    char nom_fichier[50];
    int start;

    printf("Entrer le nom du fichier du labyrinthe : "); gets(nom_fichier);
    g = lire_graphe(nom_fichier);
    // printf("Sommet initial : "); scanf("%d",&start);

    // int *bfs = BFS(g,start);
    // int *dfs = DFS(g,start);
    // print_cc(g);

    //graphe_afficher(g);
    search_cycle_chaine(g);

    return 0;
}
