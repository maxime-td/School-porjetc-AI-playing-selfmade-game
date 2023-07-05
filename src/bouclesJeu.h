#ifndef INTERACTIONJOUEUR_H
#define INTERACTIONJOUEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"
#include "threads.h"

#define MAX_SPEED 0.005
#define ACCELERATION 0.00000004
#define ACCELERATION_TROU 0.00000004

#define MAX_SPEED_IA 5
#define ACCELERATION_IA 0.00004
#define ACCELERATION_TROU_IA 0.00004

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param n_chemin permet de retourner la taille du chemin choisi par le joueur
 * @param fin permet de savoir si la fonction à était quitter en appuyant sur la crois
 * @return tableau du chemin du joueur
 */
int * boucle_jeu_graphe(sommet_t** tab, int n, int * nb_noeuds_chemin, int *fin);

/**
 * @brief Exécute la boucle de jeu dans l'espace
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param chemin Tableau du chemin choisi par le joueur
 * @param n_chemin Taille du chemin choisi par le joueur
 * @param close Permet de savoir si la boucle à était quitter en appuyant sur la crois
 * @param ia booleen disant si c'est une ia qui joue
 * @param tabIA tableau de regle de l'ia
 * @param n_ia taille du tableau de regle
 * @param result permet de recuperer le score de l'ia
 * @param affiche booleen permetant d'activer ou non l'interface graphique
 * @param use_rand booleen indiquant si l'on utilise rand ou un tableau pregenere
 * @param rand_tab tableau de valeur aleatoire pregenere
 * @param n_rand la taille de ce tableau 
 */
void boucle_jeu_espace(sommet_t **tab, int n, int* close, int ia, int ** tabIA, int n_ia, int * result, int affiche, int fast, int use_rand, int * rand_tab, int n_rand);

/**
 * @brief Exécute la boucle de jeu principal
*/
void boucle_jeu();

/**
 * @brief Exécute la boucle de jeu principal sans la première partie (la partie graph)
 */
void boucle_jeu_sans_graph();

#endif