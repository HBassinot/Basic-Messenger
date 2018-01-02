/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : killserver.c                                                     */
/******************************************************************************/
/* Description : Fichier principal de l'application chargée de tuer le        */
/*               serveur.                                                     */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "server.h"


/* Fonction principale du programme */
int main(void)
{
    char nameServer[] = NAME_SERVER;
    char killer[50];

    sprintf(killer, "pkill -SIGINT %s", nameServer);
    system(killer);

    return 0;
}
