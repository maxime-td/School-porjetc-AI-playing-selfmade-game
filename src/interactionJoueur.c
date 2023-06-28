#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"

/**
 * @brief Regarde si le noeud d'index i est dans le chemin. Renvoie 1 si vrai, 0 si faux
 * @param i L'index
 * @param chemin Le tableau du chemin
*/
int est_dans_chemin(int i, int* chemin) {
    int j = 0; //Incrément
    int retour = 0; //Initialisé à faux

    while(chemin[j] != NULL) {
        if(chemin[j] == i)
            retour = 1;
        j += 1;
    }

    return retour;
}

/**
 * @brief Exécute la boucle de jeu
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
*/
void boucle_jeu(sommet_t** tab, int n) {

    //Initialisation
    int i; //Incrément
    int nb_noeuds_chemin = 0; //Nombre de noeuds dans le chemin
    int r = R_NOEUD; //Rayon des sommets

    int * chemin_joueur = (int*) malloc(sizeof(int)*MAX_PATH); //Chemin du joueur
    for(i = 0; i < n; i += 1) //Initialisation du chemin du joueur
        chemin_joueur[i] = NULL;

    SDL_bool program_on = SDL_TRUE; //Booléen de boucle de jeu
    SDL_Event event;

    int x, y; //Position de la souris au moment du clic

    sommet_t** sous_graphe; //Sous-graphe chemin pour affichage

    init(tab, n);

    //Boucle de jeu
    while (program_on) {
        //Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                //pour fermer la fenetre quand on clique sur la croix
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                
                //Détection des clics
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) { //Si on a un clic gauche

                        //Pour voir si on clique sur un noeud
                        for(i = 0; i < n; i+=1) { //On parcour tous les noeuds

                            //On regarde si le clic est dans un carré autour du noeud
                            SDL_GetMouseState(&x, &y);
                            if((x >= (tab[i]->x)-r) && (x <= (tab[i]->x)+r) && (y >= (tab[i]->y)-r) && (y <= (tab[i]->y)+r)) {
                                chemin_joueur[nb_noeuds_chemin] = i; //On l'ajoute au chemin
                                nb_noeuds_chemin += 1;
                            }
                        }

                    }
                    break;
                
                default:
                    break;
            }
        }
        affiche(tab, n);
        affich_tab(chemin_joueur, nb_noeuds_chemin);
        printf("a\n");
        sous_graphe = chemin_en_graphe(chemin_joueur, nb_noeuds_chemin, tab, n);
        printf("b\n");
        affiche(sous_graphe, nb_noeuds_chemin);
        printf("aa\n");
    }
    free(chemin_joueur);
}