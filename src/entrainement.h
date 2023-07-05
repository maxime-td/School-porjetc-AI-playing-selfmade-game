#ifndef _ENTRAINEMENT_H_
#define _ENTRAINEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"

/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
int* gen_tableau_alea(int n);

#endif