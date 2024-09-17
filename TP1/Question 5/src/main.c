#include "../header.h"

int main(int argc, char const *argv[])
{
    Graphe *graphe = (Graphe *)malloc(sizeof(Graphe));
    char file[MAX_CHAR];
    printf("Entrez le nom du fichier : "); scanf("%s",file);

    load_graphe(file,graphe);
    print_graphe(*graphe);
    free_graphe(graphe);
    
    return 0;
}