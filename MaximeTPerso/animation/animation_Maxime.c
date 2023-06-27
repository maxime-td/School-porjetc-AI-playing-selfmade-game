#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string.h>

void end_sdl(char ok,                            // fin normale : ok = 0 ; anormale ok = 1
    char const* msg,                             // message à afficher
    SDL_Window* window,                          // fenêtre à fermer
    SDL_Renderer* renderer) {                    // renderer à fermer
        char msg_formated[255];                                            
        int l;
                                       
        if (!ok) {                               // Affichage de ce qui ne va pas
            strncpy(msg_formated, msg, 250);                                         
            l = strlen(msg_formated);                                            
            strcpy(msg_formated + l, " : %s\n");                                     
                                                
            SDL_Log(msg_formated, SDL_GetError());                                   
        }                                                          
                                                
        if (renderer != NULL) {                       // Destruction si nécessaire du renderer
            SDL_DestroyRenderer(renderer);            // Attention : on suppose que les NULL sont maintenus !!
            renderer = NULL;
        }

        if (window != NULL)   {                        // Destruction si nécessaire de la fenêtre
            SDL_DestroyWindow(window);                // Attention : on suppose que les NULL sont maintenus !!
            window= NULL;
        }
                                                
        SDL_Quit();                                                    
                                                
        if (!ok) {                                       // On quitte si cela ne va pas            
            exit(EXIT_FAILURE);                                                  
        }                                                          
}

SDL_Texture* load_texture_from_image(char* file_image_name, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Surface *my_image = NULL;           // Variable de passage
    SDL_Texture* my_texture = NULL;         // La texture

    my_image = IMG_Load(file_image_name);   // Chargement de l'image dans la surface
                                            // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL, 
                                            // uniquement possible si l'image est au format bmp */
    if (my_image == NULL) end_sdl(0, "Chargement de l'image impossible", window, renderer);
   
    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image); // la SDL_Surface ne sert que comme élément transitoire 
    if (my_texture == NULL) end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

    return my_texture;
}
IMG_Quit(); // Si on charge une librairie SDL, il faut penser à la décharger  

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int temps = 0;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
 
    SDL_Rect scene, corps_grenouille, tete_grenouille, oeil_gauche, oeil_droit, bouche, rideau_gauche, rideau_droite;

    /* Initialisation SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

    /* Création de la fenêtre */
    window = SDL_CreateWindow("Théâtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_OPENGL);
    if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

    /* Création du renderer */
    renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

    /* Dessin */
    for(temps = 0; temps < 500; temps+=25) {
        SDL_RenderClear(renderer);

        /* Scène */
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);

        scene.x = 0;
        scene.y = 400;
        scene.w = 1000;
        scene.h = 200;
        SDL_RenderFillRect(renderer, &scene);

        /* Grenouille */
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

        corps_grenouille.x = 450;
        corps_grenouille.y = 200;
        corps_grenouille.w = 100;
        corps_grenouille.h = 250;
        SDL_RenderFillRect(renderer, &corps_grenouille);

        tete_grenouille.x = 400;
        tete_grenouille.y = 200;
        tete_grenouille.w = 200;
        tete_grenouille.h = 100;
        SDL_RenderFillRect(renderer, &tete_grenouille);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        oeil_gauche.x = 420;
        oeil_gauche.y = 220;
        oeil_gauche.w = 20;
        oeil_gauche.h = 20;
        SDL_RenderFillRect(renderer, &oeil_gauche);

        oeil_droit.x = 560;
        oeil_droit.y = 220;
        oeil_droit.w = 20;
        oeil_droit.h = 20;
        SDL_RenderFillRect(renderer, &oeil_droit);

        bouche.x = 430;
        bouche.y = 260;
        bouche.w = 140;
        bouche.h = 10;
        SDL_RenderFillRect(renderer, &bouche);

        /* Rideaux */
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);  

        rideau_gauche.x = 0;
        rideau_gauche.y = 0;
        rideau_gauche.w = 500-temps;
        rideau_gauche.h = 600;
        SDL_RenderFillRect(renderer, &rideau_gauche);
                     
        rideau_droite.x = 500+temps;
        rideau_droite.y = 0;
        rideau_droite.w = 500-temps;
        rideau_droite.h = 600;
        SDL_RenderFillRect(renderer, &rideau_droite);

        /* Affichage */
        SDL_Delay(200);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderPresent(renderer);
    }

    SDL_Delay(1000); // Pause exprimée en ms
                          
    /* Fermeture SDL */
    end_sdl(1, "Normal ending", window, renderer);
    SDL_DestroyTexture(my_texture);
    
    return EXIT_SUCCESS;
}