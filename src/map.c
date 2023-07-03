#include <stdio.h>
#include "fourmi.h"
#include "graph.h"
#include "affiche.h"
#include "map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

asteroid_t * ast_Partout(/*SDL_Renderer *render,*/ sommet_t ** tab, int n, int * nAst)
{
    int frame_size = 48;

    int alea = 0;
    int angle = 0;
    int ecart = (rand() % 50)-25;
    asteroid_t * tabAst = (asteroid_t *) malloc(sizeof(asteroid_t) * H*W);
    int a = 0;

    SDL_Rect srcrect = {0, 0, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 42, 42};             // position and size of the destination on the screen

    for (int i = 0; i < W; i += 21 + ecart)
    {
        ecart = (rand() % 50)-25;
        dstrect.x = i;
        for (int j = 0; j < H; j += 21 + ecart)
        {
            ecart = (rand() % 50)-25;
            dstrect.y = j;
            alea = (rand() % 16);
            srcrect.x = alea;
            angle = rand() % 360;
            
            if(isInPath(dstrect.x, dstrect.y, tab, n, PATH_SIZE)==0){
                tabAst[a].x = dstrect.x;
                tabAst[a].y = dstrect.y;
                tabAst[a].frame = srcrect.x;
                tabAst[a].angle = angle;
                a++;
            }
        }
    }
    *nAst = a;

    return tabAst;
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
    tab[4].x = ((tab[0].x+tab[1].x)/2)-12;
    tab[4].y = ((tab[0].y+tab[1].y)/2)-12;
    tab[5].x = (tab[0].x+p2.x)/2-12;
    tab[5].y = (tab[0].y+p2.y)/2-12;
    tab[6].x = (tab[5].x+p2.x)/2-12;
    tab[6].y = (tab[5].y+p2.y)/2-12;
    tab[7].x = (tab[6].x+p2.x)/2-12;
    tab[7].y = (tab[6].y+p2.y)/2-12;
    tab[8].x = (tab[5].x+tab[0].x)/2-12;
    tab[8].y = (tab[5].y+tab[0].y)/2-12;

}

int isInPath(int pX, int pY, sommet_t **tabSom, int n, int largeur)
{
    int res=0;
    Point P = {pX,pY};
    Point tmp;
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
                    
                    if(isPointInsideRectangle(P, carre)==1)
                    {
                        res+=1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++){
        tmp.x = tabSom[i]->x;   
        tmp.y = tabSom[i]->y;
        if (distance(tmp, P) < largeur){
            res+=1;
        }
        
    }
    
    return res;
}

int isPointInsideRectangle(Point p, Point rect[4]) {
    int c=0;
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