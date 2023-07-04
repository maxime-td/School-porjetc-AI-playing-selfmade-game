#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"
#include "OptiFloyd_Warshall.h"
#include "bouclesJeu.h"
#include "map.h"
#include "threads.h"
#include "calculPosition.h"
#include "bot.h"

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param n_chemin permet de retourner la taille du chemin choisi par le joueur
 * @param fin permet de savoir si la fonction à était quitter en appuyant sur la crois
 * @return tableau du chemin du joueur
 */
int *boucle_jeu_graphe(sommet_t **tab, int n, int *n_chemin, int *fin) {
    pthread_t thread1, thread2;
    // Initialisation
    int i;                    // Incrément
    int nb_noeuds_chemin = 0; // Nombre de noeuds dans le chemin
    int r = R_NOEUD;          // Rayon des sommets
    int n_s_graphe;
    int all[n];
    int **warshallDist;
    int first = 1;
    FourmiArgs argsF;
    FloydWarshallArgs argsFl;

    int scoreFourmi, scoreFloyd, scoreBest, score;

    int **distMat = dist_tab(tab, &n);
    warshallDist = copie_tab(distMat, n);
    Floyd_Warshall(warshallDist, n);

    int *chemin_joueur = (int *)malloc(sizeof(int) * MAX_PATH); // Chemin du joueur
    for (i = 0; i < n; i += 1)                                  // Initialisation du chemin du joueur
        chemin_joueur[i] = -1;

    SDL_bool program_on = SDL_TRUE; // Booléen de boucle de jeu
    SDL_Event event;

    int x, y; // Position de la souris au moment du clic

    sommet_t **sous_graphe; // Sous-graphe chemin pour affichage

    argsFl.n = n;
    argsFl.tabDist = distMat;
    argsFl.tabWarshall = warshallDist;
    pthread_create(&thread2, NULL, (void *(*)(void *))thread_floyd, &argsFl);

    argsF.matDist = distMat;
    argsF.n = n;
    pthread_create(&thread1, NULL, (void *(*)(void *))thread_fourmi, &argsF);

    int update = 1;
    int etat_graphe = 0; // 0 = en jeu graphe, 1 = menu de fin, 2 = en jeu espace

    // Boucle de jeu
    if (*fin != 1)
    {
        while (program_on)
        {
            // Gestion des événements
            while (SDL_PollEvent(&event))
            {
                update = 1;
                switch (event.type)
                {

                // pour fermer la fenetre quand on clique sur la croix
                case SDL_QUIT:
                    *fin = 1;
                    program_on = SDL_FALSE;
                    break;

                // Détection des clics
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button)
                    {
                    case SDL_BUTTON_LEFT: // Si on a un clic gauche

                        // Pour voir si on clique sur un noeud
                        for (i = 0; i < n; i += 1)
                        { // On parcour tous les noeuds
                            // On regarde si le clic est dans un carré autour du noeud
                            SDL_GetMouseState(&x, &y);
                            if ((x >= (tab[i]->x) - r) && (x <= (tab[i]->x) + r) && (y >= (tab[i]->y) - r) && (y <= (tab[i]->y) + r))
                            {
                                if (nb_noeuds_chemin == 0 || tab[i]->voisins[chemin_joueur[nb_noeuds_chemin - 1]])
                                {
                                    chemin_joueur[nb_noeuds_chemin] = i; // On l'ajoute au chemin
                                    nb_noeuds_chemin += 1;
                                }
                            }
                        }
                        break;

                    case SDL_BUTTON_RIGHT: // Si on a un clic droit on enlève le dernier noeud
                        if (etat_graphe == 0)
                        { // Si on est encore en jeu
                            if (nb_noeuds_chemin != 0)
                            {                                             // Si on a déjà sélectionné au moins un noeud
                                chemin_joueur[nb_noeuds_chemin - 1] = -1; // on met le dernier noeud du tableau à -1 valeur impossible
                                nb_noeuds_chemin -= 1;                    // on retire 1 au nombre de noeud
                            }
                        }
                        break;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_SPACE: // Si espace on remet le nombre de noeud à 0
                        if (etat_graphe == 0)
                            nb_noeuds_chemin = 0;
                        break;

                    case SDLK_RETURN: // Si entrer on verifie que la selection est valide (cycle complet)
                                      // Si oui on passe dans l'etat de l'écran de fin de jeu (etat_graphe = 1)

                        for (int i = 0; i < n; i++)
                            all[i] = 0;
                    
                        for (int i = 0; i < nb_noeuds_chemin; i++)
                            all[chemin_joueur[i]] = 1;

                        if (tout_noeud(all, n) && chemin_joueur[nb_noeuds_chemin - 1] == chemin_joueur[0])
                            etat_graphe += 1;
                        
                        break;

                    default:
                        break;
                    }
                default:
                    break;
                }
            }

            if (update)
            {
                update = 0;
                clear_SDL(); // Clear la fenêtre (la remetre blanc)

                switch (etat_graphe) {
                    case 0: // etat_graphe = 0, etat jeu en cours
                        affiche(tab, n, 0, 0, 0, 255, 1);
                        sous_graphe = chemin_en_graphe(chemin_joueur, nb_noeuds_chemin, tab, n, &n_s_graphe);
                        affiche(sous_graphe, n_s_graphe, 255, 0, 0, 255, 0);

                        draw_path(tab, chemin_joueur, nb_noeuds_chemin);
                        draw_int(path_size(chemin_joueur, distMat, nb_noeuds_chemin));

                        free2DTab((void **)sous_graphe, n_s_graphe);

                        break;

                    case 1: // etat_graphe = 1, etat écran de fin
                        if (first)
                        {
                            first = 0;
                            pthread_join(thread1, NULL);
                            pthread_join(thread2, NULL);
                            scoreFloyd = argsFl.result;
                            scoreFourmi = argsF.result;
                        }

                        score = path_size(chemin_joueur, distMat, nb_noeuds_chemin); // Score du joueur

                        scoreBest = scoreFourmi;
                        if (scoreFloyd < scoreFourmi)
                        { // recherche quelle est le meilleur score obtenu entre les differents algo et le joueur
                            scoreBest = scoreFloyd;
                        }

                        if (scoreBest > score)
                        {
                            scoreBest = score;
                        }

                        afficheFin(score, scoreBest); // On affiche l'ecran de fin

                        if (scoreBest == score)
                        {
                            secret1();
                        }

                        break;

                    default:                    // etat_graphe = 2
                        program_on = SDL_FALSE; // On ferme cette boucle
                        break;
                }

                render(); // rendre les differents elements
            }
        }
    }

    free2DTab((void **)warshallDist, n);
    free2DTab((void **)distMat, n);
    *n_chemin = nb_noeuds_chemin;

    return chemin_joueur;
}

