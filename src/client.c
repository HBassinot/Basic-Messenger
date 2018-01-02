/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : client.c                                                         */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "client.h"
#include "socketClient.h"
#include "argumentClient.h"
#include "applicationClient.h"


/* Fonction principale du programme cote client */
int main(int argc, char **argv)
{
    Argument arg;
    int portnum;
    int fd_socket;
    char addr[SIZE];

    signal(2, handler);

    /* On recupere les arguments transmis au programme */
    if(getArguument(argc, argv, &arg) == ARGUMENT_ERROR)
    {
	fprintf(stderr, "Erreur : argument invalide.\n");
	return 2;
    }

    portnum = initPort(arg);
    initAddr(arg, addr);
    fd_socket = createSocketClient(portnum, addr);

    /* On test si la socket est correctement ouverte */
    if(fd_socket == SOCKET_ERROR)
    {
	fprintf(stderr, "Erreur : l'ouverture de la socket a echoué.\n");
	return EXIT_FAILURE;
    }

    application(fd_socket);
    close(fd_socket);

    return EXIT_SUCCESS;
}

