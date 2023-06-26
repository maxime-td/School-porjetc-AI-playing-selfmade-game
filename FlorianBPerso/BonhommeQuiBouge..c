#include <SDL2/SDL.h>
#include <stdio.h>

/************************************/
/*  exemple de création de fenêtres */
/************************************/

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  SDL_Window
      *Tete = NULL,
      *Cou = NULL,
      *Corps = NULL,
      *Bassin = NULL,
      *JambeG = NULL,
      *JambeD = NULL,
      *Bras = NULL; // Future fenêtre de gauche

  SDL_Renderer
      *RendTete = NULL,
      *RendCou = NULL,
      *RendCorps = NULL,
      *RendBassin = NULL,
      *RendBras = NULL,
      *RendJambeG = NULL,
      *RendJambeD = NULL;

  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError()); // l'initialisation de la SDL a échoué
    exit(EXIT_FAILURE);
  }

  /* Création de la fenêtre de gauche */
  Tete = SDL_CreateWindow(
      "Tete",                // codage en utf8, donc accents possibles
      820, 100,                // coin haut gauche en haut gauche de l'écran
      150, 150,              // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE); // redimensionnable
  if (Tete == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendTete = SDL_CreateRenderer(Tete, -1, 0);

  Cou = SDL_CreateWindow(
      "Cou",    // codage en utf8, donc accents possibles
      850, 250, // coin haut gauche en haut gauche de l'écran
      90, 75, // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);
  if (Cou == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendCou = SDL_CreateRenderer(Cou, -1, 0);

  Corps = SDL_CreateWindow(
      "Corps",  // codage en utf8, donc accents possibles
      750, 340, // coin haut gauche en haut gauche de l'écran
      300, 300, // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);
  if (Corps == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendCorps = SDL_CreateRenderer(Corps, -1, 0);

  Bassin = SDL_CreateWindow(
      "Bassin", // codage en utf8, donc accents possibles
      720, 640, // coin haut gauche en haut gauche de l'écran
      350, 100, // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);
  if (Bassin == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendBassin = SDL_CreateRenderer(Bassin, -1, 0);

  Bras = SDL_CreateWindow(
      "Bras", // codage en utf8, donc accents possibles
      650, 430, // coin haut gauche en haut gauche de l'écran
      500, 50, // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);
  if (Bras == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendBras = SDL_CreateRenderer(Bras, -1, 0);

  JambeG = SDL_CreateWindow(
      "Jambe gauche", // codage en utf8, donc accents possibles
      720, 740, // coin haut gauche en haut gauche de l'écran
      100, 200, // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);
  if (JambeG == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendJambeG = SDL_CreateRenderer(JambeG, -1, 0);


  JambeD = SDL_CreateWindow(
      "Jambe droite", // codage en utf8, donc accents possibles
      970, 740, // coin haut gauche en haut gauche de l'écran
      100, 200, // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);
  if (JambeD == NULL)
  {
    SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
    SDL_Quit();              // On referme la SDL
    exit(EXIT_FAILURE);
  }
  RendJambeD = SDL_CreateRenderer(JambeD, -1, 0);

  SDL_SetRenderDrawColor(RendTete, 255, 165, 0, 255);
  SDL_SetRenderDrawColor(RendCou, 255, 165, 0, 255);
  SDL_SetRenderDrawColor(RendBassin, 255, 165, 0, 255);
  SDL_SetRenderDrawColor(RendCorps, 255, 165, 0, 255);
  SDL_SetRenderDrawColor(RendBras, 255, 165, 0, 255);
  SDL_SetRenderDrawColor(RendJambeG, 255, 165, 0, 255);
  SDL_SetRenderDrawColor(RendJambeD, 255, 165, 0, 255);

  SDL_RenderClear(RendCou);
  SDL_Delay(20);

  SDL_RenderClear(RendCorps);
  SDL_Delay(20);
  SDL_RenderPresent(RendCorps);

  SDL_RenderClear(RendBassin);
  SDL_Delay(20);
  SDL_RenderPresent(RendBassin);

  SDL_RenderClear(RendBras);
  SDL_Delay(20);
  SDL_RenderPresent(RendBras);

    SDL_RenderClear(RendJambeG);
  SDL_Delay(20);
  SDL_RenderPresent(RendJambeG);

  SDL_RenderClear(RendTete);
  SDL_Delay(20);
  SDL_RenderPresent(RendTete);

  SDL_RenderClear(RendJambeD);
  SDL_Delay(20);
  SDL_RenderPresent(RendJambeD);
  SDL_RenderPresent(RendCou);

  SDL_Delay(3000);

  int x, y;
  SDL_GetWindowPosition(Bras, &x, &y);
  while(x+500 < 1820)
  {
    SDL_SetWindowPosition(JambeD, x+10+320, y+310);
    SDL_SetWindowPosition(JambeG, x+10+70, y+310);
    SDL_SetWindowPosition(Cou, x+10+200, y-280);
    SDL_SetWindowPosition(Tete, x+10+170, y-330);
    SDL_SetWindowPosition(Corps, x+10+100, y-90);
    SDL_SetWindowPosition(Bassin, x+10+70, y+210);
    SDL_SetWindowPosition(Bras, x+10, y);
    SDL_GetWindowPosition(Bras, &x, &y);
    y -= 29;
    SDL_RenderPresent(RendCorps);
    SDL_RenderPresent(RendCou);
  }

  /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
  SDL_DestroyWindow(JambeG);  
  SDL_DestroyWindow(JambeD);  
  SDL_DestroyWindow(Bras);
  SDL_DestroyWindow(Bassin);
  SDL_DestroyWindow(Corps);
  SDL_DestroyWindow(Cou);
  SDL_DestroyWindow(Tete);

  SDL_Quit(); // la SDL

  return 0;
}