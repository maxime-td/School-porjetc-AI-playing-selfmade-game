
#ifndef THREADS_H
#define THREADS_H

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

// Structure pour les arguments du Timer
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
    double * x;
    double * y;
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

#endif