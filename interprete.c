#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"
#include "ctype.h"

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
#ifdef NCURSES
    getch();  /* attente appui sur touche */
#else
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
#endif
}

cellule_t * scanCommand(cellule_t * tete, int varG);
void execute(cellule_t * tete);
cellule_t* cookBloc(cellule_t * pointerF);


void afficherList(cellule_t * myList)
{
    while(myList)
    {
        printf("%c \n",myList->command);
        myList = myList->suivant;
    }
}

int interprete (sequence_t* seq, bool debug)
{
    // Version temporaire a remplacer par 
    //  - une lecture des commandes dans la liste chainee
    // suivie par
    //  - une interpretation des commandes lues (partie fournie)

    cellule_t * cel = seq->tete;
    cellule_t *pointerV = NULL,*pointerF = NULL;
    cellule_t *finalBlocHead = NULL;


    char commande = cel->command;
    int var = 0;
    int checked = 0;

    debug = false; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher(seq);
    printf ("\n");
    if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    printf("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n");
    commande = seq->tete->command ; //à modifier: premiere commande de la sequence

    while (commande)
    {
        if(commande == '{')
        {
            if(checked ==0)
            {
                pointerV  = cel->suivant;
                checked++;
            }else if(checked ==1)
                {
                    pointerF = cel->suivant;
                }

            while(cel != NULL)
            {
                if( cel->command == '}')
                    break;
                    
                cel = cel->suivant;
            }


        }else if(commande == '?')
        {
            checked = 0;

                if(var == 0)
                    finalBlocHead = cookBloc(pointerF);
                else
                    finalBlocHead = cookBloc(pointerV);

            execute(finalBlocHead);
        }else if ((commande=='a')||(commande=='A')) {
            switch (avance()) {
                case VICTOIRE: return VICTOIRE;
                case RATE: return RATE;
                case REUSSI: break;
            }
        } else if ((commande=='g')||(commande=='G')) {
            gauche();
        } else if ((commande=='d')||(commande=='D')) {
            droite();
        }else if (0 <= (commande -48)  && (commande -48) <= 8)
        {
            var = commande - 48;
        }else if (commande=='m' || commande=='M') {
            var = mesure(var);
        }else if (commande=='p' || commande=='P') {
            pose(var);
        }else
        {
            printf ("Commande inconnue: %c\n", commande);
        }
        // a modifier :
        if(cel !=NULL)
        {
            cel= cel->suivant;
            if(cel ==NULL)
                commande = '\0';
            else
            commande = cel->command;
        }

        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        if (debug) stop();
    }

    /* Si on sort de la boucle sans arriver sur le @
     * c'est raté :-( */

    return CIBLERATEE;
}


cellule_t* cookBloc(cellule_t * pointer)
{
    cellule_t * finalBloc = NULL,*finalBlocHead = NULL;

    if(pointer->command == '}')
    {
        finalBlocHead = nouvelleCellule();
        finalBlocHead->command = '-';
        finalBlocHead->suivant = NULL;
        return finalBlocHead;
    }
                    while(pointer && (pointer->command) != '}')
                    {
                        if(finalBlocHead == NULL)
                        {
                                finalBlocHead = nouvelleCellule();
                                finalBlocHead->command = pointer->command;
                                finalBlocHead->suivant = NULL;

                                finalBloc = finalBlocHead;

                        }else
                            {
                                while(finalBloc->suivant != NULL)
                                {
                                    finalBloc = finalBloc->suivant;
                                }
                                finalBloc->suivant = nouvelleCellule();
                                finalBloc->suivant->command = pointer->command;
                                finalBloc = finalBloc->suivant;
                                finalBloc->suivant = NULL;
                            }
                        pointer = pointer->suivant;
                    }
    return finalBlocHead;
}

void execute(cellule_t * tete)
{
        while(tete)
        {
            switch(tete->command)
            {
                case 'A':
                    avance();
                    break;
                case 'G':
                    gauche();
                    break;
                case 'D':
                    droite();
                    break;
            }
            tete = tete->suivant;
        }
}
