#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"

/**
 * @brief Calcul le score de l'ia
 * @param seconde Temps que l'ia à mit pour finir
 * @param nbPlanete Le nombre de planete que l'ia a visité 
 * @param distDep La distance au point de depart
 * @return Le score trouvé
 */
int calcul_score(int seconde, int nbPlanete, int distDep) {
    int score = 0;

    score += (TIME_MAX_IA - seconde)*5;
    score += (TIME_MAX_IA - seconde > 0)*500;
    score += nbPlanete*5;
    score += distDep/20;

    return score;   
}

/**
 * @brief Trouve l'index du sommet qui a la plus courte distance avec le point p
 * @param p Position à comparer
 * @param tab Le tableau de sommet
 * @param n Le nombre de sommet
 * @param planeteVisit Les index des planete déjà faite
 * @return index du sommet le plus proche
 */
int closest_point(Point p, sommet_t ** tab, int n, int * planeteVisit) {
    int res = 0;
    int bestDist = INT_MAX;
    int dist;
    Point p2;

    for (int i = 0; i < n; i++) {
        if(!planeteVisit[i]) {
            p2.x = tab[i]->x;
            p2.y = tab[i]->y;
            dist = distance(p, p2);

            if (dist < bestDist) {
                bestDist = dist;
                res = i;
            }
        }
    }
    return res;
}

/**
 * @brief Donne la position relative du point 2 par rapport au point 1 
 * @param p1 Point 1
 * @param p2 Point 2
 * @return position : 0 si en haut à gauche 1 si en haut à droite 2 si en bas à gauche et 3 si en bas à droite
 */
int position_relative(Point p1, Point p2) {
    if (p1.x > p2.x) {
        if (p1.y > p2.y)
            return 0;
        else
            return 1;   
    }
    else {
        if (p1.y > p2.y)
            return 2;
        else
            return 3;
    } 
}

/**
 * @brief Donne la position du mur le plus proche par rapport au point donné
 * @param p Point donné
 * @param tab Tableau des sommets
 * @param n Le nombre de sommet
 * @param depth Le nombre de pats qui seront fait
 * @param precision La taille d'un pats
 * @return position : 4 si pas de mur trouver 0 si en haut à gauche 1 si en haut à droite 2 si en bas à gauche et 3 si en bas à droite
 */
int mur_proche(Point p, sommet_t ** tab, int n, int depth, int precision) {
    Point direction[4];
    direction[0].x = -precision;
    direction[0].y = -precision;
    direction[1].x =  precision;
    direction[1].y = -precision;
    direction[2].x = -precision;
    direction[2].y =  precision;
    direction[3].x =  precision;
    direction[3].y =  precision;

    int closest = 4;
    int bestDist = depth;

    for (int i = 0; i < 4; i++) {
        for (int j = precision; j < bestDist; j++) {
            if (!isInPath_Line(p.x + direction[i].x*j, p.y + direction[i].y*j, tab, n, PATH_SIZE)) {
                bestDist = j;
                closest = i;
            }
        }
    }

    return closest;
}

/**
 * @brief Cherche si il y a un mur entre les deux points
 * @param p1 Point 1
 * @param p2 Point 2
 * @param tab Tableau des sommet
 * @param n Le nombre de sommet
 * @param precision plus elle est grande moins il y a de chance de loupé un mur mais la verification serra plus longue
 * @return 0 si pas de mur 1 sinon
 */
int is_mur_in_between(Point p1, Point p2, sommet_t ** tab, int n, int precision) {
    Point direction[4];
    direction[0].x = -1;
    direction[0].y = -1;
    direction[1].x =  1;
    direction[1].y = -1;
    direction[2].x = -1;
    direction[2].y =  1;
    direction[3].x =  1;
    direction[3].y =  1;

    int dir_noeud = position_relative(p1, p2);
    int dist = distance(p1, p2);

    for (int i = 0; i < dist; i+=precision)
    {
        if (!isInPath_Line(p1.x + direction[dir_noeud].x*i, p1.y + direction[dir_noeud].y*i, tab, n, PATH_SIZE))
            return 1;
    }

    return 0;
}

/**
 * @brief Génere une regle au hasard
 * @return La règle générée (avec les 2 dernier paramètre representant l'input)
 */
int * generate_rule() {
    int * rule = (int *) malloc(sizeof(int)*(N_RULE+3));

    //Etat du jeu
    rule[0] = (rand()%5)-1;
    rule[1] = (rand()%6)-1;
    rule[2] = (rand()%3)-1;

    //Input du bot en fonction de l'etat
    rule[3] = (rand()%3)-1;
    rule[4] = (rand()%3)-1;
    rule[5] = (rand()%10)+1;

    return rule;
}

/**
 * @brief genere un tableau de n regles aleatoire
 * @param n nombre de regle a generer
 * @return le tableau de regle
*/
int ** generate_tab_rules(int n) {
    int ** tab_rules = (int **) malloc(sizeof(int*)*n);

    for (int i = 0; i < n-1; i++)
        tab_rules[i] = generate_rule();

    tab_rules[n-1] = (int *) malloc(sizeof(int)*(N_RULE+3));

    tab_rules[n-1][0] = -1;
    tab_rules[n-1][1] = -1;
    tab_rules[n-1][2] = -1;
    tab_rules[n-1][3] = (rand()%3)-1;
    tab_rules[n-1][4] = (rand()%3)-1;
    tab_rules[n-1][5] = 1;
    
    return tab_rules;
}

/**
 * @brief recupere un tableau de regle à partir d'un fichier
 * @param name nom du fichier depuis le quelle on recpuere le tableau de regle
 * @param n permet de recuperer la taille du tableau de regle
 * @return le tableau de regle
*/
int ** get_rule_from_file(char * name, int * n) {
    int code;

    FILE * file = fopen(name, "r");
    code = fscanf(file, "%d\n", n);

    int ** tab = (int**) malloc(sizeof(int*)*(*n));

    for (int i = 0; i < *n; i++) {
        tab[i] = (int*) malloc(sizeof(int)*(N_RULE+3));

        for (int j = 0; j < (N_RULE+3); j++)
            code = fscanf(file, "%d\n", &tab[i][j]);
        
        code = fscanf(file, "\n");
    }

    code = fclose(file);
    code += 1;

    return tab;
}
