#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"
#include "OptiFloyd_Warshall.h"
#include "bouclesJeu.h"

void* thread_fourmi(FourmiArgs* args){
    args->result = multi_start_fourmi(args->matDist, args->n);
    return NULL;
}


void* thread_floyd(FloydWarshallArgs* args){
    args->result = multi_Start_Floyd_Warshall(args->tabWarshall, args->tabDist, args->n);
    return NULL;
}

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @return tableau du chemin du joueur
*/
int * boucle_jeu_graphe(sommet_t** tab, int n, int * n_chemin) {
    pthread_t thread1, thread2;
    //Initialisation
    int i; //Incrément
    int nb_noeuds_chemin = 0; //Nombre de noeuds dans le chemin
    int r = R_NOEUD; //Rayon des sommets
    int n_s_graphe;
    int all[n];
    int ** warshallDist;
    int first = 1;
    FourmiArgs argsF;
    FloydWarshallArgs argsFl;

    int scoreFourmi, scoreFloyd, scoreBest, score;

    int ** distMat = dist_tab(tab, &n);

    int * chemin_joueur = (int*) malloc(sizeof(int)*MAX_PATH); //Chemin du joueur
    for(i = 0; i < n; i += 1) //Initialisation du chemin du joueur
        chemin_joueur[i] = -1;

    SDL_bool program_on = SDL_TRUE; //Booléen de boucle de jeu
    SDL_Event event;

    int x, y; //Position de la souris au moment du clic

    sommet_t** sous_graphe; //Sous-graphe chemin pour affichage

    warshallDist = copie_tab(distMat, n);
    Floyd_Warshall(warshallDist, n);

    argsFl.n           = n           ;
    argsFl.tabDist     = distMat     ;
    argsFl.tabWarshall = warshallDist;
    pthread_create(&thread2, NULL, (void * (*)(void *))thread_floyd, &argsFl);

    argsF.matDist = distMat;
    argsF.n       = n      ; 
    pthread_create(&thread1, NULL, (void * (*)(void *))thread_fourmi, &argsF);

    int update = 1;
    int etat_graphe = 0; //0 = en jeu graphe, 1 = menu de fin, 2 = en jeu espace

    //Boucle de jeu
    while (program_on) {
        //Gestion des événements
        while (SDL_PollEvent(&event))
        {
            update = 1;
            switch (event.type)
            {
                
                //pour fermer la fenetre quand on clique sur la croix
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                
                //Détection des clics
                case SDL_MOUSEBUTTONDOWN:
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT: //Si on a un clic gauche
                            
                            //Pour voir si on clique sur un noeud
                            for(i = 0; i < n; i+=1) { //On parcour tous les noeuds
                                //On regarde si le clic est dans un carré autour du noeud
                                SDL_GetMouseState(&x, &y);
                                if((x >= (tab[i]->x)-r) && (x <= (tab[i]->x)+r) && (y >= (tab[i]->y)-r) && (y <= (tab[i]->y)+r)) {
                                    if (nb_noeuds_chemin == 0 || tab[i]->voisins[chemin_joueur[nb_noeuds_chemin-1]]){
                                        chemin_joueur[nb_noeuds_chemin] = i; //On l'ajoute au chemin
                                        nb_noeuds_chemin += 1;
                                    }
                                }
                            }
                            break;
                        
                        case SDL_BUTTON_RIGHT: //Si on a un clic droit on enlève le dernier noeud
                            if(etat_graphe == 0) { //Si on est encore en jeu
                                if (nb_noeuds_chemin != 0) { //Si on a déjà sélectionné au moins un noeud
                                    chemin_joueur[nb_noeuds_chemin-1] = -1; //on met le dernier noeud du tableau à -1 valeur impossible
                                    nb_noeuds_chemin -= 1; //on retire 1 au nombre de noeud
                                }
                            }       
                            break;

                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_SPACE: //Si espace on remet le nombre de noeud à 0
                            if(etat_graphe == 0)
                                nb_noeuds_chemin = 0;
                            break;
                            
                        case SDLK_RETURN://Si entrer on verifie que la selection est valide (cycle complet) 
                                         //Si oui on passe dans l'etat de l'écran de fin de jeu (etat_graphe = 1)  
                        
                            for (int i = 0; i < n; i++){
                                all[i] = 0;
                            }

                            for (int i = 0; i < nb_noeuds_chemin; i++){
                                all[chemin_joueur[i]] = 1;
                            }

                            if (tout_noeud(all, n) && chemin_joueur[nb_noeuds_chemin-1] == chemin_joueur[0]){
                                etat_graphe += 1;
                            } 
                            
                            break;

                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        if (update) {
            update = 0;
            clear_SDL(); //Clear la fenêtre (la remetre blanc)

            switch(etat_graphe) { 
                case 0: //etat_graphe = 0, etat jeu en cours
                    affiche(tab, n, 0, 0, 0, 255, 1);
                    sous_graphe = chemin_en_graphe(chemin_joueur, nb_noeuds_chemin, tab, n, &n_s_graphe);
                    affiche(sous_graphe, n_s_graphe, 255, 0, 0, 255, 0);

                    draw_path(tab, chemin_joueur, nb_noeuds_chemin);
                    draw_int(path_size(chemin_joueur, distMat, nb_noeuds_chemin));

                    break;            

                case 1: //etat_graphe = 1, etat écran de fin
                    if (first) {
                        first = 0;
                        pthread_join(thread1, NULL); 
                        pthread_join(thread2, NULL);
                        scoreFloyd = argsFl.result;
                        scoreFourmi = argsF.result;
                    }
                
                    score = path_size(chemin_joueur, distMat, nb_noeuds_chemin); //Score du joueur

                    scoreBest = scoreFourmi;
                    if (scoreFloyd < scoreFourmi) { //recherche quelle est le meilleur score obtenu entre les differents algo et le joueur
                        scoreBest = scoreFloyd;
                    }

                    if (scoreBest > score) {
                        scoreBest = score;
                    }
                    
                    afficheFin(score, scoreBest); //On affiche l'ecran de fin 

                    if (scoreBest == score) {
                        secret1();
                    }

                    break;
                
                default: //etat_graphe = 2
                    program_on = SDL_FALSE; //On ferme cette boucle
                    break;
            }

            render();//rendre les differents elements
        }
    }

    *n_chemin = nb_noeuds_chemin;
    return chemin_joueur;
}

/**
 * @brief Exécute la boucle de jeu dans l'espace
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param chemin Tableau du chemin choisi par le joueur
*/
void boucle_jeu_espace(sommet_t** tab, int n, int * chemin){
    int count = 0;
    float speedX = 0;
    float speedY = 0;
    float x = tab[chemin[0]]->x;
    float y = tab[chemin[0]]->y;
    float directionX = 0;
    float directionY = 0; 
    SDL_bool program_on = SDL_TRUE; //Booléen de boucle de jeu
    SDL_Event event;
    SDL_Rect  navette = {x, y, 20, 20};

    int keyPressZ = 0, keyPressS = 0, keyPressD = 0, keyPressQ = 0;

    //Boucle de jeu
    while (program_on) {
        //Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                
                //pour fermer la fenetre quand on clique sur la croix
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                
                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym){
                        case SDLK_z:
                            keyPressZ = 1;
                            break;

                        case SDLK_s:
                            keyPressS = 1;
                            break;

                        case SDLK_q:
                            keyPressQ = 1;
                            break;

                        case SDLK_d:
                            keyPressD = 1;
                            break;


                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym){
                        case SDLK_z:
                            keyPressZ = 0;
                            break;

                        case SDLK_s:
                            keyPressS = 0;
                            break;

                        case SDLK_q:
                            keyPressQ = 0;
                            break;

                        case SDLK_d:
                            keyPressD = 0;
                            break;


                        default:
                            break;
                    }
                default:
                    break;
            }
            break;
        }

        directionX = 0;
        directionY = 0;
        if (keyPressZ){
            directionY += -0.5;
        }
        if (keyPressS){
            directionY += 0.5;
        }

        if(!keyPressZ && !keyPressS){
            directionY = 0;
        }

        if (keyPressQ){
            directionX += -0.5;
        }
        if (keyPressD){
            directionX += 0.5;
        }

        if(!keyPressD && !keyPressQ){
            directionX = 0;
        }

        if (fabs(directionX) + fabs(directionY) == 0.5){
            directionX *= 2;
            directionY *= 2;
        }
        
        speedX += directionX*ACCELERATION;
        speedY += directionY*ACCELERATION;

        if(directionX == 0 && speedX != 0){
            if (speedX < 0){
                speedX += ACCELERATION/4;
            }else{
                speedX -= ACCELERATION/4;
            }   

            if(speedX < ACCELERATION  && speedX > -ACCELERATION){
                speedX = 0;
            }
        }

        if(directionY == 0 && speedY != 0){
            if (speedY < 0){
                speedY += ACCELERATION/4;
            }else{
                speedY -= ACCELERATION/4;
            }  
            if(speedY < ACCELERATION  && speedY > -ACCELERATION){
                speedY = 0;
            } 
        }
        
        if (speedX < -MAX_SPEED/2){
            speedX = -MAX_SPEED/2;
        }else if(speedX > MAX_SPEED/2){
            speedX = MAX_SPEED/2;
        }

        if (speedY < -MAX_SPEED/2){
            speedY = -MAX_SPEED/2;
        }else if(speedY > MAX_SPEED/2){
            speedY = MAX_SPEED/2;
        }

        x += speedX;
        y += speedY;
        
        if (x < 0){
            x = 0;
            speedX = 0;
            speedY = 0;
        }else if (x > W-navette.w){
            x = W-navette.w;
            speedX = 0;
            speedY = 0;
        }
        if (y < 0){
            y = 0;
            speedX = 0;
            speedY = 0;
        }else if (y > H-navette.h){
            y = H-navette.h;
            speedX = 0;
            speedY = 0;
        }
        
        

        if (count%10 == 0){
            clear_SDL(); //Clear la fenêtre (la remetre blanc)

            navette.x = x;
            navette.y = y;
            draw_rect(navette);       

            render();//rendre les differents elements
        }
        
        

        count++;
    }
}

/**
 * @brief Exécute la boucle de jeu principal
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
*/
void boucle_jeu(sommet_t ** tab, int n){
    init(tab, n); //Affichage du graphe
    
    int n_chemin;
    int * chemin = boucle_jeu_graphe(tab, n, &n_chemin);

    boucle_jeu_espace(tab, n, chemin);

    closeSDL();//free de tout les elements de SDL
}