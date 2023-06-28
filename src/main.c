#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"
#include "OptiFloyd_Warshall.h"
#include "fourmi.h"
#include "interactionJoueur.h"

int main()
{
    srand(time(NULL));
    int n = 0;
    sommet_t **tab = gen_tab_sommets(&n, 800, 800);

    tab_to_graph(tab, 0, n - 1);

    make_new_links(10, tab, &n);
    print_tab_coord(tab, &n);

    int ** TableauDistances = dist_tab(tab, &n);
    print_dist_tab(TableauDistances, &n);

    //free2DTab((void **) tab, n);
    //free2DTab((void **) TableauDistances, n);

    int y = 0, x;
    int width, height, w_window = 800, h_window = 800, final_width = w_window / 2;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;

    int n_chemin = 0;
    int *path = colonni_fourmi(tab, TableauDistances, n, rand() % n, &n_chemin);

    affich_tab(path, n_chemin);
    printf("%d : %d\n",n , path_size(path,TableauDistances , n_chemin));

    Floyd_Warshall(TableauDistances, n);
    
    int * tempAff = multi_Start_Floyd_Warshall(TableauDistances, n, tab);
    printf("cycle OPTI: ");
    affich_tab(tempAff, n);
    printf("\nTaille selon pathsize: %d\n\n", path_size(tempAff, TableauDistances, n)+tempAff[n]);
    
    //affiche(tab, n);

    init(tab, n);

    // boucle_jeu(tab, n);

    free2DTab((void **)TableauDistances, n);
    free2DTab((void **)tab, n);
    free(path);
    return 0;
}