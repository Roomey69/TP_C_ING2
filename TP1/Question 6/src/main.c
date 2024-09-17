/* code r�alis� par JPS en C et inspir� du code C++ de Mr. Fercoq */
#include <math.h>
#include "../station.h"
#include "../reseau.h"

#define MAX_LEN 100

/// Retourne la norme d'un vecteur : utilis� par le calcule de distance entre stations
double norme(double vx, double vy)
{
    return sqrt(vx*vx + vy*vy);
}

/// La construction se fait directement depuis le fichier ouvert en cours de lecture...
Station * lecture_station (FILE * is)
{
    Station * s=(Station *)malloc(sizeof(Station));
    s->m_frequence = 0;
    fscanf(is,"%d%f%f\n",&s->m_id, &s->m_x, &s->m_y);
    return s;
}

/// M�thode de calcul de distance � une autre station (pour d�terminer le voisinage)
double distance(const Station * autre, const Station * s)
{
    return norme( autre->m_x - s->m_x, autre->m_y - s->m_y  );
}

/// Affecte une fr�quence � la station
void setFrequence(Station *s, int frequence)
{
    s->m_frequence = frequence;
}

/// Test l'affectation d'une fr�quence
/// retourne vrai si la fr�quence n'est pas en conflit avec une station adjacente
/// faux sinon
int testFrequence(Station ** stations, Station * s, int ordre, int frequence)
{
    int i=s->m_id-1;
    Station * s_adj;

    for (int j=0; j<ordre; j++)
    {
        s_adj=NULL;

        if(m_adjacents[i][j])
            s_adj=stations[j];

        if ( s_adj!=NULL && s_adj->m_frequence && s_adj->m_frequence == frequence )
            return 0;
    }

    return 1;
}

/// M�thode de d�termination des stations adjacentes ( distance < dmin )
void determineAdjacents(Station** stations, int ordre, Station *s, double dmin)
{
    for (int i=0; i<ordre; i++)
    {
        if ( distance(stations[i],s)<dmin && stations[i]!=s )
            m_adjacents[stations[i]->m_id-1][s->m_id-1]=1;
    }
}

/// La construction du r�seau se fait � partir d'un fichier
/// dont le nom est pass� en param�tre
Station ** ouvrir_reseau(char * nomFichier, int *ordre)
{
    char path[MAX_LEN];
    sprintf(path, "../%s",nomFichier);

    FILE * ifs = fopen(path,"r");

    Station** stations;

    if(!ifs)
    {
        printf("Erreur de fichier");
        exit(-1);
    }

    double dmin;
    fscanf(ifs,"%lf",&dmin);

    fscanf(ifs,"%d",ordre);

    stations=(Station**) malloc(*ordre * sizeof (Station*));
    m_adjacents=(int**) malloc(*ordre * sizeof (int*));

    for(int i=0; i<*ordre; i++)
    {
        m_adjacents[i]=(int*) malloc(*ordre * sizeof (int));
        for(int j=0; j<*ordre; j++)
            m_adjacents[i][j]=0;
    }

    for (int i=0; i<*ordre; ++i)
    {
        stations[i]=lecture_station(ifs);
    }

    for (int i=0; i<*ordre; ++i)
        determineAdjacents(stations, *ordre, stations[i], dmin);

    return stations;
}

/// afficher toutes les stations du reseau
void afficher_reseau(Station **stations, int ordre)
{
    printf("Reseau d'ordre %d :\n",ordre);

    for (int i=0; i<ordre; ++i)
    {
        printf("id = %d x = %.2f y = %.2f frequence = %d",stations[i]->m_id,stations[i]->m_x,stations[i]->m_y,stations[i]->m_frequence);

        printf(" Adjacents=");

        for (int j=0; j<ordre ; j++)
        {
            if (m_adjacents[stations[i]->m_id-1][j])
                printf("%d ",j+1);
        }
        printf("\n");
    }
}

/// Remet toutes les fr�quences des stations � 0 (non affect�es)
void resetFrequences(Station ** stations, int ordre)
{
    for (int i=0; i<ordre; ++i)
    {
        stations[i]->m_frequence=0;
    }
}

/// ************* CODE ETUDIANT *************
void attribuerNaif(int *nbFrequences, Station **stations, int ordre)
{
    int **tab_freq = (int**)malloc(ordre * sizeof(int*));
    for (int i = 0; i < ordre; i++){
        tab_freq[stations[i]->m_id - 1] = (int*)calloc(ordre, sizeof(int));
    }
    
    int current_freq = 1;
    *nbFrequences = 1;
    stations[0]->m_frequence = 1;

    for (int i = 1; i < ordre ; i++){
        current_freq = 1;
        for (int j = 0; j < ordre; j++){
            if (m_adjacents[stations[i]->m_id - 1][stations[j]->m_id - 1] && stations[j]->m_frequence != 0){
                tab_freq[stations[i]->m_id - 1][stations[j]->m_frequence - 1] = 1;
            }
        }
        
        while (tab_freq[stations[i]->m_id - 1][current_freq - 1] != 0){
            current_freq++;
        }

        stations[i]->m_frequence = current_freq;

        if (current_freq > *nbFrequences){
            *nbFrequences = current_freq;
        }
    }

    for (int i = 0; i < ordre; i++){
        free(tab_freq[stations[i]->m_id - 1]);
    }
    free(tab_freq);
}

int comparerStations(const void *a, const void *b)
{
    Station *stationA = *(Station **)a;
    Station *stationB = *(Station **)b;

    return stationB->m_degre - stationA->m_degre;

}

void calculerDegreStations(Station **stations, int ordre, int **m_adjacents)
{
    for (int i = 0; i < ordre; i++)
    {
        int degre = 0;
        for (int j = 0; j < ordre; j++)
        {
            if (m_adjacents[stations[i]->m_id - 1][j])  
                degre++;
        }
        stations[i]->m_degre = degre;
    }
}

void attribuerWelshPowell(int *nbFrequences, Station **stations, int ordre)
{
    calculerDegreStations(stations, ordre, m_adjacents);
    qsort(stations, ordre, sizeof(Station*), comparerStations);
    attribuerNaif(nbFrequences,stations,ordre);
}


/// ************* FIN CODE ETUDIANT *************


int main()
{

    int ordre;

    Station ** stations = ouvrir_reseau("reseau2.txt", &ordre);
    afficher_reseau(stations, ordre);

    int nbFreqSyst;
    attribuerWelshPowell(&nbFreqSyst, stations, ordre);
    printf("Nombre chromatique : %d frequences utilisees (optimal)\n", nbFreqSyst);
    afficher_reseau(stations, ordre);
    return 0;
}

void attribuerSystematique(int* nbFrequences, Station ** stations, int ordre)
{
    *nbFrequences = 0;

    while ( 1 )
    {
        ++(*nbFrequences);

        int nbCas = pow(*nbFrequences, ordre);
        for (int k=0; k<nbCas; ++k)
        {
            resetFrequences(stations, ordre);
            int nbAffecte = 0;
            int combi = k;

            for (int i=0; i<ordre; ++i)
            {
                int freq = combi%(*nbFrequences) + 1;

                if ( !testFrequence(stations,stations[i],ordre, freq) )
                    break;

                setFrequence(stations[i], freq);
                ++nbAffecte;

                combi /= *nbFrequences;
            }

            if ( nbAffecte == ordre )
                return;
        }
    }
}

