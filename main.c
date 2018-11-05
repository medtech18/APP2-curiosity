#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "curiosity.h"
#include "listes.h"
#include "interprete.h"

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */


#define LINESIZE 256
#define PROGSIZE 4096

char program[PROGSIZE];
sequence_t prog_seq;
int numero_carte = -1; /* ne lancer que le test de cette carte */


void help(char *progname)
{
    eprintf ("Usage: %s [-d] [-carte <n>] <fichier>\n\n", progname);
    eprintf ("Options:\n");
    eprintf ("\t-d\t\tmode debug\n");
    eprintf ("\t-carte <n>\tUtiliser la carte <n> du fichier de test\n");
    exit(EXIT_FAILURE);
}


/*
 * Lance l'interprétation du programme <prog> sur la carte <carte>.
 */
void launch (bool debug, int carte_num)
{
    if (program[0] != '\0' && carte_num != -1 &&
            (numero_carte == -1 || numero_carte == carte_num)) {

        printf ("Lancement du test...\n");
        printf ("\tProgramme: %s\n", program);
        afficherCarte ();

        /* curiosity_reset (cur); */

        conversion(program,&prog_seq);

        switch (interprete (&prog_seq, debug)) { //interprete le programme lu jusqu'a la fin de son execution 
            afficherCarte();
            case VICTOIRE:
                if (mars.map[cY][cX] != TARGET) {
                    printf ("*********************************************\n");
                    printf ("Curiosity n'est pas sur la cible !\n");
                    printf ("Pourquoi l'interprète crie-t-il victoire ?!?\n");
                    printf ("...\n");
                    printf ("... (tricheurs)...\n");
                } else {
                    if (verifieMarques ()) {
                        printf ("***************************\n");
                        printf ("Carte %d passée avec succès\n", mars.carte_num);
                    } else {
                        printf ("*********************************************\n");
                        printf ("Échec sur la carte %d\n", mars.carte_num);
                        exit (EXIT_FAILURE);
                    }
                }
                break;
            case CIBLERATEE:
                printf ("*********************************************\n");
                printf ("Curiosity n'est pas sur la cible !\n");
            case RATE:
                printf ("*********************************************\n");
                printf ("Échec sur la carte %d\n", mars.carte_num);
                exit (EXIT_FAILURE);
            default:
                printf ("Valeur de retour de l'interpréte inconnue\n");
                exit (EXIT_FAILURE);
        }
    }
}

void read_test_file (char* fichier, bool debug)
{
    FILE *f = fopen (fichier,"r");
    char line[LINESIZE];
    bool in_prog = true;
    int carte_num = -1;

    if (f==NULL) {
        fprintf (stderr, "Impossible d'ouvrir %s en lecture.\n", fichier);
        exit(EXIT_FAILURE);
    }



    program[0] = '\0'; /* nouveau programme */


    while (! feof(f)) {
        if (!fgets (line, LINESIZE, f)) break;

        char *p = line;

        while (*p == ' ') p++;
        if (*p == '#') continue; /* commentaire */
        if (*p == '\n') continue; /* ligne vide */

        if (!strncmp(p, "Pile", 4)) {
            /* Pas de gestion de la directive "Pile" */
            continue;
        }

        if (!strncmp(p, "Programme", 9)) {
            launch (debug, carte_num);

            program[0] = '\0'; /* nouveau programme */
            carte_num = -1;
            in_prog = true;
            continue;
        }

        if (!strncmp(p, "Map", 3)) {
            launch (debug, carte_num);

            carte_num = atoi (p+4);

            initCarte (carte_num);

            printf ("Lecture de la map n°%d\n", carte_num);
            in_prog = false;
            continue;
        }

        if (in_prog) {
            /* ajout ligne au programme */
            assert (strlen(program) + LINESIZE < PROGSIZE);
            strncat (program, line, LINESIZE);
        }
        else {
            /* ajout ligne à la carte */
            ajoutLigneCarte (line);
        }
    }

    launch (debug, carte_num);

}


int main(int argc, char *argv[]) {
    int arg;
    bool debug = false;
    char *fichier = NULL;

    if (argc<2) {
        help (argv[0]);
    }

    arg = 1;
    while (arg < argc) {
        if (! strncmp(argv[arg], "-h", 2)) {
            help (argv[0]);
        }
        if (! strncmp(argv[arg], "-d", 2)) {
            debug = true;
            arg++;
            continue;
        }

        if (! strncmp(argv[arg], "-carte", 6)) {
            numero_carte = atoi(argv[arg+1]);
            arg += 2;
            continue;
        }

        if (fichier) {
            fprintf (stderr, "Un seul fichier de test autorisé!\n");
            exit (EXIT_FAILURE);
        }

        fichier = argv[arg];
        printf ("Fichier de test: %s\n", fichier);

        arg++;
    }


    read_test_file (fichier, debug);

    exit (EXIT_SUCCESS);
}

