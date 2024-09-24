//
// Created by Laurent Debize on 04/10/2023.
//

#ifndef TP2_LABYRINTHE_CELLULE_H
#define TP2_LABYRINTHE_CELLULE_H

/*cellule.h*/
//source : http://morissardjerome.free.fr/langageC/graphe2.html

typedef int typage;

struct _cellule {
    typage element;
    struct _cellule *suivant;
};

typedef struct _cellule* Cellule;

#endif //TP2_LABYRINTHE_CELLULE_H
