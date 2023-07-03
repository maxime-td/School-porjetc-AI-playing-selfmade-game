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

#define MAX_SPEED 0.003
#define ACCELERATION 0.000000025
#define N_RULE 3
#define TIME_MAX_IA 40

// Structure pour les arguments de multi_start_fourmi
typedef struct coordonne_s{
    int x;
    int y;
} coordonne_t;
 
// Structure pour les arguments de multi_start_fourmi
typedef struct FourmiArgs_s{
    int** matDist;
    int n;
    int result;
} FourmiArgs;

// Structure pour les arguments de cycle_Floyd_Warshall
typedef struct {
    int** tabWarshall;
    int** tabDist;
    int n;
    int result;
} FloydWarshallArgs;

// Structure pour les arguments de cycle_Floyd_Warshall
typedef struct {
    SDL_bool * fin;
    int time;
} timerArgs;

typedef struct afficheArgs_s{
    int * count;
    int frame;
    int frameEF;
    int frameFlag;
    int frameTN;
    int type_fin;
    SDL_Rect etoile;
    SDL_Rect etoileFilante;
    SDL_Rect background;
    SDL_Rect navette;
    SDL_Rect planete;
    SDL_Rect flag;
    SDL_Rect affTrouNoir;
    asteroid_t * asteroid;
    SDL_Texture * texture;
    SDL_Texture * textureBg;
    SDL_Texture * textureE1;
    SDL_Texture * textureE2;
    SDL_Texture * textureEF;
    SDL_Texture * textureP;
    SDL_Texture * textureF;
    SDL_Texture * textureTN;
    int * fin;
    int * planeteVisite;
    sommet_t ** sous_graphe;
    sommet_t ** tab;
    int n_sous_graphe;
    int n;
    int n_ast;
    SDL_bool * program_on;
    int * chemin;
    coordonne_t * co;
    float * x;
    float * y;
}afficheArgs;

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @return tableau du chemin du joueur
*/
int * boucle_jeu_graphe(sommet_t** tab, int n, int * nb_noeuds_chemin, int *fin);

/**
 * @brief Exécute la boucle de jeu dans l'espace
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param chemin Tableau du chemin choisi par le joueur
*/
void boucle_jeu_espace(sommet_t **tab, int n, int *chemin, int n_chemin, int* close, int ia, int ** tabIA, int n_ia, int * result, int affiche);

/**
 * @brief Exécute la boucle de jeu principal
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
*/
void boucle_jeu();

void boucle_jeu_sans_graph();

/**
 * @brief Trouve l'index du sommet qui a la plus courte distance avec le point p
 * @param p Position à comparer
 * @param tab Le tableau de sommet
 * @param n Le nombre de sommet
 * @param planeteVisit Les index des planete déjà faite
 * @return index du sommet le plus proche
 */
int closest_point(Point p, sommet_t ** tab, int n, int * planeteVisit);

/**
 * @brief Donne la position relative du point 2 par rapport au point 1 
 * @param p1 Point 1
 * @param p2 Point 2
 * @return position : 0 si en haut à gauche 1 si en haut à droite 2 si en bas à gauche et 3 si en bas à droite
 */
int position_relative(Point p1, Point p2);

/**
 * @brief Donne la position du mur le plus proche par rapport au point donner
 * @param p Point donner
 * @param tab Tableau des sommet
 * @param n Le nombre de sommet
 * @param depth plus depth grand plus on va chercher des mur loins
 * @param precision plus elle est grande moins il y a de chance de loupé un mur mais la verification serra plus longue
 * @return position : -1 si pas de mur trouver 0 si en haut à gauche 1 si en haut à droite 2 si en bas à gauche et 3 si en bas à droite
 */
int mur_proche(Point p, sommet_t ** tab, int n, int depth, int precision);

/**
 * @brief Cherche si il y a un mur entre les deux points
 * @param p1 Point 1
 * @param p2 Point 2
 * @param tab Tableau des sommet
 * @param n Le nombre de sommet
 * @param precision plus elle est grande moins il y a de chance de loupé un mur mais la verification serra plus longue
 * @return 0 si pas de mur 1 sinon
 */
int is_mur_in_between(Point p1, Point p2, sommet_t ** tab, int n, int precision);

/**
 * @brief Calcul le score de l'ia
 * @param seconde Temps que l'ia à mit pour finir
 * @param nbPlanete Le nombre de planete que l'ia a visité 
 * @param distDep La distance au point de depart
 * @return Le score trouvé
 */
int calcul_score(int seconde, int nbPlanete, int distDep);

void * afficheJeu(afficheArgs * argsAff);

#endif