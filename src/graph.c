#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "graph.h"
#include "affiche.h"

/**
 * Libère la mémoire allouée pour un tableau à deux dimensions.
 *
 * @param tab Le tableau à libérer.
 * @param n   Le nombre de lignes du tableau.
 */
void free2DTab(void ** tab, int n) {
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
void tab_to_graph(sommet_t ** tab, int start, int end, int use_rand, int * tab_rand, int i_rand, int n_rand) {
    if(use_rand) n_rand = 1;

    if(start < end) {
        int k = use_rand ? rand()%(end-start)+(start+1) : tab_rand[i_rand]%(end-start)+(start+1);
        tab[start]->voisins[start+1] = 1;
        tab[start + 1]->voisins[start] = 1;
        tab[start]->n_voisin++;
        tab[start + 1]->n_voisin++;

        if(k+1 <= end) {
            tab[start]->voisins[k+1] = 1; 
            tab[k+1]->voisins[start] = 1;
            tab[start]->n_voisin++;
            tab[k+1]->n_voisin++;   
        }

        tab_to_graph(tab, start+1, k, use_rand, tab_rand, (i_rand+1)%n_rand, n_rand);
        tab_to_graph(tab, k+1, end, use_rand, tab_rand,(k+i_rand)%n_rand, n_rand);
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
    for (int i = 0; i < size; i++) { 
        if ((sqrt((cour->y-tab[i]->y)*(cour->y-tab[i]->y)+(cour->x-tab[i]->x)*(cour->x-tab[i]->x))) <= threshold)
            return 1;
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
sommet_t ** gen_tab_sommets_cercle(int * n) {
    *n = rand()%(N-(N_MIN-1)) + N_MIN;
    int first = 1;
    int angle;
    sommet_t ** tab = malloc((*n)*sizeof(sommet_t *));

    for(int i=0; i<*n; i++) {
        first = 1;
        tab[i] = malloc(sizeof(sommet_t));

        while (first || is_close_to_value_in_tab(tab, i, tab[i], (4*R/(*n)))) {
            first = 0;
            angle = rand()%(360);
            
            tab[i]->x = cos(angle)*R+W/2;
            tab[i]->y = sin(angle)*R+H/2; //On génère les nombre aléatoirement entre des bornes représentants la taille de la fenêtre
        }

        tab[i]->val = i+'A'; //On assigne des valeurs aux sommet, en l'occurence A,B,C...

        for(int j=0; j<*n;j++)
            tab[i]->voisins[j]=0;    //Tableau des liens initialisé vide
    }

    return tab;
}

sommet_t ** gen_tab_sommets_rand(int * n, int use_rand, int * tab_rand, int n_rand, int i_rand) {
    if(use_rand) n_rand = 1;

    *n = use_rand ? rand()%(N-(N_MIN-1)) + N_MIN : tab_rand[i_rand]%(N-(N_MIN-1)) + N_MIN;
    i_rand = (i_rand+1)%n_rand;
    int first = 1;
    int x,y;
    sommet_t ** tab = malloc((*n)*sizeof(sommet_t *));

    for(int i=0; i<*n; i++) {
        first = 1;
        tab[i] = malloc(sizeof(sommet_t));
        while (first || is_close_to_value_in_tab(tab, i, tab[i], 100)) {
            first = 0;
            x = use_rand ? rand()%(W-200)+90 : tab_rand[i_rand]%(W-200)+90;
            i_rand = (i_rand+1)%n_rand;
            y = use_rand ? rand()%(H-200)+90 : tab_rand[i_rand]%(W-200)+90;
            i_rand = (i_rand+1)%n_rand;
        
            tab[i]->x = x;
            tab[i]->y = y;        //On génère les nombre aléatoirement entre des bornes représentants la taille de la fenêtre
        }

        tab[i]->val = i+'A';             //On assigne des valeurs aux sommet, en l'occurence A,B,C...

        for(int j=0; j<*n;j++)
            tab[i]->voisins[j]=0;    //Tableau des liens initialisé vide
    }

    return tab;
}

/**
 * @brief Affiche le tableau des coordonnées des points
 * @param tab le pointeur sur le tableau de points
 * @param n le pointeur sur le nombre de sommets du tableau
*/
void print_tab_coord(sommet_t ** tab, int * n) {
    printf("nombre de points: %d\n", *n);
    for(int i=0; i<(*n); i++) {
        printf("%c : (%d,%d) :\n",tab[i]->val ,tab[i]->x, tab[i]->y);
        for (int j = 0; j < (*n); j++)
            printf("%d ", tab[i]->voisins[j]);
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
void make_new_links(int p, sommet_t ** tab, int * n, int use_rand, int * tab_rand, int n_rand, int i_rand) {
    if(use_rand) n_rand = 1; 

    int random = 0;
    for(int i=0; i<*n; i++) {
        for(int j=0; j<*n; j++) {

            random = use_rand ? rand()%100 : tab_rand[i_rand]%100;
            i_rand = (i_rand + 1)%n_rand;

            if(i!=j && tab[i]->voisins[j]==0 && random<p) { // On regarde pour chaque point son tableau "binaire" de voisins et on tire un random entre 0 et 100
                tab[i]->voisins[j]=1;          // Si 2 points ne sont pas voisins et qu'on tire un random respectant notre porba souhaitée, on lie les points.
                tab[j]->voisins[i]=1;
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
int calcul_distance(sommet_t * a, sommet_t * b) {
    int tmp1 = (a->x-b->x), tmp2 = (a->y-b->y);

    return (int) round(sqrt(tmp1*tmp1+tmp2*tmp2)/10);
}

/**
 * @brief Créée un tableau de distance entre les points, -1 si non lié, > 0 sinon.
 * @param tab pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 * @return Le pointeur sur le tableau 2D des distances.
 */
int ** dist_tab(sommet_t ** tab, int * n) {
    int ** tabDist = malloc((*n)*sizeof(int *));
    int tmpDist=0;
    for(int i=0; i<*n; i++) {
        tabDist[i] = malloc((*n)*sizeof(int));
        for(int j=0; j<=i; j++) {
            if(i!=j && tab[i]->voisins[j]==1) {
                tmpDist = calcul_distance(tab[i], tab[j]);
                tabDist[i][j] = tmpDist;
                tabDist[j][i] = tmpDist;
            }
            else if(i!=j) {
                tabDist[i][j] = -1;
                tabDist[j][i] = -1;
            }
            else
                tabDist[j][i] = 0;
        }
    }

    return tabDist;
}

/**
 * @brief Affiche le tableau des distances
 * @param dist_tab le tableau des distances
 * @param n le pointeur sur le ombre de sommets
 */
void print_dist_tab(int ** dist_tab, int * n) {
    for(int i=0; i<(*n); i++) {
        for(int j=0; j<(*n); j++)
            printf("distance (%c,%c) : %d -- ", i+65, j+65, dist_tab[i][j]);
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief Transforme un chemin en sous-graphe pour le tracé avec draw_graph
 * @param chemin Le chemin
 * @param n_chemin Le nombre de noeud dans le chemin
 * @param tab Le graphe initial
 * @param n Le nombre de noeuds dans le graphe initial
*/
sommet_t** chemin_en_graphe(int * chemin, int n_chemin, sommet_t** tab, int n, int* n_sous_graph) {
    //Initialisation
    sommet_t ** sous_graphe_chemin = NULL;

    int i, j = 0, k; //Incréments
    int noeuds_dans_chemin [n]; //Tableau des noeuds dans le chemin
    int n_noeuds_differents = 0; //Nombre de noeuds uniques dans le chemin
    int conv_tab_to_n_chemin[n];

    //Initialisation noeuds_dans_chemin
    for(i = 0; i < n; i++)
        noeuds_dans_chemin[i] = 0;

    //On note les noeuds dans le chemin dans noeuds_dans_chemin
    for(i = 0; i < n_chemin; i++)
        noeuds_dans_chemin[chemin[i]] = 1;

    //On compte le nombre de noeuds différents dans noeuds_dans_chemin
    for (i = 0; i < n; i++)
        n_noeuds_differents += noeuds_dans_chemin[i];
    
    j = 0;
    for (i = 0; i < n; i++) {
        if(noeuds_dans_chemin[i]) {
            conv_tab_to_n_chemin[i] = j;
            j++;
        }
    }

    sous_graphe_chemin = (sommet_t**)malloc(sizeof(sommet_t*)*n_noeuds_differents); //Allocation sous_graphe_chemin

    j = 0;
    for(i = 0; i < n; i++) {
        if(noeuds_dans_chemin[i]) {
            sous_graphe_chemin[j] = (sommet_t*)malloc(sizeof(sommet_t));
            sous_graphe_chemin[j]->val = tab[i]->val;
            sous_graphe_chemin[j]->x = tab[i]->x;
            sous_graphe_chemin[j]->y = tab[i]->y;
            for(k = 0; k < n_noeuds_differents; k++)
                sous_graphe_chemin[j]->voisins[k] = 0;
            j++;
        }
    }
    
    for (int i = 0; i < n_chemin-1; i++) {
        sous_graphe_chemin[conv_tab_to_n_chemin[chemin[i]]]->voisins[conv_tab_to_n_chemin[chemin[i+1]]] = 1;
        sous_graphe_chemin[conv_tab_to_n_chemin[chemin[i+1]]]->voisins[conv_tab_to_n_chemin[chemin[i]]] = 1;
    }
    
    *n_sous_graph = n_noeuds_differents;
    
    return sous_graphe_chemin;
}


segmment_t * gen_tab_seg(sommet_t ** tab, int n, int * n_seg){
    segmment_t * seg = (segmment_t *) malloc(sizeof(segmment_t)*(n*n));

    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (tab[i]->voisins[j] == 1) {
                seg[k].p1.x = tab[i]->x;
                seg[k].p1.y = tab[i]->y;
                seg[k].p2.x = tab[j]->x;
                seg[k].p2.y = tab[j]->y;
                k++;
            }
        }
    }
    
    *n_seg = k;
    return seg;
}