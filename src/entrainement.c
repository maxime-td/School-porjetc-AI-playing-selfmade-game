#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"

/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
int* gen_tableau_alea(int n) {
    int i, j; //Incréments
    int * tab_final = (int *) malloc(sizeof(int)*(n)); //Initialisation du tableau qu'on renvoie
    int * tab_temp = (int *) malloc(sizeof(int)*(n)); //Initialisation du tableau temporaire

    //Initialisation du tableau temporaire
    for(i = 0; i < n; i++)
        tab_temp[i] = i;

    for(i = 0; i < n; i++) {
        j = rand()%n; //On prend un j aléatoire
        while(tab_temp[j] == -1) { //Parcours de tab_temp si on a déjà pris cette valeur
            j++;
            if(j == n) //Retour au début du tableau si on l'a parcouru en entier
                j = 0;
        }
        tab_final[i] = tab_temp[j]; //On met la valeur aléatoire dans tab_final
        tab_temp[j] = -1; //Pour ne pas reprendre cette valeur
    }
    
    free(tab_temp); //On libère le tableau temporaire
    return tab_final;
}
