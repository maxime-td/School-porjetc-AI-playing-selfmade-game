#ifndef _CALCUL_POSITION_H_
#define _CALCUL_POSITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>


#include "graph.h"

#define ATTRACTION_TROU 0.0000007
#define ATTRACTION_TROU_IA 0.007


/**
 * @brief calcul de la direction du trou noir
 * @param directionX permet de recuperer la direction x du trou noir
 * @param directionY permet de recuperer la direction y du trou noir
 * @param x position x du trou noir
 * @param y position y du trou noir
 * @param use_rand booleen indiquant si l'on utilise la fonction rand ou un tableau pregenere
 * @param tab_rand le tableau pregenere
 * @param i_rand l'index de depart sur le tableau
 * @param n_rand taille du tableau
*/
void directionTN(float * directionX, float * directionY, int xTN, int yTN, int use_rand, int * tab_rand, int i_rand, int n_rand) ;

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
void calcul_speed(float directionX, float directionY, float * speedX, float * speedY, float * x, float * y, SDL_Rect * rect, double acceleration, double max_speed);

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
void attractionTN(float * directionXN, float * directionYN, int xTN, int yTN, float x, float y , float * speedX, float * speedY, double attraction);

/**
 * @brief Initialise la position du Trou noir assez loin de la navette
 * @param xTN pointeur sur la coord X du Trou Noir
 * @param yTN pointeur sur la coord Y du Trou Noir
*/
void initPosTN(float * xTN, float * yTN, int use_rand, int * tab_rand, int n_rand, int i_rand);

#endif