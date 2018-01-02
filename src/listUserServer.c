/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : listUserServer.c                                                 */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "listServer.h"
#include "listUserServer.h"


/* Fonction qui créee une nouvelle cellule CellUser */
CellUser* createUser(int fd, int id, char* name)
{
    CellUser* user = (CellUser*)malloc(sizeof(CellUser));

    memset(user->name, 0, SIZE);

    user->idUser = id;
    user->fd = fd;
    user->statut = NEW;
    user->dynamique = OFF;

    user->next = NULL;
    user->twitt = NULL;
    user->follower = NULL;
    
    if(name != NULL)
    {
	sprintf(user->name, "%s", name);
    }

    return user;
}


/* Fonction qui ajoute un element en tete de liste utilisateur */
int addHeadListUser(ListUser* list, CellUser* cell)
{
    if(cell == NULL)
    {
	return 1;
    }

    cell->next = *list;
    *list = cell;

    return 0;
}


/* Fonction qui ajoute un utilisateur à la liste des utilisateurs */
void addUser(ListUser* user, int idUser, int sock_client)
{
    CellUser* tmp;
    tmp = createUser(sock_client, idUser, NULL);

    /* On ajoute le client à la liste des utilisateurs */
    addHeadListUser(user, tmp);
}


/* Fonction qui retourne un utilisateur parmis la liste des utilisateurs */
CellUser* getUserFd(ListUser* user, int sock_client)
{
    ListUser tmp=*user;

    while(tmp != NULL)
    {
	if(tmp->fd == sock_client)
	{
	    return tmp;
	}
	tmp = tmp->next;
    }

    return NULL;
}


/* Fonction qui retourne un utilisateur parmis la liste des utilisateurs */
CellUser* getUserId(ListUser* user, int id)
{
    ListUser tmp=*user;

    while(tmp != NULL)
    {
	if(tmp->idUser == id)
	{
	    return tmp;
	}
	tmp = tmp->next;
    }

    return NULL;
}
