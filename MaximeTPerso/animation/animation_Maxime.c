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

    IMG_Quit(); // Si on charge une librairie SDL, il faut penser à la décharger

    return my_texture;
}

void play_with_texture_1(SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Rect 
        source = {0}, // Rectangle définissant la zone de la texture à récupérer
        window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
        destination = {0}; // Rectangle définissant où la zone_source doit être déposée dans le renderer

    SDL_GetWindowSize(
    window, &window_dimensions.w,
    &window_dimensions.h); // Récupération des dimensions de la fenêtre
    SDL_QueryTexture(my_texture, NULL, NULL,
             &source.w, &source.h); // Récupération des dimensions de l'image

    destination = window_dimensions; // On fixe les dimensions de l'affichage à celles de la fenêtre

    /* On veut afficher la texture de façon à ce que l'image occupe la totalité de la fenêtre */
    SDL_RenderCopy(renderer, my_texture, &source, &destination); // Création de l'élément à afficher
}

void play_with_texture_2(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Rect 
        source = {0}, // Rectangle définissant la zone de la texture à récupérer
        window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
        destination = {0}; // Rectangle définissant où la zone_source doit être déposée dans le renderer

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Récupération des dimensions de la fenêtre
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h); // Récupération des dimensions de l'image

    float zoom = 0.5; // Facteur de zoom à appliquer    
    destination.w = source.w * zoom; // La destination est un zoom de la source
    destination.h = source.h * zoom; // La destination est un zoom de la source
    destination.x = (window_dimensions.w - destination.w) /2; // La destination est au milieu de la largeur de la fenêtre
    destination.y = (window_dimensions.h - destination.h) /2; // La destination est au milieu de la hauteur de la fenêtre

    SDL_RenderCopy(renderer, my_texture, &source, &destination);            
        SDL_RenderPresent(renderer);             
        SDL_Delay(1000);                         

        SDL_RenderClear(renderer); // Effacer la fenêtre
}

void play_with_texture_3(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Rect 
        source = {0}, // Rectangle définissant la zone de la texture à récupérer
        window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
        destination = {0}; // Rectangle définissant où la zone_source doit être déposée dans le renderer

    SDL_Texture* my_texture_fond = NULL;

    my_texture_fond = load_texture_from_image("fond-flammes.jpg", window, renderer);

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Récupération des dimensions de la fenêtre
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h); // Récupération des dimensions de l'image
                                
    /* On décide de déplacer dans la fenêtre cette image */
    float zoom = 0.25; // Facteur de zoom entre l'image source et l'image affichée
                                
    int nb_it = 200; // Nombre d'images de l'animation
    destination.w = source.w * zoom; // On applique le zoom sur la largeur
    destination.h = source.h * zoom; // On applique le zoom sur la hauteur
    destination.x = (window_dimensions.w - destination.w) / 2; // On centre en largeur
    float h = window_dimensions.h - destination.h; // hauteur du déplacement à effectuer

    for (int i = 0; i < nb_it; ++i) {
        destination.y = h * (1 - exp(-5.0 * i / nb_it) / 2 *(1 + cos(10.0 * i / nb_it * 2 * M_PI))); // hauteur en fonction du numéro d'image

        SDL_RenderClear(renderer); // Effacer l'image précédente

        play_with_texture_1(my_texture_fond, window, renderer);

        SDL_SetTextureAlphaMod(my_texture,(1.0-1.0*i/nb_it)*255); // L'opacité va passer de 255 à 0 au fil de l'animation
        SDL_RenderCopy(renderer, my_texture, &source, &destination); // Préparation de l'affichage
        SDL_RenderPresent(renderer); // Affichage de la nouvelle image
        SDL_Delay(30); // Pause en ms
    }                               
    SDL_RenderClear(renderer); // Effacer la fenêtre une fois le travail terminé
    SDL_DestroyTexture(my_texture_fond);
}

void play_with_texture_4(SDL_Texture *bg_texture1, SDL_Texture *bg_texture2, SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Rect
        source = {0},                    // Rectangle définissant la zone totale de la planche
        window_dimensions = {0},         // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
        destination = {0},               // Rectangle définissant où la zone_source doit être déposée dans le renderer
        state = {0};                     // Rectangle de la vignette en cours dans la planche 

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Récupération des dimensions de la fenêtre
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h); // Récupération des dimensions de l'image

    /* Mais pourquoi prendre la totalité de l'image, on peut n'en afficher qu'un morceau, et changer de morceau :-) */

    int nb_images = 2;
    float zoom = 0.2;   
    int offset_x = source.w / nb_images, offset_y = source.h;
    
    state.x = 0;
    state.y = 0;
    state.w = offset_x;                    // Largeur de la vignette
    state.h = offset_y;                    // Hauteur de la vignette

    destination.w = offset_x * zoom;       // Largeur du sprite à l'écran
    destination.h = offset_y * zoom;       // Hauteur du sprite à l'écran

    destination.y = (window_dimensions.h - destination.h - 100) ; // Hauteur de la course

    int speed = 9;
    for (int x = 0; x < window_dimensions.w - destination.w; x += speed) {
        destination.x = x; // Position en x pour l'affichage du sprite
        state.x += offset_x; // On passe à la vignette suivante dans l'image
        state.x %= source.w; // La vignette qui suit celle de fin de ligne est celle de début de ligne

        SDL_RenderClear(renderer); // Effacer l'image précédente avant de dessiner la nouvelle

        play_with_texture_1(bg_texture1, window, renderer);

        SDL_RenderCopy(renderer, my_texture, &state, &destination); 

        play_with_texture_1(bg_texture2, window, renderer);
        SDL_RenderPresent(renderer); // Affichage
        SDL_Delay(80); // Pause en ms
    }
    SDL_RenderClear(renderer);             // Effacer la fenêtre avant de rendre la main
}

