#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "graph.h"
#include <math.h>

/**
 * @brief Convertit un tableau de sommets en un graphe connexe non cyclique.
 * @param tab Le tableau de sommets à convertir.
 * @param n Le nombre de sommets dans le tableau.
 * @return Un pointeur vers un sommet du graph
 */
sommet_t * tabToGraph(sommet_t ** tab, int n){
    if(tab != NULL){
        srand(time(NULL));
        int cut = rand()%(n-1);
        tab[0]->voisins[tab[0]->n_voisin] = tabToGraph(tab+1, cut);
        tab[1]->voisins[tab[1]->n_voisin] = tab[0];
        tab[0]->n_voisin++;
        tab[1]->n_voisin++;
        if(cut != 0){
            tab[0]->voisins[tab[0]->n_voisin] = tabToGraph(tab+cut, cut-n);
            tab[cut]->voisins[tab[cut]->n_voisin] = tab[0];
            tab[0]->n_voisin++;
            tab[cut]->n_voisin++;
        }
        return tab[0];
    }
}


/**
 * @brief Créée un tableau de points avec des coordonnées aléatoires
 * @param n le pointeur sur le nombre de points à générer
 * @param width la borne sur l'axe des abscisses
 * @param height la borne sur l axe des ordonnées
 * @return un pointeur sur un tableau de sommets
 */

sommet_t ** genTabSommets(int * n, int width, int height)
{
    *n = rand()%(N-3) +4;
    sommet_t ** tab = malloc((*n)*sizeof(sommet_t *));
    for(int i=0; i<*n; i++)
    {
        tab[i] = malloc(sizeof(sommet_t));
        tab[i]->x = rand()%(width);
        tab[i]->y = rand()%(height);        //On génère les nombre aléatoirement entre des bornes représentants la taille de la fenêtre
        tab[i]->val = i+65;             //On assigne des valeurs aux sommet, en l'occurence A,B,C...
        for(int j=0; j<*n;j++)
        {
            tab[i]->voisin[j]=0;    //Tableau des liens initialisé vide
        }
    }
    return tab;
}

/**
 * @brief Affiche le tableau des coordonnées des points
 * @param tab le pointeur sur le tableau de points
 * @param n le pointeur sur le nombre de sommets du tableau
*/
void printTabCoord(sommet_t ** tab, int * n)
{
    printf("nombre de points: %d\n", *n);
    for(int i=0; i<(*n); i++)
    {
        printf("%c : (%d,%d) - ",tab[i]->val, tab[i]->x, tab[i]->y);
    }
    printf("\n");

}


/**
 * @brief Créée des liasons aléatoires entre les sommets
 * @param p probabilité de création de liaison pour chaque couple de point.
 * @param tab le pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 */
void makeNewLinks(int p, sommet_t ** tab, int * n)
{
    for(int i=0; i<*n; i++)
    {
        for(int j=0; j<n*; j++)  
        {
            if(tab[i]->voisins[j]==0 && rand()%(100)<p) // On regarde pour chaque point son tableau "binaire" de voisins et on tire un random entre 0 et 100
            {
                tab[i]->voisins[j]=1;          // Si 2 points ne sont pas voisins et qu'on tire un random respectant notre porba souhaitée, on lie les points.
                tab[j]->voisins[i]=1;
                printf("NEW_LINK: (%c, %c) - ", tab[i]->val, tab[j]->val);
            }   
        }
    }
}

/**
 * @brief Calcul la distance entre 2 sommets
 * @param a pointeur sur un sommet
 * @param b pointeur sur un sommet
 * @return La distance
 */
int calculDistance(sommet_t * a, sommet_t * b)
{
    int tmp1 = (a->x-b->x), tmp2 = (a->y-b->y);
    return (int)math.sqrt(tmp1*tmp1+tmp2*tmp2);
}


/**
 * @brief Créée un tableau de distance entre les points, -1 si non lié, > 0 sinon.
 * @param tab pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 * @return Le pointeur sur le tableau 2D des distances.
 */
int ** distTab(sommet_t ** tab)
{
    int ** tabDist = malloc((*n)*sizeof(int *));
    int tmpDist=0;
    for(int i=0; i<*n; i++)
    {
        tabDist[i] = malloc((*n)*sizeof(int));
        for(int j=0; j<i; j++)  
        {
            if(i!=j && tab[i]->voisins[j]==0)
            {
                tmpDist = calculDistance(tab[i], tab[j]);
                tabDist[i][j] = tmpDist;
                tabDist[j][i] = tmpDist;
            }
            else if(i==j)
            {
                tabDist[i][i] = 0;
            }
            else
            {
                tabDist[j][i] = -1;
            }
        }
    }
    return distTab;
}



void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n) {
    char deja_trace[n];
}
