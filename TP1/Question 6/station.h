#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

/// La Station repr�sente un sommet du graphe
/// Les adjacences sont d�duites des positions x,y des stations
/// durant le chargement du fichier, en fonction du param�tre

/// Donn�es sp�cifiques du sommet
typedef struct station
{
    int m_id;        // Indice num�rique pour l'affichage
    float m_x, m_y; // Position sur le terrain
    int m_frequence; // Fr�quence affect�e (ou 0 si pas encore de fr�quence)
    int m_degre;
} Station;

/// Voisinage : matrice d'adjacence
int** m_adjacents;

/// La construction se fait directement depuis le fichier ouvert en cours de lecture...
Station * lecture_station (FILE * is);

/// M�thode de calcul de distance � une autre station (pour d�terminer le voisinage)
double distance(const Station * autre, const Station * s) ;

/// Affecte une fr�quence � la station
void setFrequence(Station *s, int frequence);

/// Test l'affectation d'une fr�quence
/// retourne vrai si la fr�quence n'est pas en conflit avec une station adjacente
/// faux sinon
int testFrequence(Station ** stations, Station * s, int ordre, int frequence);

/// M�thode de d�termination des stations adjacentes ( distance < dmin )
void determineAdjacents(Station** stations, int ordre, Station *s, double dmin);

#endif // STATION_H_INCLUDED
