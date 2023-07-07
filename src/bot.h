#ifndef _BOT_H_
#define _BOT_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"

#define N_RULE 5
#define TIME_MAX_IA 10000000

/**
 * @brief Trouve l'index du sommet qui a la plus courte distance avec le point p
 * @param p Position à comparer
 * @param tab Le tableau de sommet
 * @param n Le nombre de sommet
 * @param planeteVisit Les index des planete déjà faite
 * @return index du sommet le plus proche
 */
int closest_point(Point p, sommet_t ** tab, int n, int * planeteVisit);

/**
 * @brief Donne la position relative du point 2 par rapport au point 1 
 * @param p1 Point 1
 * @param p2 Point 2
 * @return position : 0 si en haut à gauche 1 si en haut à droite 2 si en bas à gauche et 3 si en bas à droite
 */
int position_relative(Point p1, Point p2);

/**
 * @brief Donne la position du mur le plus proche par rapport au point donner
 * @param p Point donner
 * @param tab Tableau des sommet
 * @param n Le nombre de sommet
 * @param depth plus depth grand plus on va chercher des mur loins
 * @param precision plus elle est grande moins il y a de chance de loupé un mur mais la verification serra plus longue
 * @return position : -1 si pas de mur trouver 0 si en haut à gauche 1 si en haut à droite 2 si en bas à gauche et 3 si en bas à droite
 */
int mur_proche(Point p, sommet_t ** tab, int n, segmment_t * segs, int n_seg, int depth, int precision);

/**
 * @brief Cherche si il y a un mur entre les deux points
 * @param p1 Point 1
 * @param p2 Point 2
 * @param tab Tableau des sommet
 * @param n Le nombre de sommet
 * @param precision plus elle est grande moins il y a de chance de loupé un mur mais la verification serra plus longue
 * @return 0 si pas de mur 1 sinon
 */
int is_mur_in_between(Point p1, Point p2, sommet_t ** tab, int n, segmment_t * segs, int n_seg, int precision);

/**
 * @brief Calcul le score de l'ia
 * @param seconde Temps que l'ia à mit pour finir
 * @param nbPlanete Le nombre de planete que l'ia a visité 
 * @param distDep La distance au point de depart
 * @return Le score trouvé
 */
int calcul_score(int seconde, int nbPlanete, int distDep);

/**
 * @brief donne la distance entre deux points de façon de discretise cad (0 proche, 1 moyen, 2 loin)
 * @param p1 premier point
 * @param p2 second point
 * @return distance discrete
*/
int distance_objet(Point p1, Point p2);

/**
 * @brief Génere une regle au hasard
 * @return La règle générée (avec les 2 dernier paramètre representant l'input)
 */
void generate_rule(int * tab_rule);

/**
 * @brief genere un tableau de n regles aleatoire
 * @param n nombre de regle a generer
 * @return le tableau de regle
*/
void generate_tab_rules(int n, int ** tab_rule);

/**
 * @brief recupere un tableau de regle à partir d'un fichier
 * @param name nom du fichier depuis le quelle on recpuere le tableau de regle
 * @param n permet de recuperer la taille du tableau de regle
 * @return le tableau de regle
*/
void get_rule_from_file(char * name, int * n, int ** tab);

/**
 * @brief enregistre un cerveau dans un fichier
 * @param name une chaine de caractères contenant le nom du ficher où enregistrer le cerveau
 * @param tab_rules le cerveau (tableau de regles)
 * @param n le nombre de regles du cerveau
*/
void set_rules_into_file(char * name, int ** tab_rules, int n);

int where_to_go(Point p1, Point p2, sommet_t ** tab, int n, segmment_t * segs, int n_seg, int depth, int precision);

#endif