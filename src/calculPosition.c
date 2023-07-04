#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"

/**
 * @brief calcul de la direction du trou noir
 * @param directionX permet de recuperer la direction x du trou noir
 * @param directionY permet de recuperer la direction y du trou noir
 * @param x position x du trou noir
 * @param y position y du trou noir
*/
void directionTN(float * directionX, float * directionY, int xTN, int yTN) {
    int tirage;
    tirage = (rand()%20)-10;

    if(*directionY<0) {
        tirage = (rand()%20-4);
        if(yTN<H/4){tirage *= 2;}
    }
    else if(*directionY>0) {
            tirage = (rand()%20)-16;
            if(yTN>3*H/4)
                tirage *= 2;
    }

    *directionY += ((float)tirage)/200;
    tirage = (rand()%20)-10;

    if(*directionX<0) {
            tirage = (rand()%20-4);
            if(xTN<W/4)
                tirage *= 2;
    }    
    else if(*directionX>0) {
            tirage = (rand()%20)-16;
            if(xTN>3*W/4)
                tirage *= 2;
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
void calcul_speed(float directionX, float directionY, float * speedX, float * speedY, float * x, float * y, SDL_Rect * rect, double acceleration){
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

    if (*speedX < -MAX_SPEED / 2)
    {
        *speedX = -MAX_SPEED / 2;
    }
    else if (*speedX > MAX_SPEED / 2)
    {
        *speedX = MAX_SPEED / 2;
    }

    if (*speedY < -MAX_SPEED / 2)
    {
        *speedY = -MAX_SPEED / 2;
    }
    else if (*speedY > MAX_SPEED / 2)
    {
        *speedY = MAX_SPEED / 2;
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
