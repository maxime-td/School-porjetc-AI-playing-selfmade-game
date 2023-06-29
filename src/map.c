#include <stdio.h>
#include "graph.h"
#include "affiche.h"
#include "map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void ast_Partout(SDL_Renderer *render, sommet_t ** tab, int n)
{
    int frame_size = 48;
    SDL_Surface
        *surf = IMG_Load("./images/planetes.png");

    SDL_Texture *texture = NULL;

    int alea = 0;
    int angle = 0;
    int ecart = (rand() % 20) - 10;

    SDL_Rect srcrect = {0, 0, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 15, 15};             // position and size of the destination on the screen

    texture = SDL_CreateTextureFromSurface(render, surf);

    for (int i = 0; i < W; i += 15 + ecart)
    {
        ecart = (rand() % 20) - 10;
        dstrect.x = i;
        for (int j = 0; j < H; j += 15 + ecart)
        {
            ecart = (rand() % 20) - 10;
            dstrect.y = j;
            alea = (rand() % 16) * 48;
            //printf("%d\n", alea);
            srcrect.x = alea;
            angle = rand() % 360;
            //coord_sur_chemin(dstrect.x, dstrect.y, tab, n, frame_size, 80);
            
            
            if(isInPath(dstrect.x, dstrect.y, tab, n, 120)==0)
            {SDL_RenderCopyEx(render, texture, &srcrect, &dstrect, angle, NULL, 0);}
            
                Point p = {3,3};
                Point p1 = {0,0};
                Point p2 = {6,0};
                Point p3 = {0,6};
                Point p4 = {6,6};
                Point tP[4] = {p1,p2,p3,p4};
                printf("AAAAAAAAAAAAAAAAAAAAAAAAA %d\n",isPointInsideRectangle(p, tP));
        }
    }
    SDL_RenderPresent(render);

    // SDL_Delay(3000);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
}

float fonction_affine(float a, int x, float b)
{
    return a*x+b;
}


float distance(Point p1, Point p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrt(dx*dx + dy*dy);
}

void remp_tabPm(Point p1, Point p2, Point * tab)
{
    tab[0].x = (p1.x+p2.x)/2-12;
    tab[0].y = (p1.y+p2.y)/2-12;
    tab[1].x = ((p1.x+tab[0].x)/2)-12;
    tab[1].y = ((p1.y+tab[0].y)/2)-12;
    tab[2].x = ((p1.x+tab[1].x)/2)-12;
    tab[2].y = ((p1.y+tab[1].y)/2)-12;
    tab[3].x = ((p1.x+tab[2].x)/2)-12;
    tab[3].y = ((p1.y+tab[2].y)/2)-12;
    tab[4].x = ((p1.x+tab[3].x)/2)-12;
    tab[4].y = ((p1.y+tab[3].y)/2)-12;
    tab[5].x = (tab[0].x+p2.x)/2-12;
    tab[5].y = (tab[0].y+p2.y)/2-12;
    tab[6].x = (tab[5].x+p2.x)/2-12;
    tab[6].y = (tab[5].y+p2.y)/2-12;
    tab[7].x = (tab[6].x+p2.x)/2-12;
    tab[7].y = (tab[6].y+p2.y)/2-12;
    tab[8].x = (tab[7].x+p2.x)/2-12;
    tab[8].y = (tab[7].y+p2.y)/2-12;
}

int isInPath(int pX, int pY, sommet_t **tabSom, int n, int largeur)
{
    int res=0;
    int dist;
    int nbCarre;
    Point P = {pX,pY};
    Point carre[4];
    Point pmTab[9];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(tabSom[i]->voisins[j]==1)
            {
                Point p1 = {tabSom[i]->x,tabSom[i]->y};
                Point p2 = {tabSom[j]->x,tabSom[j]->y};
                nbCarre = (int)(dist/10);
                remp_tabPm(p1, p2, pmTab);
                for(int k=0; k<9; k++)
                {
                    carre[0].x = pmTab[k].x-(largeur/2);
                    carre[0].y = pmTab[k].y-(largeur/2);

                    carre[1].x = pmTab[k].x+(largeur/2);
                    carre[1].y = pmTab[k].y-(largeur/2);

                    carre[2].x = pmTab[k].x-(largeur/2);
                    carre[2].y = pmTab[k].y+(largeur/2);

                    carre[3].x = pmTab[k].x+(largeur/2);
                    carre[3].y = pmTab[k].y+(largeur/2);
                    SDL_Rect  rect = {carre[0].x, carre[0].y, largeur, largeur};
                   // draw_rect(rect);
                    
                    if(isPointInsideRectangle(P, carre)==1)
                    {
                        res+=1;
                    }
                }
            }
        }
    }
    return res;
}