void play_with_texture_5(SDL_Texture *bg_texture1, SDL_Texture *bg_texture2, SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Rect
        source = {0}, // Rectangle définissant la zone de la texture à récupérer
        window_dimensions = {0},                  // Rectangle définissant la fenêtre, on  n'utilisera que largeur et hauteur
        destination = {0};                        // Rectangle définissant où la zone_source doit être déposée dans le renderer

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Récupération des dimensions de la fenêtre
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h); // Récupération des dimensions de l'image

    int nb_images = 2; //  Il y a 8 vignette dans la ligne qui nous intéresse
    int nb_images_animation = 1 * nb_images;
    float zoom = 0.2; // zoom, car ces images sont un peu petites
    int offset_x = source.w / 4, // La largeur d'une vignette de l'image
    offset_y = source.h / 5; // La hauteur d'une vignette de l'image
    SDL_Rect state[40]; // Tableau qui stocke les vignettes dans le bon ordre pour l'animation

    /* construction des différents rectangles autour de chacune des vignettes de la planche */
    int i = 0;                                   
    for (int y = 0; y < source.h ; y += offset_y) {
      for (int x = 0; x < source.w; x += offset_x) {
    state[i].x = x;
    state[i].y = y;
    state[i].w = offset_x;
    state[i].h = offset_y;
    ++i;
      }
    }
                                                // ivaut 20 en sortie de boucle
    state[15]  = state[14]                      // on fabrique des images 14 et 15 en reprenant la 13  
              = state[13];                      // donc state[13 à 15] ont la même image, le monstre ne bouge pas   

    for(; i< nb_images ; ++i){                  // reprise du début de l'animation en sens inverse  
      state[i] = state[39-i];                   // 20 == 19 ; 21 == 18 ; ... 39 == 0  
    }

    destination.w = offset_x * zoom;            // Largeur du sprite à l'écran
    destination.h = offset_y * zoom;            // Hauteur du sprite à l'écran
    destination.x = window_dimensions.w * 0.75; // Position en x pour l'affichage du sprite
    destination.y = window_dimensions.h * 0.7;  // Position en y pour l'affichage du sprite

    i = 0;
    for (int cpt = 0; cpt < nb_images_animation ; ++cpt) {
      play_with_texture_1(bg_texture1,         // identique à play_with_texture_1, où on a enlevé l'affichage et la pause
                window, renderer); 
      SDL_RenderCopy(renderer,                  // Préparation de l'affichage
             my_texture, &state[i], &destination);
      i = (i + 1) % nb_images;                  // Passage à l'image suivante, le modulo car l'animation est cyclique 
      SDL_RenderPresent(renderer);              // Affichage
      SDL_Delay(100);                           // Pause en ms
    }
    SDL_RenderClear(renderer);                  // Effacer la fenêtre avant de rendre la main
  }
int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    //int temps = 0;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    //SDL_Texture* my_texture_stand = NULL;
    SDL_Texture* my_texture_marche = NULL;
    SDL_Texture* my_texture_foret = NULL;
    SDL_Texture* my_texture_herbe = NULL;
    
 
    /* Initialisation SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

    /* Création de la fenêtre */
    window = SDL_CreateWindow("Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_OPENGL);
    if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

    /* Création du renderer */
    renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

    /* Dessin 
    for(temps = 0; temps < 500; temps+=25) {
        SDL_RenderClear(renderer);



         
        SDL_Delay(200);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderPresent(renderer);
    } */

    //my_texture_stand = load_texture_from_image("comcomdile.png", window, renderer);
    my_texture_marche = load_texture_from_image("comcomdile_marche.png", window, renderer);
    my_texture_foret = load_texture_from_image("fond-foret.jpg", window, renderer);
    my_texture_herbe = load_texture_from_image("fond-herbe.png", window, renderer);

    //play_with_texture_3(my_texture_stand, window, renderer);
    play_with_texture_4(my_texture_foret, my_texture_herbe, my_texture_marche, window, renderer);

    SDL_Delay(1000); // Pause exprimée en ms
                          
    /* Fermeture SDL */
    end_sdl(1, "Normal ending", window, renderer);
    //SDL_DestroyTexture(my_texture_stand);
    SDL_DestroyTexture(my_texture_marche);

    return EXIT_SUCCESS;
}