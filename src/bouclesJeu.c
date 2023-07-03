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

void *thread_fourmi(FourmiArgs *args)
{
    args->result = multi_start_fourmi(args->matDist, args->n);
    return NULL;
}

void *thread_floyd(FloydWarshallArgs *args)
{
    args->result = multi_Start_Floyd_Warshall(args->tabWarshall, args->tabDist, args->n);
    return NULL;
}

void *timer(timerArgs* timer){
    while((*timer->fin)){
        SDL_Delay(10);
        (timer->time)++;
    }
    return NULL;
}


void * afficheJeu(afficheArgs * argsAff){
    int alpha = 0;
    int etatAlpha = 1;
    int first = 1;

    int seconde = 0;
    
    while (*(argsAff->program_on))
    {   
        if (*(argsAff->fin) && first){
            first = 0;
            seconde = *(argsAff->count)/100;
        }
        
        if (*(argsAff->count)%2 == 0){
            if (*(argsAff->count)%10 == 0){
                argsAff->frameEF = (argsAff->frameEF + 1)%8;
                if (argsAff->frameEF == 0){
                    argsAff->etoileFilante.x = rand()%W;
                    argsAff->etoileFilante.y = rand()%H;
                }
            }
            
            draw_sprite(argsAff->background, argsAff->textureBg, 1, 0, 0, 540);

            SDL_SetTextureAlphaMod(argsAff->textureE2, alpha);
            SDL_SetTextureAlphaMod(argsAff->textureE1, 255-alpha);

            draw_sprite(argsAff->etoile, argsAff->textureE1, 0, 0, 0, 540);
            draw_sprite(argsAff->etoile, argsAff->textureE2, 0, 1, 0, 540);

            draw_sprite(argsAff->etoileFilante, argsAff->textureEF, argsAff->frameEF, 0, 0, 26);

            

            if (!(*(argsAff->fin))){

                for (int i = 0; i < argsAff->n_sous_graphe; i++){
                    argsAff->planete.x = argsAff->sous_graphe[i]->x-24;
                    argsAff->planete.y = argsAff->sous_graphe[i]->y-24;
                    draw_sprite(argsAff->planete, argsAff->textureP, argsAff->co[i].x, argsAff->co[i].y, 0, 48);
                }
                for (int i = 0; i < argsAff->n; i++){
                    if (argsAff->planeteVisite[i]){
                        argsAff->flag.x = argsAff->tab[i]->x-12;
                        argsAff->flag.y = argsAff->tab[i]->y-48;
                        if (i == argsAff->chemin[0]){
                            draw_sprite(argsAff->flag, argsAff->textureF, argsAff->frameFlag, 1, 0, 60);
                        }else{
                            draw_sprite(argsAff->flag, argsAff->textureF, argsAff->frameFlag, 0, 0, 60);
                        }
                        
                    }
                    
                }

                draw_sprite(argsAff->trouNoir, argsAff->textureTN, argsAff->frameTN, 0, 0, argsAff->trouNoir.w);
                draw_sprite(argsAff->navette, argsAff->texture, argsAff->frame, 0, 0, argsAff->navette.w);
                
                if (*(argsAff->count)%20 == 0){
                    argsAff->frame = (argsAff->frame + 1)%4;
                    argsAff->frameTN = (argsAff->frameTN + 1)%4;
                    argsAff->frameFlag = (argsAff->frameFlag + 1)%5;
                }

                affichAst(argsAff->asteroid, argsAff->n_ast, argsAff->textureP);

                argsAff->navette.x = *(argsAff->x);
                argsAff->navette.y = *(argsAff->y);

                draw_time(*(argsAff->count)/100);
                
            }

            else{
                affiche_fin_espace(seconde, 0);
            }

            render(); //rendre les differents elements

            if (etatAlpha){
                alpha--;
                if (0 >= alpha){
                    etatAlpha = !etatAlpha;
                } 
            }else{
                alpha++;
                if (255 <= alpha){
                    etatAlpha = !etatAlpha;
                }
            }
                
        }
    }

    return NULL;
}

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @return tableau du chemin du joueur
 */
