#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"
#include "OptiFloyd_Warshall.h"

/**
 * @brief Regarde si le noeud d'index i est dans le chemin. Renvoie 1 si vrai, 0 si faux
 * @param i L'index
 * @param chemin Le tableau du chemin
*/
int est_dans_chemin(int i, int* chemin) {
    int j = 0; //Incrément
    int retour = 0; //Initialisé à faux

    while(chemin[j] != -1) {
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
    int n_s_graphe;
    int all[n];
    int ** warshallDist;

    int scoreFourmi, scoreFloyd, scoreBest, score;

    int ** distMat = dist_tab(tab, &n);

    int * chemin_joueur = (int*) malloc(sizeof(int)*MAX_PATH); //Chemin du joueur
    for(i = 0; i < n; i += 1) //Initialisation du chemin du joueur
        chemin_joueur[i] = -1;

    SDL_bool program_on = SDL_TRUE; //Booléen de boucle de jeu
    SDL_Event event;

    int x, y; //Position de la souris au moment du clic

    sommet_t** sous_graphe; //Sous-graphe chemin pour affichage

    init(tab, n); //Affichage du graphe
    int update = 1;
    int valid  = 0;

    warshallDist = copie_tab(distMat, n);
    Floyd_Warshall(warshallDist, n);
    scoreFloyd  = multi_Start_Floyd_Warshall(warshallDist, distMat, n, tab);
    scoreFourmi = multi_start_fourmi(distMat, n);

    //printf("%d, %d\n", scoreFloyd, scoreFourmi);

    //Boucle de jeu
    while (program_on) {
        //Gestion des événements
        while (SDL_PollEvent(&event))
        {
            update = 1;
            switch (event.type)
            {
                
                //pour fermer la fenetre quand on clique sur la croix
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                
                //Détection des clics
                case SDL_MOUSEBUTTONDOWN:
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT: //Si on a un clic gauche
                            
                            //Pour voir si on clique sur un noeud
                            for(i = 0; i < n; i+=1) { //On parcour tous les noeuds
                                //On regarde si le clic est dans un carré autour du noeud
                                SDL_GetMouseState(&x, &y);
                                if((x >= (tab[i]->x)-r) && (x <= (tab[i]->x)+r) && (y >= (tab[i]->y)-r) && (y <= (tab[i]->y)+r)) {
                                    if (nb_noeuds_chemin == 0 || tab[i]->voisins[chemin_joueur[nb_noeuds_chemin-1]]){
                                        chemin_joueur[nb_noeuds_chemin] = i; //On l'ajoute au chemin
                                        nb_noeuds_chemin += 1;
                                    }
                                }
                            }
                            break;
                        
                        case SDL_BUTTON_RIGHT: //Si on a un clic droit on deselectionne un noeud
                            
                            //Pour voir si on clique sur un noeud
                            for(i = 0; i < n; i+=1) { //On parcour tous les noeuds
                                //On regarde si le clic est dans un carré autour du noeud
                                SDL_GetMouseState(&x, &y);
                                if((x >= (tab[i]->x)-r) && (x <= (tab[i]->x)+r) && (y >= (tab[i]->y)-r) && (y <= (tab[i]->y)+r)) {
                                    if (nb_noeuds_chemin != 0 && i == chemin_joueur[nb_noeuds_chemin-1]){
                                        chemin_joueur[nb_noeuds_chemin-1] = -1; //on met le dernier noeud du tableau à -1 valeur impossible
                                        nb_noeuds_chemin -= 1; //on retire 1 au nombre de noeud
                                    }
                                }
                            }
                            break;

                    }
                    break;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_SPACE: //Si espace on remet le nombre de noeud à 0
                            nb_noeuds_chemin = 0;
                            break;
                        case SDLK_RETURN://Si entrer on verifie que la selection est valide (cycle complet) 
                                         //Si oui on passe dans l'etat de fin de jeu (valid = 1)  
                        
                            for (int i = 0; i < n; i++){
                                all[i] = 0;
                            }

                            for (int i = 0; i < nb_noeuds_chemin; i++){
                                all[chemin_joueur[i]] = 1;
                            }

                            if (tout_noeud(all, n)){
                                valid = 1;
                            }
                             
                            
                            break;

                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        if (update){
            update = 0;
            clear_SDL();//Clear la fenêtre (la remetre blanc)

            if(!valid) {//Etat jeu en cour
                affiche(tab, n, 0, 0, 0, 255, 1);
                sous_graphe = chemin_en_graphe(chemin_joueur, nb_noeuds_chemin, tab, n, &n_s_graphe);
                affiche(sous_graphe, n_s_graphe, 255, 0, 0, 255, 0);

                draw_path(tab, chemin_joueur, nb_noeuds_chemin);
                draw_int(path_size_round(chemin_joueur, distMat, nb_noeuds_chemin));
            }
            else {//Etat de fin de jeu 
                score       = path_size(chemin_joueur, distMat, nb_noeuds_chemin); //Score du joueur

                scoreBest = scoreFourmi;
                if (scoreFloyd < scoreFourmi){ //recherche quelle est le meilleur score obtenu entre les differents algo et le joueur
                    scoreBest = scoreFloyd;
                }

                if (scoreBest > score){
                    scoreBest = score;
                }
                
                

                afficheFin(score, scoreBest);//On affiche l'ecran de fin 
            }

            render();//rendre les differents elements
        }
    }
    free(chemin_joueur);
    closeSDL();//free de tout les elements de SDL
}