/**
 * @brief Exécute la boucle de jeu dans l'espace
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param chemin Tableau du chemin choisi par le joueur
 * @param n_chemin Taille du chemin choisi par le joueur
 * @param close Permet de savoir si la boucle à était quitter en appuyant sur la crois
 * @param ia booleen disant si c'est une ia qui joue
 * @param tabIA tableau de regle de l'ia
 * @param n_ia taille du tableau de regle
 * @param result permet de recuperer le score de l'ia
 * @param affiche booleen permetant d'activer ou non l'interface graphique
 */
void boucle_jeu_espace(sommet_t **tab, int n, int *chemin, int n_chemin, int* close, int ia, int ** tabIA, int n_ia, int * result, int affiche) {
    float speedX = 0; //vitesse x du joueur
    float speedY = 0; //vitesse y du joueur
    float speedXTN = 0; //vitesse x trou noir
    float speedYTN = 0; //Vitesse y trou noir
    float speedXTN2 = 0; //vitesse x trou noir
    float speedYTN2 = 0; //Vitesse y trou noir
    float x = tab[chemin[0]]->x - 16; //position x du joueur
    float y = tab[chemin[0]]->y - 16; //position y du joueur
    float xTN = 0, yTN = 0; //Position trou noir
    float xTN2 = 0, yTN2 = 0; //Position trou noir 2
    float directionXTN = 0, directionYTN = 0; //Direction trou noir
    float directionXTN2 = 0, directionYTN2 = 0; //Direction trou noir
    float directionX = 0; //direction x du joueur
    float directionY = 0; //direction y du joueur
    int planeteVisite[n]; //tableau de booleen representant les planetes déjà visité 1 si oui 0 sinon
    int frame = 0; //taille d'une frame de la navette
    int frameFlag = 0; //taille d'une frame du drapeau
    int frameEF = 0; //taille d'une frame d'une etoile filante
    int frameTN = 0; //taille d'une frame du trou noir
    int n_sous_graphe = 0; //nombre de noeud du sous graph généré à partir du chemin
    int n_ast = 0; //nombre d'asteroid
    int fin = 0; //booleen gerant si l'on passe à l'ecran de fin
    int nb_planet = 0; //nombre de planete
    int seconde = 0; //nombre de seconde ecoule quand le jeu s'arrete (pas initialise avant)
    int k; //variable iterative pour le parcour de tabIA
    int poid; //variable servant à stoquer la somme des poids des regles pouvant être prise
    int selectPoid; //variable prise au hasard denant le poid selectionner aleatoirement 
    int distTrouNoir; //la ditance du trou noir au joueur

    timerArgs argsT; // variable servant à stoquer les argument de la fonction timer pour l'appeler en thread
    pthread_t thread, thread2; // initialisation des 2 thread pour le timer et l'affichage

    Point p1; //deux point temporaire servant à stoquer des coordonees pour les compare
    Point p2;

    initPosTN(&xTN, &yTN, x, y);
    initPosTN(&xTN2, &yTN2, x, y);

    // variable pour les regles à choisir
    int closestP; // variable servant à stoquer la sortie de closest_point (l'index de la planete la plus proche)
    int posClosestP; //variable servant à stoquer la sortie de position_relative (la position de la planete la plus proche)
    int posClosestW; // variable servant à stoquer la sortie de mur_proche (la position du mur le plus proche)
    int isWall; // booleen servant à stoquer la sortie de is_wall_in_between (si il y a un mur entre le joueur et la planete la plus proche)
    int selectRule; //variable servant à stoquer l'index de la regle selectionne
    int * validRule = (int *) malloc(sizeof(int)*n_ia); //tableau servant a stoquer les regle valide dans l'etat du jeu actuel

    int keyPressZ = 0; //booleen disant si la touche z est presse
    int keyPressS = 0; //booleen disant si la touche s est presse
    int keyPressQ = 0; //booleen disant si la touche q est presse
    int keyPressD = 0; //booleen disant si la touche d est presse

    Point co[n]; //tableau de coordonne servant à stoquer les coordonné des images des planetes genere
    sommet_t **sous_graphe = chemin_en_graphe(chemin, n_chemin, tab, n, &n_sous_graphe); //sous graphe genere a partir du chemin

    asteroid_t * asteroid = ast_Partout(sous_graphe, n_sous_graphe, &n_ast); //tableau des asteroid

    int planeteLigne   = 10; //nombre de ligne sur l'image des planete
    int planeteColones[10] = {8, 14, 16, 4, 12, 8, 12, 12, 16, 8}; //nombre de planete par ligne

    SDL_bool program_on = SDL_TRUE; // Booléen de boucle de jeu
    SDL_Event event; //variable SDL pour la gestion d'event

    argsT.time = 0; 
    argsT.fin = &(program_on);
    pthread_create(&thread, NULL, (void *(*)(void *))timer, &argsT);
    
    //texture navette
    SDL_Rect navette = {x, y, 32, 32}; 
    SDL_Surface * image = IMG_Load("images/soucoupeV3.png");
    SDL_Texture * texture = create_texture(image);
    IMG_Quit();

    //texture drapeau
    SDL_Rect flag = {0, 0, 48, 48};
    SDL_Surface * imageF = IMG_Load("images/flag.png");
    SDL_Texture * textureF = create_texture(imageF);
    IMG_Quit();

    //texture fond
    SDL_Rect background = {0, 0, W, H};
    SDL_Surface * imageBg = IMG_Load("images/background.png");
    SDL_Texture * textureBg = create_texture(imageBg);
    IMG_Quit();

    //texture etoile
    SDL_Rect etoile = {0, 0, W, H};
    SDL_Surface * imageE = IMG_Load("images/etoiles.png");    
    SDL_Texture * textureE1 = create_texture(imageE);
    SDL_Texture * textureE2 = create_texture(imageE);
    SDL_SetTextureAlphaMod(textureE2, 0);
    IMG_Quit();

    //texture planete
    SDL_Rect planete = {0, 0, 48, 48};
    SDL_Surface *imageP = IMG_Load("images/planetes.png");
    SDL_Texture *textureP = create_texture(imageP);
    IMG_Quit();

    //texture navette
    SDL_Rect etoileFilante = {rand()%W, rand()%H, 26, 26};
    SDL_Surface *imageEF = IMG_Load("images/Comet.png");
    SDL_Texture *textureEF = create_texture(imageEF);
    IMG_Quit();

    //texture trou noir
    int rayonTN = 50; //Le rayon du trou noir
    SDL_Rect trouNoir = {300, 200, rayonTN*2, rayonTN*2};
    SDL_Surface * imageTN = IMG_Load("images/trou_noir_rouge.png");
    SDL_Texture * textureTN = create_texture(imageTN);
    IMG_Quit();
    //texture trou noir 2
    rayonTN = 50; //Le rayon du trou noir
    SDL_Rect trouNoir2 = {300, 200, rayonTN*2, rayonTN*2};
    SDL_Surface * imageTN2 = IMG_Load("images/trou_noir_bleu.png");
    SDL_Texture * textureTN2 = create_texture(imageTN2);
    IMG_Quit();

    //argument pour le thread affiche
    afficheArgs affArgs;
    affArgs.asteroid = asteroid;
    affArgs.count = &argsT.time;
    affArgs.fin   = &fin;
    affArgs.program_on = &program_on;
    affArgs.frame = frame;
    affArgs.frameFlag = frameFlag;
    affArgs.frameEF = frameEF;
    affArgs.frameTN = frameTN;
    affArgs.n = n;
    affArgs.n_sous_graphe = n_sous_graphe;
    affArgs.planeteVisite = planeteVisite;
    affArgs.sous_graphe = sous_graphe;
    affArgs.tab = tab;
    affArgs.texture = texture;
    affArgs.textureBg = textureBg;
    affArgs.textureE1 = textureE1;
    affArgs.textureE2 = textureE2;
    affArgs.textureEF = textureEF;
    affArgs.textureF = textureF;
    affArgs.textureP = textureP;
    affArgs.textureTN = textureTN;
    affArgs.textureTN2 = textureTN2;
    affArgs.chemin = chemin;
    affArgs.co = co;
    affArgs.etoileFilante = etoileFilante;
    affArgs.planete = planete;
    affArgs.etoile = etoile;
    affArgs.background = background;
    affArgs.navette = navette;
    affArgs.flag = flag;
    affArgs.affTrouNoir = trouNoir;
    affArgs.affTrouNoir2 = trouNoir2;
    affArgs.n_ast = n_ast;
    affArgs.x = &x;
    affArgs.y = &y;
    affArgs.type_fin = 0;
    
    if(affiche)
        pthread_create(&thread2, NULL, (void *(*)(void *))afficheJeu, &affArgs);

    //Choix aleatoire des texture des planetes
    for (int i = 0; i < n_sous_graphe; i++) {
        co[i].y = rand() % planeteLigne;
        co[i].x = rand() % planeteColones[co[i].y];
        co[i].y++;
    }

    //Initialisation des planetes
    for (int i = 0; i < n; i++) {
        planeteVisite[i] = 0;
    }

    //Boucle principale
    while (program_on && (!ia || argsT.time/1000 <= TIME_MAX_IA))
    {
        // Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

                // pour fermer la fenetre quand on clique sur la croix
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    fin = 1;
                    (*close) = 1;
                    break;

                case SDL_KEYDOWN:
                    if(!ia) {
                        switch (event.key.keysym.sym) {
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

                            case SDLK_RETURN:
                                if (fin)
                                    program_on = SDL_FALSE;
                                break;

                            default:
                                break;
                        }
                    }
                    break;

                case SDL_KEYUP:
                    if(!ia) {
                        switch (event.key.keysym.sym) {
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
                    }
                default:
                    break;
            }
            break;
        }

        if(ia) {
            if(rand()%500 == 0) { //On ne change pas le mouvement de l'ia à chaque tour de boucle (~1/500)
                //Intialisation des variables pour donner l'etat du jeu
                p1.x = x+navette.w/2;
                p1.y = y+navette.h/2;

                if (!tout_noeud(planeteVisite, n))
                    closestP = closest_point(p1, tab, n, planeteVisite);
                else
                    closestP = chemin[0];

                p2.x = tab[closestP]->x;
                p2.y = tab[closestP]->y;
                posClosestP = position_relative(p1, p2);

                posClosestW = mur_proche(p1, tab, n, 128, 1);

                isWall = is_mur_in_between(p1, p2, sous_graphe, n, 1);

                selectRule = -1;

                for (int i = 0; i < n_ia; i++)
                    validRule[i] = -1;
                
                k = 0;

                //recherche des regles valides
                for (int i = 0; i < n_ia; i++) {
                    if (tabIA[i][0] == -1 || tabIA[i][0] == posClosestP) { 
                        if (tabIA[i][1] == -1 || tabIA[i][1] == posClosestW) {
                            if (tabIA[i][2] == -1 || tabIA[i][2] == isWall) {
                                validRule[k] = i;
                                k++;
                            }
                        }
                    } 
                }

                //choix d'une regles valide au hasard
                poid = 0;

                for (int i = 0; i < k; i++)
                    poid += tabIA[validRule[i]][N_RULE+2];

                selectPoid = rand()%poid;
                selectRule = -1;

                poid = 0;
                for (int i = 0; i < k && selectRule == -1; i++){
                    poid += tabIA[validRule[i]][N_RULE+2];
                    if (poid > selectPoid)
                        selectRule = validRule[i];
                }
                
                //Prise en conte des input en fonction de la regles choisi
                keyPressZ = (tabIA[selectRule][3] == 1);
                keyPressS = (tabIA[selectRule][3] == -1);
                keyPressD = (tabIA[selectRule][4] == -1);
                keyPressQ = (tabIA[selectRule][4] == 1);
            }
        }
        if (!fin && argsT.time%3 == 0){
            
            //Prise en conte de la direction du joueur en fonction des input
            
            directionX = 0;
            directionY = 0;

            calcul_direction_navette(keyPressZ, keyPressS, keyPressQ, keyPressD, &directionX, &directionY);
            //if(rand()%10000 == 0)directionTN(&directionXTN, &directionYTN, xTN, yTN);

            calcul_speed(directionX, directionY, &speedX, &speedY, &x, &y, &navette, ACCELERATION);
            //calcul_speed(directionXTN, directionYTN, &speedXTN, &speedYTN, &xTN, &yTN, &trouNoir, ACCELERATION_TROU);

            for (int i = 0; i < n; i++) {
                p1.x = tab[i]->x;
                p1.y = tab[i]->y;
                p2.x = x+16;
                p2.y = y+16;
                if (distance(p1, p2) < 16+24) {
                    planeteVisite[i] = 1;
                    if (tout_noeud(planeteVisite, n) && i == chemin[0])
                        fin = 1;
                }
            }

            if (!isInPath_Line(x, y, sous_graphe, n, PATH_SIZE-10) && !isInPath_Line(x-32, y-32, sous_graphe, n, PATH_SIZE-10)) {
                x -= speedX;
                y -= speedY;
                speedX = -speedX/2;
                speedY = -speedY/2;
            }

            //Coo joueur pour tests trous noirs
            p1.x = x+16;
            p1.y = y+16;

            //Partie trou noir (bleu)
            p2.x = trouNoir.x+50;
            p2.y = trouNoir.y+50;

            distTrouNoir = distance(p1, p2);

            if(distTrouNoir<rayonTN/3) { //Si trop près meurt
                affArgs.type_fin = 1;
                fin = 1;
            }

            if(distTrouNoir<4*rayonTN) //Si trop près attiré
                attractionTN(&directionX, &directionY, xTN, yTN, x, y, &speedX, &speedY);
            
            if(rand()%10000 == 0)
                directionTN(&directionXTN, &directionYTN, xTN, yTN);
            calcul_speed(directionXTN, directionYTN, &speedXTN, &speedYTN, &xTN, &yTN, &trouNoir, ACCELERATION_TROU);

            trouNoir.x = (int)xTN;
            trouNoir.y = (int)yTN;
            
            affArgs.affTrouNoir = trouNoir;
            //Fin trou noir (bleu)

            //Partie vérif trou noir 2 (rouge)
            p2.x = trouNoir2.x+50;
            p2.y = trouNoir2.y+50;

            distTrouNoir = distance(p1, p2);

            if(distTrouNoir<rayonTN/3) { //Si trop près meurt
                affArgs.type_fin = 1;
                fin = 1;
            }

            if(distTrouNoir<4*rayonTN) //Si trop près attiré
                attractionTN(&directionX, &directionY, xTN2, yTN2, x, y, &speedX, &speedY);

            if(rand()%10000 == 0)
                directionTN(&directionXTN2, &directionYTN2, xTN2, yTN2);

            trouNoir2.x = (int)xTN2;
            trouNoir2.y = (int)yTN2;

            calcul_speed(directionXTN2, directionYTN2, &speedXTN2, &speedYTN2, &xTN2, &yTN2, &trouNoir2, ACCELERATION_TROU);

            affArgs.affTrouNoir2 = trouNoir2;
            //Fin trou noir 2 (rouge)

            if (fin && seconde == 0)
                seconde = argsT.time/1000;
        }

        if (ia && fin){
            program_on = SDL_FALSE;
            if (affArgs.type_fin)
                argsT.time = (1+TIME_MAX_IA)*100;
        }
    }
    
    program_on = SDL_FALSE;
    pthread_join(thread,  NULL);

    if (ia) {
        seconde = argsT.time/1000;   
        for (int i = 0; i < n; i++)
            nb_planet += planeteVisite[i];

        p2.x = tab[chemin[0]]->x;
        p2.y = tab[chemin[0]]->y;

        *result = calcul_score(seconde, nb_planet, distance(p1, p2));
    }

    free2DTab((void**)sous_graphe, n_sous_graphe);
    
    if (affiche)
        pthread_join(thread2, NULL);
    
    free(validRule);
    free(asteroid);
}