int *boucle_jeu_graphe(sommet_t **tab, int n, int *n_chemin, int *fin)
{
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
                        {
                            all[i] = 0;
                        }

                        for (int i = 0; i < nb_noeuds_chemin; i++)
                        {
                            all[chemin_joueur[i]] = 1;
                        }

                        if (tout_noeud(all, n) && chemin_joueur[nb_noeuds_chemin - 1] == chemin_joueur[0])
                        {
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

            if (update)
            {
                update = 0;
                clear_SDL(); // Clear la fenêtre (la remetre blanc)

                switch (etat_graphe)
                {
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
 */
void boucle_jeu_espace(sommet_t **tab, int n, int *chemin, int n_chemin, int* close, int ia, int ** tabIA, int n_ia, int * result, int affiche)
{
    float speedX = 0;
    float speedY = 0;
    float x = tab[chemin[0]]->x;
    float y = tab[chemin[0]]->y;
    float directionX = 0;
    int planeteVisite[n];
    int frame = 0;
    int frameFlag = 0;
    int frameEF = 0;
    int frameTN = 0;
    int n_sous_graphe = 0;
    int n_ast = 0;
    float directionY = 0; 
    int fin = 0;
    int nb_planet = 0;
    int seconde = 0;
    int k;
    int poid;
    int selectPoid;

    float tmpSpeedX = 0;
    float tmpSpeedY = 0;

    timerArgs argsT;
    pthread_t thread, thread2;

    Point p1;
    Point p2;

    Point posNav;
    Point posPlan;

    int closestP;
    int posClosestP;
    int posClosestW;
    int isWall;
    int selectRule;
    int * validRule = (int *) malloc(sizeof(int)*n_ia);

    int keyPressZ = 0;
    int keyPressS = 0;
    int keyPressQ = 0;
    int keyPressD = 0;

    coordonne_t co[n];
    sommet_t **sous_graphe = chemin_en_graphe(chemin, n_chemin, tab, n, &n_sous_graphe);

    asteroid_t * asteroid = ast_Partout(sous_graphe, n_sous_graphe, &n_ast);

    int planeteLigne   = 10;
    int planeteColones[10] = {8, 14, 16, 4, 12, 8, 12, 12, 16, 8}; 

    SDL_bool program_on = SDL_TRUE; // Booléen de boucle de jeu
    SDL_Event event;

    argsT.time = 0;
    argsT.fin = &(program_on);
    pthread_create(&thread, NULL, (void *(*)(void *))timer, &argsT);
    
    SDL_Rect navette = {x, y, 32, 32};
    SDL_Surface * image = IMG_Load("images/soucoupeV3.png");
    SDL_Texture * texture = create_texture(image);
    IMG_Quit();

    SDL_Rect flag = {0, 0, 48, 48};
    SDL_Surface * imageF = IMG_Load("images/flag.png");
    SDL_Texture * textureF = create_texture(imageF);
    IMG_Quit();

    SDL_Rect background = {0, 0, W, H};
    SDL_Surface * imageBg = IMG_Load("images/background.png");
    SDL_Texture * textureBg = create_texture(imageBg);
    IMG_Quit();

    SDL_Rect etoile = {0, 0, W, H};
    SDL_Surface * imageE = IMG_Load("images/etoiles.png");    
    SDL_Texture * textureE1 = create_texture(imageE);
    SDL_Texture * textureE2 = create_texture(imageE);
    SDL_SetTextureAlphaMod(textureE2, 0);
    IMG_Quit();

    SDL_Rect planete = {0, 0, 48, 48};
    SDL_Surface *imageP = IMG_Load("images/planetes.png");
    SDL_Texture *textureP = create_texture(imageP);
    IMG_Quit();

    SDL_Rect etoileFilante = {rand()%W, rand()%H, 26, 26};
    SDL_Surface *imageEF = IMG_Load("images/Comet.png");
    SDL_Texture *textureEF = create_texture(imageEF);
    IMG_Quit();

    SDL_Rect trouNoir = {300, 200, 48, 48};
    SDL_Surface * imageTN = IMG_Load("images/trou_noir.png");
    SDL_Texture * textureTN = create_texture(imageTN);
    IMG_Quit();

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
    affArgs.chemin = chemin;
    affArgs.co = co;
    affArgs.etoileFilante = etoileFilante;
    affArgs.planete = planete;
    affArgs.etoile = etoile;
    affArgs.background = background;
    affArgs.navette = navette;
    affArgs.flag = flag;
    affArgs.trouNoir = trouNoir;
    affArgs.n_ast = n_ast;
    affArgs.x = &x;
    affArgs.y = &y;
    
    pthread_create(&thread2, NULL, (void *(*)(void *))afficheJeu, &affArgs);

    for (int i = 0; i < n_sous_graphe; i++)
    {
        co[i].y = rand() % planeteLigne;
        co[i].x = rand() % planeteColones[co[i].y];
        co[i].y++;
    }

    for (int i = 0; i < n; i++){
        planeteVisite[i] = 0;
    }

    while (program_on && (!ia || argsT.time/100 <= TIME_MAX_IA))
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
                    if(!ia){
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
                    if(!ia){
                        switch (event.key.keysym.sym)
                        {
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
        if(ia){
            if(rand()%1000 == 0){
                posNav.x = x;
                posNav.y = y;

                if (!tout_noeud(planeteVisite, n)){
                    closestP = closest_point(posNav, tab, n, planeteVisite);
                }else{
                    closestP = chemin[0];   
                }

                posPlan.x   = tab[closestP]->x;
                posPlan.y   = tab[closestP]->y;
                posClosestP = position_relative(posNav, posPlan);

                posClosestW = mur_proche(posNav, tab, n, 8, 8);

                isWall = is_mur_in_between(posNav, posPlan, sous_graphe, n, 4);

                selectRule = -1;

                for (int i = 0; i < n_ia; i++){
                    validRule[i] = -1;
                }
                
                k = 0;

                for (int i = 0; i < n_ia; i++){
                    if (tabIA[i][0] == -1 || tabIA[i][0] == posClosestP){
                        if (tabIA[i][1] == -1 || tabIA[i][1] == posClosestW){
                            if (tabIA[i][2] == -1 || tabIA[i][2] == isWall){
                                validRule[k] = i;
                                k++;
                            }
                        }
                    } 
                }

                poid = 0;

                for (int i = 0; i < k; i++){
                    poid += tabIA[validRule[i]][N_RULE+2];
                }

                selectPoid = rand()%poid;
                selectRule = -1;

                poid = 0;
                for (int i = 0; i < k && selectRule == -1; i++){
                    poid += tabIA[validRule[i]][N_RULE+2];
                    if (poid > selectPoid){
                        selectRule = validRule[i];
                    }
                }
                //printf("rule : %d\nInput : %d, %d\n", selectRule, tabIA[selectRule][3], tabIA[selectRule][4]);

                keyPressZ = (tabIA[selectRule][3] == 1);
                keyPressS = (tabIA[selectRule][3] == -1);
                keyPressD = (tabIA[selectRule][4] == -1);
                keyPressQ = (tabIA[selectRule][4] == 1);
            }
        }
        

        directionX = 0;
        directionY = 0;

        if (keyPressZ)
        {
            directionY += -0.5;
        }
        if (keyPressS)
        {
            directionY += 0.5;
        }

        if (!keyPressZ && !keyPressS)
        {
            directionY = 0;
        }

        if (keyPressQ)
        {
            directionX += -0.5;
        }
        if (keyPressD)
        {
            directionX += 0.5;
        }

        if (!keyPressD && !keyPressQ)
        {
            directionX = 0;
        }

        if (fabs(directionX) + fabs(directionY) == 0.5)
        {
            directionX *= 2;
            directionY *= 2;
        }
        
        if (argsT.time%2 == 0){
            speedX += directionX * ACCELERATION;
            speedY += directionY * ACCELERATION;

            if (directionX == 0 && speedX != 0)
            {
                if (speedX < 0)
                {
                    speedX += ACCELERATION * 0.25;
                }
                else
                {
                    speedX -= ACCELERATION * 0.25;
                }

                if (speedX < ACCELERATION && speedX > -ACCELERATION)
                {
                    speedX = 0;
                }
            }

            if (directionY == 0 && speedY != 0)
            {
                if (speedY < 0)
                {
                    speedY += ACCELERATION * 0.25;
                }
                else
                {
                    speedY -= ACCELERATION * 0.25;
                }
                if (speedY < ACCELERATION && speedY > -ACCELERATION)
                {
                    speedY = 0;
                }
            }

            if (speedX < -MAX_SPEED / 2)
            {
                speedX = -MAX_SPEED / 2;
            }
            else if (speedX > MAX_SPEED / 2)
            {
                speedX = MAX_SPEED / 2;
            }

            if (speedY < -MAX_SPEED / 2)
            {
                speedY = -MAX_SPEED / 2;
            }
            else if (speedY > MAX_SPEED / 2)
            {
                speedY = MAX_SPEED / 2;
            }

            x += speedX;
            y += speedY;

            for (int i = 0; i < n; i++){
                p1.x = tab[i]->x;
                p1.y = tab[i]->y;
                p2.x = x+16;
                p2.y = y+16;
                if (distance(p1, p2) < 16+24){
                    planeteVisite[i] = 1;
                    if (tout_noeud(planeteVisite, n) && i == chemin[0]){
                        fin = 1;
                    }
                }
            }
            
            tmpSpeedX = speedX;
            tmpSpeedY = speedY;

            while (!isInPath(x, y, sous_graphe, n, PATH_SIZE-10) && !isInPath(x-32, y-32, sous_graphe, n, PATH_SIZE-10))
            {
                x -= tmpSpeedX*2;
                y -= tmpSpeedY*2;
                speedX = 0;
                speedY = 0;
            }

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

            if (fin && seconde == 0){
                seconde = argsT.time/100;
            }
        }
    }
    
    program_on = SDL_FALSE;
    pthread_join(thread,  NULL);


    seconde = argsT.time/100;   
    for (int i = 0; i < n; i++){
        nb_planet += planeteVisite[i];
    }

    posPlan.x = tab[chemin[0]]->x;
    posPlan.y = tab[chemin[0]]->y;

    *result = calcul_score(seconde, nb_planet, distance(posNav, posPlan));

    free2DTab((void**)sous_graphe, n_sous_graphe);
    
    if (affiche){
        pthread_join(thread2, NULL);
    }
    
    free(validRule);
    free(asteroid);
}

/**
 * @brief Exécute la boucle de jeu principal
 */
void boucle_jeu()
{
    int n = 0;
    sommet_t **tab = NULL;

    init(); // Affichage du graphe

    int n_chemin;
    int fin=0;

    while (!fin){
        tab = gen_tab_sommets(&n);

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
 * @brief Calcul le score de l'ia
 * @param seconde Temps que l'ia à mit pour finir
 * @param nbPlanete Le nombre de planete que l'ia a visité 
 * @param distDep La distance au point de depart
 * @return Le score trouvé
 */
int calcul_score(int seconde, int nbPlanete, int distDep){
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
int closest_point(Point p, sommet_t ** tab, int n, int * planeteVisit){
    int res = 0;
    int bestDist = INT_MAX;
    int dist;
    Point p2;
    for (int i = 0; i < n; i++){
        if(!planeteVisit[i]){
            p2.x = tab[i]->x;
            p2.y = tab[i]->y;
            dist = distance(p, p2);
            if (dist < bestDist){
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
int position_relative(Point p1, Point p2){
    if (p1.x > p2.x){
        if (p1.y > p2.y){
            return 0;
        }else{
            return 1;
        }        
    }else{
        if (p1.y > p2.y){
            return 2;
        }else{
            return 3;
        }
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
int mur_proche(Point p, sommet_t ** tab, int n, int depth, int precision){
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

    for (int i = 0; i < 4; i++){
        for (int j = 1; j < bestDist; j++)
        {
            if (!isInPath(p.x + direction[i].x*j, p.y + direction[i].y*j, tab, n, PATH_SIZE-10)){
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
int is_mur_in_between(Point p1, Point p2, sommet_t ** tab, int n, int precision){
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
        if (!isInPath(p1.x + direction[dir_noeud].x*i, p1.y + direction[dir_noeud].y*i, tab, n, PATH_SIZE-10)){
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Génere une regle au hasard
 * @return La règle générée (avec les 2 dernier paramètre representant l'input)
 */
int * generate_rule(){
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

int ** generate_tab_rules(int n){
    int ** tab_rules = (int **) malloc(sizeof(int*)*n);

    for (int i = 0; i < n-1; i++){
        tab_rules[i] = generate_rule();
    }

    tab_rules[n-1] = (int *) malloc(sizeof(int)*(N_RULE+3));

    tab_rules[n-1][0] = -1;
    tab_rules[n-1][1] = -1;
    tab_rules[n-1][2] = -1;
    tab_rules[n-1][3] = (rand()%3)-1;
    tab_rules[n-1][4] = (rand()%3)-1;
    tab_rules[n-1][5] = 1;
    
    return tab_rules;
}


int ** get_rule_from_file(char * name, int * n){
    int code;
    FILE * file = fopen(name, "r");
    code = fscanf(file, "%d\n", n);
    printf("%d\n", *n);
    int ** tab = (int**) malloc(sizeof(int*)*(*n));
    for (int i = 0; i < *n; i++){
        tab[i] = (int*) malloc(sizeof(int)*(N_RULE+3));
        for (int j = 0; j < (N_RULE+3); j++){
            code = fscanf(file, "%d\n", &tab[i][j]);
        }
        code = fscanf(file, "\n");
    }
    code = fclose(file);
    code += 1;

    return tab;
}


/**
 * @brief Exécute la boucle de jeu principal sans la première partie
 */
void boucle_jeu_sans_graph()
{
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

    while (!fin){
        tab = gen_tab_sommets(&n);

        rules = get_rule_from_file("testRule.txt", &n_rules);     

        tab_to_graph(tab, 0, n - 1);

        make_new_links(10*5/n, tab, &n);

        matDist = dist_tab(tab, &n);
        chemin = colonni_fourmi(matDist, n, rand()%n, &n_chemin);

        boucle_jeu_espace(tab, n, chemin, n_chemin, &fin, 1, rules, n_rules, &res, 1);

        if(chemin != NULL)
            free(chemin);

        free2DTab((void **)matDist, n);
        free2DTab((void **)tab, n);
        free2DTab((void **)rules, n_rules);
    }
    
    closeSDL(); // free de tout les elements de SDL
}


