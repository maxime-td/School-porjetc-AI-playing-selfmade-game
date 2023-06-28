#ifndef INTERACTIONJOUEUR_H
#define INTERACTIONJOUEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "graph.h"
#include "affiche.h"
#include <math.h>

/**
 * @brief Exécute la boucle de jeu
 * @param tab Le tableau des sommets
*/
void boucle_jeu(sommet_t** tab);

#endif