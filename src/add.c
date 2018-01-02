/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : add.c                                                            */
/******************************************************************************/
/* Description : fichier qui contient toutes les fonctions utiles à la        */
/*               commande ADD du serveur.                                     */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <string.h>

#include "server.h"
#include "listServer.h"
#include "argumentServer.h"
#include "commandeServer.h"
#include "displayServer.h"
#include "applicationServer.h"
#include "listUserServer.h"
#include "add.h"


/* Fonction qui recupere le message twitt de la commande ADD */
int getMessageTwitt(char* message, char* msg)
{
    int i = 4;
    int size;

    while(message[i] != '\0')
    {
	msg[i-4] = message[i];
	i++;
    }
    msg[i-4] = '\0';

    size = strlen(msg);
    /* Si le message depasse la taille maximum d'un twitt */
    if((size > SIZE_TWITT) || (size == 0))
    {
	return 1;
    }

    return 0;
}


/* Fonction qui indique au client que son twitt ne convient pas */
void messageError(CellUser* user)
{
    char msg[SIZE];

    sprintf(msg, "Votre twitt est trop long : taille maximum %d caracteres.",
	    SIZE_TWITT);
    printMessage(msg, user->fd);
}


/* Fonction qui execute la commande ADD */
int commandeADD(ListUser list, char* message, CellUser* user)
{
    CellUser* tmpUser;
    char msg[SIZE];
    char msgFol[SIZE];
    Twitt* twitt;
    int fd;
    int i;

    memset(msg, 0, SIZE);

    /* Si le message recuperé est trop long */
    if(getMessageTwitt(message, msg) == 1)
    {
	printMessage("10", user->fd);
	return 10;
    }
  
    /* Ajoute le twitt a la liste des twitts du client */
    twitt = createTwitt(0, msg);
    addHeadList(&(user->twitt), twitt, TWITT);
   
    /* Si le mode dynamique est activé sur le serveur */
    if(getDynamique() == ON)
    {
	sprintf(msgFol, "Message de %s: %s", user->name, msg);
	for(i=0; i<getNbUser(); i++)
	{
	    fd = getFdUser(list, i);
	    if(fd == 0)
	    {
		printMessage("10", user->fd);
		return 10;
	    }
	    tmpUser = getUserFd(&list, fd);
	    
	    if((findList(tmpUser->follower, user->idUser, FOLLOWER) != NULL)
	       && (tmpUser->dynamique == ON))
	    {
		printMessage(msgFol, fd);
	    }
	}
    }

    printMessage("0", user->fd);

    return 0;
}
