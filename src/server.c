/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : server.c                                                         */
/******************************************************************************/
/* Description : Fichier principal de l'application cote serveur.             */
/*               Ne contient que la fonction main() qui gere les differents   */
/*               parametres du serveur.                                       */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "server.h"
#include "listServer.h"
#include "argumentServer.h"
#include "socketServer.h"
#include "applicationServer.h"
#include "logServer.h"


/* Fonction principale du programme */
int main(int argc, char **argv)
{
    Argument arg;
    int portnum;
    int fd_socket;
    int pid;

    signal(SIGINT, handler);
    pid = fork();
    if(pid != 0)
    {
	exit(0);
    }
    setsid();

    /* On ouvre les fichiers logs du server */
    if(openLog(YES) == LOG_ERROR)
    {
	return EXIT_FAILURE;
    }

    /* On recupere dans Argument les options transmis au programme */
    if(getArgument(argc, argv, &arg) == ARGUMENT_ERROR)
    {
	fprintf(stderr, "Erreur : argument transmis invalide.\n");
	return EXIT_FAILURE;
    }
   
    portnum = initPort(arg);
    fd_socket = createSocketServer(portnum);
    setSocket(fd_socket);

    /* On test si la socket est correctement ouverte */
    if(fd_socket == SOCKET_ERROR)
    {
	fprintf(stderr, "Erreur : l'ouverture de la socket a echoué.\n");
	return EXIT_FAILURE;
    }

    /* Si une erreur se produit durant la communication du serveur*/
    if(application(fd_socket, arg) == APPLICATION_ERROR)
    {
	fprintf(stderr, "Erreur : un probleme inatendu est survenu.\n");
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
