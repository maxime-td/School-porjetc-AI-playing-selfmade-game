#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"
#include "solOpt.h"
#include "fourmi.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = gen_tab_sommets(&n, 800, 800);
    //printTabCoord(tab, &n);
    //printf("--------------------------------------------\n");
    tab_to_graph(tab, 0, n-1);
    //printTabCoord(tab, &n);

    make_new_links(10, tab, &n);
    print_tab_coord(tab, &n);
    int ** TableauDistances = dist_tab(tab, &n);
    print_dist_tab(TableauDistances, &n);

    Floyd_Warshall(TableauDistances, n);
    print_dist_tab(TableauDistances, &n);

    printf("Tentative cycle: %d\n", cycle_min_approx(TableauDistances, tab, n));

    
    int y = 0, x;
    int width, height, w_window = 800, h_window = 800, final_width = w_window/2;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;   

    int n_chemin = 0;
    //int * chemin = fourmi(tab, n, &n_chemin);
    //affichTab(chemin, n_chemin);
    
    affiche(tab, n);


    free2DTab((void **)TableauDistances, n);
    free2DTab((void **)tab, n);

    return 0;
}