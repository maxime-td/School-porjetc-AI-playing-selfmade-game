#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include <math.h>


/**
 * Libère la mémoire allouée pour un tableau à deux dimensions.
 *
 * @param tab Le tableau à libérer.
 * @param n   Le nombre de lignes du tableau.
 */
void free2DTab(void ** tab, int n){
    for (int i = 0; i < n; i++){
        free(tab[i]);
    }
    free(tab);
}


/**
 * @brief Convertit un tableau de sommets en un graphe connexe non cyclique.
 * @param tab Le tableau de sommets à convertir.
 * @param n Le nombre de sommets dans le tableau.
 * @return Un pointeur vers un sommet du graph
 */
void tab_to_graph(sommet_t ** tab, int start, int end){
    if(start < end){
        int k = rand()%(end-start)+(start+1);
        tab[start]->voisins[start+1] = 1;
        tab[start + 1]->voisins[start] = 1;
        tab[start]->n_voisin++;
        tab[start + 1]->n_voisin++;

        if(k+1 <= end){
            tab[start]->voisins[k+1] = 1; 
            tab[k+1]->voisins[start] = 1;
            tab[start]->n_voisin++;
            tab[k+1]->n_voisin++;   
        }

        tab_to_graph(tab, start+1, k);
        tab_to_graph(tab, k+1, end);
    }
}

