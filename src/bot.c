#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"

/**
 * @brief Calcul le score de l'ia
 * @param seconde Temps que l'ia à mit pour finir
 * @param nbPlanete Le nombre de planete que l'ia a visité 
 * @param distDep La distance au point de depart
 * @return Le score trouvé
 */
int calcul_score(int seconde, int nbPlanete, int distG) {
    int score = 0;

    score += ((TIME_MAX_IA+1) - seconde);
    score += (TIME_MAX_IA - seconde > 0)*1000;
    score += (nbPlanete-1)*100;
    score += 100/((distG/10)+1);
    //printf("Score : %d\n", score);

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
            return 2;   
    }
    else {
        if (p1.y > p2.y)
            return 1;
        else
            return 3;
    } 
}

/**
 * @brief donne la distance entre deux points de façon de discretise cad (0 proche, 1 moyen, 2 loin)
 * @param p1 premier point
 * @param p2 second point
 * @return distance discrete
*/
int distance_objet(Point p1, Point p2){
    int dist = distance(p1, p2);
    if (dist < 150){
        return 0;
    }else if(dist < 500){
        return 1;
    }
    return 2;
}

int where_to_go(Point p1, Point p2, sommet_t ** tab, int n, segmment_t * segs, int n_seg, int depth, int precision){
    Point direction[8];
    Point cour;
    direction[0].x = -precision;
    direction[0].y = -precision;
    direction[1].x =  precision;
    direction[1].y = -precision;
    direction[2].x = -precision;
    direction[2].y =  precision;
    direction[3].x =  precision;
    direction[3].y =  precision;
    direction[4].x = -precision;
    direction[4].y =          0;
    direction[5].x =          0;
    direction[5].y =  precision;
    direction[6].x =  precision;
    direction[6].y =          0;
    direction[7].x =          0;
    direction[7].y = -precision;

    int where = 4;
    int random = 0;

    for (int i = 0; i < 8 && where == 4; i++) {
        for (int j = 16; j <= depth  && where == 4; j++) {
            cour.x = p1.x + direction[i].x*j;
            cour.y = p1.y + direction[i].y*j;
            if (!isInPath_Line(cour.x, cour.y, tab, n, segs, n_seg,  PATH_SIZE)) {
                break;
            }else if(!is_mur_in_between(cour, p2, tab, n, segs, n_seg, precision)){
                if(i < 4) where = i;
                else {
                    random = rand()%2;
                    switch(i){
                        case 4:
                            where = (random ? 0 : 2);
                            break;
                        case 5:
                            where = random + 2;
                            break;
                        case 6:
                            where = (random ? 1 : 3);
                            break;
                        case 7:
                            where = random;
                            break;
                    }
                }
                
            }
        }
    }

    return where;
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
int mur_proche(Point p, sommet_t ** tab, int n, segmment_t * segs, int n_seg, int depth, int precision) {
    Point direction[4];
    direction[0].x = -precision;
    direction[0].y =          0;
    direction[1].x =          0;
    direction[1].y =  precision;
    direction[2].x =  precision;
    direction[2].y =          0;
    direction[3].x =          0;
    direction[3].y = -precision;

    int closest = 4;
    int bestDist = depth;

    for (int i = 0; i < 4; i++) {
        for (int j = 16; j <= bestDist; j++) {
            if (!isInPath_Line(p.x + direction[i].x*j, p.y + direction[i].y*j, tab, n, segs, n_seg,  PATH_SIZE) && (closest == 4 || rand()%4 == 0)) {
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
int is_mur_in_between(Point p1, Point p2, sommet_t ** tab, int n, segmment_t * segs, int n_seg, int precision) {
    float directionX = p2.x - p1.x;
    float directionY = p2.y - p1.y;

    float norm = sqrt(directionX*directionX + directionY*directionY);

    directionX /= norm;
    directionY /= norm;

    int dist = distance(p1, p2);

    for (int i = 16; i < dist; i+=precision)
    {
        if (!isInPath_Line(p1.x + directionX*i, p1.y + directionY*i, tab, n, segs, n_seg, PATH_SIZE))
            return 1;
    }

    return 0;
}

/**
 * @brief Génere une regle au hasard
 * @return La règle générée (avec les 2 dernier paramètre representant l'input)
 */
void generate_rule(int * rule) {
    //Etat du jeu
    rule[0] = (rand()%2 == 0) ? -1 : (rand()%4); //Position planete
    rule[1] = (rand()%2 == 0) ? -1 : (rand()%5); //Position mur
    rule[2] = (rand()%2 == 0) ? -1 : (rand()%2); //Is Mur
    rule[3] = (rand()%2 == 0) ? -1 : (rand()%4); //Pos trou noir
    rule[4] = (rand()%2 == 0) ? -1 : (rand()%3); //Distance trou noir

    //Input du bot en fonction de l'etat
    rule[N_RULE]   = (rand()%3) -1; //Direction haut/bas
    rule[N_RULE+1] = (rand()%3) -1; //Direction droite/gauche

    //Priorite
    rule[N_RULE+2] = (rand()%6)+1;

    //return rule;
}

/**
 * @brief genere un tableau de n regles aleatoire
 * @param n nombre de regle a generer
 * @return le tableau de regle
*/
void generate_tab_rules(int n, int ** tab_rules) {

    
    for (int i = 0; i < n-1; i++){
        tab_rules[i] = (int *) malloc(sizeof(int)*(N_RULE+3));
        generate_rule(tab_rules[i]);
    }

    tab_rules[n-1] = (int *) malloc(sizeof(int)*(N_RULE+3));

    for (int i = 0; i < N_RULE; i++){
        tab_rules[n-1][i] = -1;
    }
    
    tab_rules[n-1][N_RULE]   = (rand()%3)-1;
    tab_rules[n-1][N_RULE+1] = (rand()%3)-1;
    tab_rules[n-1][N_RULE+2] = 1;

}

/**
 * @brief recupere un tableau de regle à partir d'un fichier
 * @param name nom du fichier depuis le quelle on recpuere le tableau de regle
 * @param n permet de recuperer la taille du tableau de regle
 * @return le tableau de regle
*/
void get_rule_from_file(char * name, int * n, int ** tab) {
    int code;
    FILE * file = fopen(name, "r");
    code = fscanf(file, "%d\n", n);

    for (int i = 0; i < *n; i++) {
        tab[i] = (int*) malloc(sizeof(int)*(N_RULE+3));

        for (int j = 0; j < (N_RULE+3); j++)
            code = fscanf(file, "%d\n", &tab[i][j]);
        
        code = fscanf(file, "\n");
    }

    code = fclose(file);
    code += 1;
}

/**
 * @brief enregistre un cerveau dans un fichier
 * @param name une chaine de caractères contenant le nom du ficher où enregistrer le cerveau
 * @param tab_rules le cerveau (tableau de regles)
 * @param n le nombre de regles du cerveau
*/
void set_rules_into_file(char * name, int ** tab_rules, int n)
{
    FILE * f = fopen(name, "w+");
    fprintf(f, "%d\n", n);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<N_RULE+3; j++)
        {
            fprintf(f, "%d ", tab_rules[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
