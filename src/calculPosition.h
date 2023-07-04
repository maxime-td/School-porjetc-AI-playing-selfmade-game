#ifndef _CALCUL_POSITION_H_
#define _CALCUL_POSITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "graph.h"

/**
 * @brief calcul de la direction du trou noir
 * @param directionX permet de recuperer la direction x du trou noir
 * @param directionY permet de recuperer la direction y du trou noir
 * @param x position x du trou noir
 * @param y position y du trou noir
*/
void directionTN(float * directionX, float * directionY, int xTN, int yTN);

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

#endif