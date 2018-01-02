/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : socketClient.c                                                   */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "socketClient.h"


/* Fonction qui initialise la structure sockaddr pour une socket client */
struct sockaddr_in createStructClient(int port, char addresse[])
{
    struct sockaddr_in addr;
    
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addresse);

    return addr;
}


/* Fonction qui créee et parametre une socket de type client */
int createSocketClient(int port, char addresse[])
{
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = createStructClient(port, addresse);  

    if(fd_socket == -1)
    {
	perror("socket() ");
	return SOCKET_ERROR;
    }

    if(connect(fd_socket, (struct sockaddr *) &addr, 
	       sizeof(struct sockaddr_in)) == -1)
    {
	perror("connect() ");
	return SOCKET_ERROR;
    }

    return fd_socket;
}
