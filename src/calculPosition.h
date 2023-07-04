#ifndef _CALCUL_POSITION_H_
#define _CALCUL_POSITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>


#include "graph.h"

#define ATTRACTION_TROU 0.000007


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

/**
 * @brief modifie la direction de la navette en fonction de sa distance au Trou Noir
 * @param directionXN adresse de la direction X de la navette
 * @param directionYN adresse de la direction Y de la navette
 * @param xTN coord X du trou Noir
 * @param yTN coord Y du trou Noir
 * @param x adresse de la coord X de la navette
 * @param y adresse de la coord Y de la navette
 * @param speedX adresse de la vitesse sur X de la navette
 * @param speedY adresse de la vitesse sur Y de la navette
*/
void attractionTN(float * directionXN, float * directionYN, int xTN, int yTN, float x, float y , float * speedX, float * speedY);

/**
 * @brief Initialise la position du Trou noir assez loin de la navette
 * @param xTN pointeur sur la coord X du Trou Noir
 * @param yTN pointeur sur la coord Y du Trou Noir
 * @param x la coord X de la navette
 * @param y la coord Y de la navette
*/
void initPosTN(float * xTN, float * yTN, float x, float y);

#endif