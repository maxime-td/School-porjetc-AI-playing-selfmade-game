#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "map.h"

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
void directionTN(float * directionX, float * directionY, int xTN, int yTN, int use_rand, int * tab_rand, int i_rand, int n_rand) {
    if(use_rand) n_rand = 1;
    
    int tirage;
    tirage = (use_rand) ? (rand()%20)-10 : (tab_rand[i_rand]%20)-10;
    i_rand = (i_rand+1)%n_rand;

    if(*directionY<0) {
        tirage = (use_rand) ? (rand()%20-4) : (tab_rand[i_rand]%20) - 4;
        if(yTN<H/4){tirage *= 2;}
        i_rand = (i_rand+1)%n_rand;
    }
    else if(*directionY>0) {
            tirage = (use_rand) ? (rand()%20)-16 : (tab_rand[i_rand]%20) - 16;
            if(yTN>3*H/4)
                tirage *= 2;
            i_rand = (i_rand+1)%n_rand;
    }

    *directionY += ((float)tirage)/200;
    tirage = (use_rand) ? (rand()%20)-10 : (tab_rand[i_rand]%20) - 10;
    i_rand = (i_rand+1)%n_rand;

    if(*directionX<0) {
            tirage = (use_rand) ? (rand()%20-4) : (tab_rand[i_rand]%20) - 4;
            if(xTN<W/4)
                tirage *= 2;
            i_rand = (i_rand+1)%n_rand;
    }    
    else if(*directionX>0) {
            tirage = (use_rand) ? (rand()%20)-16 : (tab_rand[i_rand]%20) - 16;
            if(xTN>3*W/4)
                tirage *= 2;
            i_rand = (i_rand+1)%n_rand;
    }

    *directionX += ((float)tirage)/200;
}

/**
 * @brief calcul la direction de la navette en fonction des touches pressee
 * @param keyPressZ booleen indiquant si z est pressee
 * @param keyPressS booleen indiquant si s est pressee
 * @param keyPressQ booleen indiquant si q est pressee
 * @param keyPressD booleen indiquant si d est pressee
 * @param directionX permet de recuperer la direction x trouve
 * @param directionY permet de recuperer la direction y trouve
*/
void calcul_direction_navette(int keyPressZ, int keyPressS, int keyPressQ, int keyPressD, float * directionX, float * directionY){
    if (keyPressZ)
        (*directionY) += -0.5;

    if (keyPressS)
        (*directionY) += 0.5;

    if (!keyPressZ && !keyPressS)
        (*directionY) = 0;

    if (keyPressQ)
        (*directionX) += -0.5;
        
    if (keyPressD)
        (*directionX) += 0.5;

    if (!keyPressD && !keyPressQ)
        (*directionX) = 0;

    if (fabs((*directionX)) + fabs((*directionY)) == 0.5) {
            (*directionX) *= 2;
            (*directionY) *= 2;
        }
}

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
void calcul_speed(float directionX, float directionY, float * speedX, float * speedY, float * x, float * y, SDL_Rect * rect, double acceleration, double max_speed){

    *speedX += directionX * acceleration;
    *speedY += directionY * acceleration;

    if (directionX == 0 && speedX != 0)
    {
        if (*speedX < 0)
        {
            *speedX += acceleration * 0.25;
        }
        else
        {
           *speedX -= acceleration * 0.25;
        }

        if (*speedX < acceleration && *speedX > -acceleration)
        {
            *speedX = 0;
        }
    }

    if (directionY == 0 && *speedY != 0)
    {
        if (*speedY < 0)
        {
            *speedY += acceleration * 0.25;
        }
        else
        {
            *speedY -= acceleration * 0.25;
        }
        if (*speedY < acceleration && *speedY > -acceleration)
        {
            *speedY = 0;
        }
    }

    if (*speedX < -max_speed / 2)
    {
        *speedX = -max_speed / 2;
    }
    else if (*speedX > max_speed / 2)
    {
        *speedX = max_speed / 2;
    }

    if (*speedY < -max_speed / 2)
    {
        *speedY = -max_speed / 2;
    }
    else if (*speedY > max_speed / 2)
    {
        *speedY = max_speed / 2;
    }

    *x += *speedX;
    *y += *speedY;

    if (*x < 0){
        *x = 0;
        *speedX = 0;
        *speedY = 0;
    }else if (*x > W-rect->w){
        *x = W-rect->w;
        *speedX = 0;
        *speedY = 0;
    }

    if (*y < 0){
        *y = 0;
        *speedX = 0;
        *speedY = 0;
    }else if (*y > H-rect->h){
        *y = H-rect->h;
        *speedX = 0;
        *speedY = 0;
    }
    
}

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
void attractionTN(float * directionXN, float * directionYN, int xTN, int yTN, float x, float y , float * speedX, float * speedY, double attraction)
{
    Point navette = {(int)x, (int)y};
    Point trouNoir = {xTN, yTN};
    float dist = distance(navette, trouNoir);
    *directionXN = 0; *directionYN = 0;
    *directionXN += (xTN+24-x);
    *directionYN += (yTN+24-y);

    *speedX += *directionXN * attraction/(dist*dist);
    *speedY += *directionYN * attraction/(dist*dist);
}

/**
 * @brief Initialise la position du Trou noir assez loin de la navette
 * @param xTN pointeur sur la coord X du Trou Noir
 * @param yTN pointeur sur la coord Y du Trou Noir

*/
void initPosTN(float * xTN, float * yTN, int use_rand, int * tab_rand, int n_rand, int i_rand)
{
    if (use_rand) n_rand = 1;
    
    *xTN = use_rand ? rand()%W : tab_rand[i_rand]%W;
    i_rand = (i_rand + 1)%n_rand;

    *yTN = use_rand ? rand()%H : tab_rand[i_rand]%H;
    i_rand = (i_rand + 1)%n_rand;

    Point TN = {*xTN, *yTN};
    /*
    while(distance(navette, TN)<250)
    {
        TN.x = use_rand ? rand()%W : tab_rand[i_rand]%W;
        i_rand = (i_rand + 1)%n_rand;

        TN.y = use_rand ? rand()%H : tab_rand[i_rand]%W;
        i_rand = (i_rand + 1)%n_rand;
    }
    */
    *xTN = TN.x;
    *yTN = TN.y;
}