/*
int isPointOnPath(Point p1, Point p2, Point p, float width) {
    float d1 = distance(p1, p);
    float d2 = distance(p1, p2);
    float d = fabs((p2.x - p1.x) * (p1.y - p.y) - (p1.x - p.x) * (p2.y - p1.y)) / d2;
    
    if (d1 <= d2 && d <= width) {
        return 1; // Le point est sur le chemin
    }
    
    return 0; // Le point n'est pas sur le chemin
}
*/

/**
 * @brief Permet de savoir si un objet est dans un chemin ou s'il est en dehors de tous
 * @param pX position x de l'objet
 * @param pY position y de l'objet
 * @param tabSomm tableau de sommet
 * @param n nombre de sommet
 * @param frame_size dimension de l'objet
 * @param largeur largeur du chemin
 * @return 1 si l'objet est sur un chemin, 0 sinon
*/
/*
int coord_sur_chemin(int pX, int pY, sommet_t **tabSom, int n, int frame_size, int largeur)
{
    
    float x1, x2, x3, x4, y1, y2, y3, y4;
    float a1, a2, a3, a4, b1, b2, b3, b4;
    int interieurUnite=0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(tabSom[i]->voisins[j]==1)
            {
                x1 = tabSom[i]->x;           y1 = tabSom[i]->y ;
                x2 = tabSom[i]->x; y2 = tabSom[i]->y;
                x3 = tabSom[j]->x ; y3 = tabSom[j]->y;
                x4 = tabSom[j]->x;           y4 = tabSom[j]->y ;

                Point p = {pX,pY};
                Point p1 = {x1,y1};
                Point p2 = {x2,y2};
                Point p3 = {x3,y3};
                Point p4 = {x4,y4};
                interieurUnite=isPointOnPath(p1, p2, p, largeur);

                calculateLineCoefficients(x1, y1, x3, y3, &a1, &b1);
                calculateLineCoefficients(x2, y2, x4, y4, &a2, &b2);
                calculateLineCoefficients(x1, y1, x2, y2, &a3, &b3);
                calculateLineCoefficients(x4, y4, x3, y3, &a4, &b4);
                for(int i=0; i<700; i+=10)
                {
                    draw_disk(i, fonction_affine(a1,i,b1), 3);
                }
                                for(int i=0; i<700; i+=10)
                {
                    draw_disk(i, fonction_affine(a2,i,b2), 3);
                }
                                for(int i=0; i<700; i+=10)
                {
                    draw_disk(i, fonction_affine(a3,i,b3), 3);
                }
                                for(int i=0; i<700; i+=10)
                {
                    draw_disk(i, fonction_affine(a4,i,b4), 3);
                }
                if(pY+frame_size/2<=fonction_affine(a1, pX, b1) && pY-frame_size/2>=fonction_affine(a2, pX, b2)){interieurUnite=1; break;}
                if(pY+frame_size/2<=fonction_affine(a3, pX, b3) && pY-frame_size/2>=fonction_affine(a4, pX, b4)){interieurUnite=1; break;}
                Point p = {pX,pY};
                Point p1 = {x1,y1};
                Point p2 = {x2,y2};
                Point p3 = {x3,y3};
                Point p4 = {x4,y4};
                Point tabP[4] = {p1,p2,p3,p4};

//                interieurUnite = isPointInsideRectangle(p, tabP);
                
                //printf("chemin %d->%d : a1 = %f\n",i ,j ,a1);
            }
        }
    }
    
    Point p = {10,10};
    Point p1 = {10,10};
    Point p2 = {12,8};
    Point p3 = {2,2};
    Point p4 = {4,0};
    Point tabP[4] = {p1,p2,p3,p4};
    interieurUnite = isPointInsideRectangle(p, tabP);
    printf("interieur? %d\n", interieurUnite);
    
    return interieurUnite;
}
*/


int isPointInsideRectangle(Point p, Point rect[4]) {
    int c=0;
    printf("rect[0].x = %d -- rect[1].x = %d -- rect[0].y = %d -- rect[1].y = %d",rect[0].x,rect[1].x,rect[0].y,rect[1].y);
    if(rect[0].x<p.x && rect[1].x>p.x && rect[0].y<p.y && rect[3].y>p.y)
    {
        c=1;
    }
    return c;
}


void calculateLineCoefficients(int x1, int y1, int x2, int y2, float *a, float *b) {
    if (x1 > x2) {
        // Inverser les points
        int tempX = x1;
        int tempY = y1;
        x1 = x2;
        y1 = y2;
        x2 = tempX;
        y2 = tempY;
    }
    
    *a = (float)(y2 - y1) / (x2 - x1);
    *b = y1 - (*a) * x1;
}

/*
void passage_Ast(SDL_Renderer * render, sommet_t ** tabsomm)
{

}*/