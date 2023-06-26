#include <SDL2/SDL.h>
#include <math.h>
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
                                                
void draw(SDL_Renderer* renderer) {            // Je pense que vous allez faire moins laid :)
    SDL_Rect rectangle;                                                
                                                
    SDL_SetRenderDrawColor(renderer,                                       
                    250, 0, 0,                             // mode Red, Green, Blue (tous dans 0..255)
                    255);                                 // 0 = transparent ; 255 = opaque
    rectangle.x = 0;                                             // x haut gauche du rectangle
    rectangle.y = 0;                                                  // y haut gauche du rectangle
    rectangle.w = 500;                                                // sa largeur (w = width)
    rectangle.h = 600;                                                // sa hauteur (h = height)
                                                
    SDL_RenderFillRect(renderer, &rectangle);                        
                                                
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                   
    SDL_RenderDrawLine(renderer,                             
        0, 0,                                   // x,y du point de la première extrémité
        400, 400);                              // x,y seconde extrémité
    
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int temps = 0;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Rect rideau_gauche;
    SDL_Rect rideau_droite;

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

        SDL_Delay(200);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderPresent(renderer);
    }

    SDL_RenderPresent(renderer); // affichage
    SDL_Delay(1000); // Pause exprimée en ms
                          
    /* Fermeture SDL */
    end_sdl(1, "Normal ending", window, renderer);

    return EXIT_SUCCESS;
}