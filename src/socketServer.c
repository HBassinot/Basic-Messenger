/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : socketServer.c                                                   */
/******************************************************************************/
/* Description : Fichier qui contient toutes ls fonctions necessairent pour   */
/*               la declaration et la configuration d'une socket pour le      */
/*               serveur.                                                     */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "server.h"
#include "socketServer.h"


/* Fonction qui initialise la structure sockaddr pour une socket serveur */
struct sockaddr_in createStructServer(int port)
{
    struct sockaddr_in so;
    
    so.sin_family = AF_INET;
    so.sin_port = htons(port);
    so.sin_addr.s_addr = INADDR_ANY;

    return so;
}


/* Fonction qui créee et parametre une socket de type serveur */
int createSocketServer(int port)
{
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = createStructServer(port);

    if(fd_socket == -1)
    {
	perror("socket() ");
	return SOCKET_ERROR;
    }

    if(bind(fd_socket, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)) == -1)
    {
	perror("bind() ");
	return SOCKET_ERROR;
    }
  
    if(listen(fd_socket, CLIENT) == -1)
    {
	perror("listen() ");
	return SOCKET_ERROR;
    } 

    return fd_socket;
}
