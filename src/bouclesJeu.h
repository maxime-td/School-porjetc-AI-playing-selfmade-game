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

#define MAX_SPEED 0.0025
#define ACCELERATION 0.000000015
#define ACCELERATION_TROU 0.00000002

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

// Structure pour les arguments de affiche
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
 * @brief fonction de lancement de thread pour l'algorithme des fourmis
 * @param args Une structure contenant tout les argument a donner a multi_start_fourmi
*/
void *thread_fourmi(FourmiArgs *args);

/**
 * @brief fonction de lancement de thread pour l'algorithme des floyd
 * @param args Une structure contenant tout les argument a donner a multi_Start_Floyd_Warshall
*/
void *thread_floyd(FloydWarshallArgs *args);

/**
 * @brief fonction de lancement de thread pour le timer du jeu
 * @param args Une structure contenant tout les argument necessaire au timer et à la recuperation de ce timer
*/
void *timer(timerArgs* timer);

/**
 * @brief fonction de lancement de thread pour l'affichage graphique du jeu
 * @param args Une structure contenant tout les argument necessaire a l'affichage du jeu
*/
void * afficheJeu(afficheArgs * argsAff);

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
 */
void boucle_jeu_espace(sommet_t **tab, int n, int *chemin, int n_chemin, int* close, int ia, int ** tabIA, int n_ia, int * result, int affiche);

/**
 * @brief Exécute la boucle de jeu principal
*/
void boucle_jeu();

/**
 * @brief Exécute la boucle de jeu principal sans la première partie (la partie graph)
 */
void boucle_jeu_sans_graph();

/**
 * @brief calcul de la direction du trou noir
 * @param directionX permet de recuperer la direction x du trou noir
 * @param directionY permet de recuperer la direction y du trou noir
 * @param xTN position x du trou noir
 * @param yTN position y du trou noir
*/
void directionTN(float * directionX, float * directionY, int xTN, int yTN);

/**
 * @brief calcul de la vitesse d'un objet
 * @param directionX direction x du trou noir
 * @param directionY direction y du trou noir
 * @param speedX permet de recuperer la vitesse x du trou noir
 * @param speedY permet de recuperer la vitesse y du trou noir
 * @param x pemet de recuperer la nouvelle position x du trou noir
 * @param y pemet de recuperer la nouvelle position y du trou noir
 * @param rect le rectangle representant le trou noir 
*/
void calcul_speed(float directionX, float directionY, float * speedX, float * speedY, float * x, float * y, SDL_Rect * rect, double acceleration);

/**
 * @brief calcul la direction de la navette en fonction des touches pressee
 * @param keyPressZ booleen indiquant si z est pressee
 * @param keyPressS booleen indiquant si s est pressee
 * @param keyPressQ booleen indiquant si q est pressee
 * @param keyPressD booleen indiquant si d est pressee
 * @param directionX permet de recuperer la direction x trouve
 * @param directionY permet de recuperer la direction y trouve
*/
void calcul_direction_navette(int keyPressZ, int keyPressS, int keyPressQ, int keyPressD, float * directionX, float * directionY);

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

/**
 * @brief Génere une regle au hasard
 * @return La règle générée (avec les 2 dernier paramètre representant l'input)
 */
int * generate_rule();

/**
 * @brief genere un tableau de n regles aleatoire
 * @param n nombre de regle a generer
 * @return le tableau de regle
*/
int ** generate_tab_rules(int n);

/**
 * @brief recupere un tableau de regle à partir d'un fichier
 * @param name nom du fichier depuis le quelle on recpuere le tableau de regle
 * @param n permet de recuperer la taille du tableau de regle
 * @return le tableau de regle
*/
int ** get_rule_from_file(char * name, int * n);

#endif