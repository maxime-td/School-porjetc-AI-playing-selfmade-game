#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "graph.h"

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
        tab[i]->y = rand()%(height);
        tab[i]->val = i+65;
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
            if(tab[i]->voisins[j]==0 && rand()%(p)<25)
            {
                tab[i]->voisins[j]=1;
                tab[j]->voisins[i]=1;
                printf("NEW_LINK: (%c, %c) - ", tab[i]->val, tab[j]->val);
            }   
        }
    }
}

/**
 * @brief Trace un disque
 * @param renderer Le renderer où tracer le disque
 * @param center_x La coordonnée x du centre du disque
 * @param center_y La coordonnée y du centre du disque
 * @param radius Le rayon du disque
 */
void draw_disk(SDL_Renderer* renderer, int center_x, int center_y, int radius) {
    // Calculer les coordonnées du rectangle englobant le disque
    int x = center_x - radius;
    int y = center_y - radius;
    int width = radius * 2;
    int height = radius * 2;

    // Dessiner le disque rempli
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            // Vérifier si le point (i, j) est à l'intérieur du cercle
            if ((i - center_x) * (i - center_x) + (j - center_y) * (j - center_y) <= radius * radius) {
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

/**
 * @brief Dessine un graphe à l'aide d'un rendu SDL.
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param graph Le pointeur vers le graphe à dessiner.
 * @param n Le nombre de sommets dans le tableau.
 */
void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n) {

    /* Initialisations */
    char tab_deja_trace[n]; //tableau des sommets déjà tracés
    int bool_deja_trace; //Booléeen si un sommet à déjà été tracé ou non. 0 = Faux, 1 = Vrai
    int compteur_deja_trace = 0; //Compteur de sommets déjà tracés
    int i, j, k; //Incréments
    int rayon = 25; //Rayon des disques des sommets

    sommet_t* sommet_courant; //Sommet courant
    sommet_t* voisin_courant; //Voisin courant

    /* Initialisation tableau deja_trace */
    for(i = 0; i < n; i+=1) {
        deja_trace[i] = NULL;
    }

    /* Parcour */
    for(i = 0; i < n; i+=1) {
        sommet_courant = *tab[i];
        draw_disk(renderer, sommet_courant.x, sommet_courant.y, rayon); //Traçage du sommet
        
        tab_deja_trace[compteur_deja_trace] = sommet_courant.val; //Ajoute le sommet courant a tab_deja_trace
        compteur_deja_trace += 1;

        for(j = 0; j < sommet_courant.n_voisins; j+=1) {
            voisin_courant = sommet_courant.voisins[j];

            /* Vérification si déjà tracé ou pas */
            bool_deja_trace = 0;
            while(deja_trace[k] != NULL) {
                if(deja_trace[k] == voisin_courant.val)
                    bool_deja_trace = 1;
                k += 1;
            }

            if(bool_deja_trace == 0) {
                SDL_RenderDrawLine(renderer, sommet_courant.x, sommet_courant.y, voisin_courant.x, voisin_courant.y); //Traçage du lien

                tab_deja_trace[compteur_deja_trace] = voisin_courant.val; //Ajoute le voisin courant a tab_deja_trace
                compteur_deja_trace += 1;
            }
        }
    }
}