/**
 * @brief Vérifie si un angle est proche d'un angle déjà présent dans un tableau.
 * @param angles Le tableau d'angles.
 * @param size La taille du tableau.
 * @param angle L'angle à vérifier.
 * @param threshold La marge d'erreur pour considérer deux angles comme proches.
 * @return 1 si l'angle est proche d'un angle déjà présent, 0 sinon.
*/ 
int is_close_to_value_in_tab(sommet_t ** tab, int size, sommet_t * cour, int threshold) {
    for (int i = 0; i < size; i++) 
    { 
        if (sqrt(calcul_distance(tab[i], cour)) <= threshold) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Créée un tableau de points avec des coordonnées aléatoires
 * @param n le pointeur sur le nombre de points à générer
 * @param width la borne sur l'axe des abscisses
 * @param height la borne sur l axe des ordonnées
 * @return un pointeur sur un tableau de sommets
 */

sommet_t ** gen_tab_sommets(int * n, int width, int height)
{
    *n = rand()%(N-3) +4;
    int first = 1;
    int angle;
    sommet_t ** tab = malloc((*n)*sizeof(sommet_t *));
    for(int i=0; i<*n; i++)
    {
        first = 1;
        tab[i] = malloc(sizeof(sommet_t));
        while (first || is_close_to_value_in_tab(tab, i, tab[i], (4*R/(*n))))
        {
            first = 0;
            angle = rand()%(360);

            
            tab[i]->x = cos(angle)*R+W/2;
            tab[i]->y = sin(angle)*R+H/2;        //On génère les nombre aléatoirement entre des bornes représentants la taille de la fenêtre
        }

        tab[i]->val = i+'A';             //On assigne des valeurs aux sommet, en l'occurence A,B,C...

        for(int j=0; j<*n;j++){
            tab[i]->voisins[j]=0;    //Tableau des liens initialisé vide
        }
    }
    return tab;
}

/**
 * @brief Affiche le tableau des coordonnées des points
 * @param tab le pointeur sur le tableau de points
 * @param n le pointeur sur le nombre de sommets du tableau
*/
void print_tab_coord(sommet_t ** tab, int * n)
{
    printf("nombre de points: %d\n", *n);
    for(int i=0; i<(*n); i++)
    {
        printf("%c : (%d,%d) :\n",tab[i]->val ,tab[i]->x, tab[i]->y);
        for (int j = 0; j < (*n); j++)
        {
            printf("%d ", tab[i]->voisins[j]);
        }
        printf("\n");
        
    }
    printf("\n");

}

/**
 * @brief Créée des liasons aléatoires entre les sommets
 * @param p probabilité de création de liaison pour chaque couple de point.
 * @param tab le pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 */
void make_new_links(int p, sommet_t ** tab, int * n)
{
    for(int i=0; i<*n; i++)
    {
        for(int j=0; j<*n; j++)  
        {
            if(i!=j && tab[i]->voisins[j]==0 && rand()%(100)<p) // On regarde pour chaque point son tableau "binaire" de voisins et on tire un random entre 0 et 100
            {
                tab[i]->voisins[j]=1;          // Si 2 points ne sont pas voisins et qu'on tire un random respectant notre porba souhaitée, on lie les points.
                tab[j]->voisins[i]=1;
            }   
        }
    }
    printf("\n");
}

/**
 * @brief Calcul la distance entre 2 sommets
 * @param a pointeur sur un sommet
 * @param b pointeur sur un sommet
 * @return La distance
 */

int calcul_distance(sommet_t * a, sommet_t * b)
{
    int tmp1 = (a->x-b->x), tmp2 = (a->y-b->y);
    return tmp1*tmp1+tmp2*tmp2;
}

/**
 * @brief Créée un tableau de distance entre les points, -1 si non lié, > 0 sinon.
 * @param tab pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 * @return Le pointeur sur le tableau 2D des distances.
 */
int ** dist_tab(sommet_t ** tab, int * n)
{
    int ** tabDist = malloc((*n)*sizeof(int *));
    int tmpDist=0;
    for(int i=0; i<*n; i++)
    {
        tabDist[i] = malloc((*n)*sizeof(int));
        for(int j=0; j<=i; j++)  
        {
            if(i!=j && tab[i]->voisins[j]==1)
            {
                tmpDist = calcul_distance(tab[i], tab[j]);
                tabDist[i][j] = tmpDist;
                tabDist[j][i] = tmpDist;
            }
            else if(i!=j)
            {
                tabDist[i][j] = -1;
                tabDist[j][i] = -1;
            }
            else
            {
                tabDist[j][i] = 0;
            }
        }
    }
    return tabDist;
}

/**
 * @brief Affiche le tableau des distances
 * @param dist_tab le tableau des distances
 * @param n le pointeur sur le ombre de sommets
 */
void print_dist_tab(int ** dist_tab, int * n)
{
    for(int i=0; i<(*n); i++)
    {
        for(int j=0; j<(*n); j++)
        {
            printf("distance (%c,%c) : %d -- ", i+65, j+65, dist_tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    //return distTab;
}

/**
 * @brief Trace un disque
 * @param renderer Le renderer où tracer le disque
 * @param center_x La coordonnée x du centre du disque
 * @param center_y La coordonnée y du centre du disque
 * @param radius Le rayon du disque

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
*/

/**
 * @brief Dessine un graphe à l'aide d'un rendu SDL.
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param graph Le pointeur vers le graphe à dessiner.
 * @param n Le nombre de sommets dans le tableau.
 
void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n) {

    // Initialisations 
    char tab_deja_trace[n]; //tableau des sommets déjà tracés
    int bool_deja_trace; //Booléeen si un sommet à déjà été tracé ou non. 0 = Faux, 1 = Vrai
    int compteur_deja_trace = 0; //Compteur de sommets déjà tracés
    int i, j, k; //Incréments
    int rayon = 10; //Rayon des disques des sommets

    sommet_t* sommet_courant; //Sommet courant
    sommet_t* voisin_courant; //Voisin courant

    // Initialisation tableau deja_trace 
    for(i = 0; i < n; i+=1) {
        tab_deja_trace[i] = '0';
    }

    // Parcour 
    for(i = 0; i < n; i+=1) {
        printf("i:%d\n", i);
        sommet_courant = tab[i];
        draw_disk(renderer, sommet_courant->x, sommet_courant->y, rayon); //Traçage du sommet
        
        tab_deja_trace[compteur_deja_trace] = sommet_courant->val; //Ajoute le sommet courant a tab_deja_trace
        compteur_deja_trace += 1;

        for(j = 0; j < n; j+=1) {
            k=0;
            printf("j:%d\n", j);
            // Si j est voisin de i
            if(tab[i]->voisins[j] == 1) {
                voisin_courant = tab[j];

                // Vérification si déjà tracé ou pas
                bool_deja_trace = 0;
                while(tab_deja_trace[k] != '0') {
                    if(tab_deja_trace[k] == voisin_courant->val)
                        bool_deja_trace = 1;
                    k += 1;
                }

                // Si pas déjà tracé, on le trace 
                if(bool_deja_trace == 0) {
                    SDL_RenderDrawLine(renderer, sommet_courant->x, sommet_courant->y, voisin_courant->x, voisin_courant->y); //Traçage du lien

                    tab_deja_trace[compteur_deja_trace] = voisin_courant->val; //Ajoute le voisin courant a tab_deja_trace
                    compteur_deja_trace += 1;
                }
            }
        }
    }
}

*/
