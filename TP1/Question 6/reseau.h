#ifndef RESEAU_H_INCLUDED
#define RESEAU_H_INCLUDED

/// Le Reseau repr�sente un graphe dans son ensemble

/// La construction du r�seau se fait � partir d'un fichier
/// dont le nom est pass� en param�tre
Station ** ouvrir_reseau(char * nomFichier, int *ordre);

/// afficher toutes les stations du reseau
void afficher_reseau(Station **stations, int ordre);

/// Remet toutes les fr�quences des stations � 0 (non affect�es)
void resetFrequences(Station ** stations, int ordre);

/// L'algorithme suivant trouve une colloration
/// en essayant syst�matiquement toutes les combinaisons
/// d'affectations avec 1 couleur, puis 2 couleurs, puis 3 ...
/// Cet algorithme garantit donc une meilleure coloration
/// ( nombre de couleur utilis�es = nombre chromatique )
/// mais il pr�sente un grave inconv�nient... Lequel ?
void attribuerSystematique(int* nbFrequences, Station ** stations, int ordre);


/// ************* CODE ETUDIANT A COMPLETER *************
void attribuerNaif(int* nbFrequences, Station ** stations, int ordre);

void attribuerWelshPowell(int* nbFrequences, Station ** stations, int ordre);

#endif // RESEAU_H_INCLUDED