/**
 * @brief Exécute la boucle de jeu principal
 */
void boucle_jeu() {
    int n = 0;
    sommet_t **tab = NULL;

    init(); // Affichage du graphe

    int n_chemin;
    int fin=0;

    while (!fin) {
        tab = gen_tab_sommets_rand(&n);

        tab_to_graph(tab, 0, n - 1);

        make_new_links(10*5/n, tab, &n);

       int *chemin = boucle_jeu_graphe(tab, n, &n_chemin, &fin);

        if(!fin)
            boucle_jeu_espace(tab, n, chemin, n_chemin, &fin, 0, NULL, 0, NULL, 1);

        if(chemin != NULL)
           free(chemin);

        free2DTab((void **)tab, n);
    }
    
    closeSDL(); // free de tout les elements de SDL
}

/**
 * @brief Exécute la boucle de jeu principal sans la première partie (la partie graph)
 */
void boucle_jeu_sans_graph() {
    int n = 0;
    sommet_t **tab = NULL;

    init(); // Affichage du graphe

    int n_chemin;
    int fin=0;
    int ** matDist;
    int *chemin;
    int ** rules;
    int n_rules = 10;
    int res;

    while (!fin) {
        tab = gen_tab_sommets_rand(&n);

        rules = get_rule_from_file("testRule.txt", &n_rules);     

        tab_to_graph(tab, 0, n - 1);

        make_new_links(7*5/n, tab, &n);

        matDist = dist_tab(tab, &n);
        chemin = colonni_fourmi(matDist, n, rand()%n, &n_chemin);

        boucle_jeu_espace(tab, n, chemin, n_chemin, &fin, 0, rules, n_rules, &res, 1);

        if(chemin != NULL)
            free(chemin);

        free2DTab((void **)matDist, n);
        free2DTab((void **)tab, n);
        free2DTab((void **)rules, n_rules);
    }
    
    closeSDL(); // free de tout les elements de SDL
}