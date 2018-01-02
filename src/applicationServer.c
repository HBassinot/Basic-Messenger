/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : applicationServer.c                                              */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <ctype.h>

#include "server.h"
#include "listServer.h"
#include "argumentServer.h"
#include "socketServer.h"
#include "commandeServer.h"
#include "displayServer.h"
#include "logServer.h"
#include "applicationServer.h"
#include "listUserServer.h"


static int nbUser;

/* Fonction qui ajoute tous les FD des clients à l'ensemble fd_set */
void addFdUser(ListUser user, fd_set *readfs)
{
    if(user == NULL)
    {
	return;
    }
    FD_SET(user->fd, readfs);

    addFdUser(user->next, readfs);
}


/* Fonction qui retourne le FD du client au numero indice */
int getFdUser(ListUser user, int indice)
{
    int i;

    for(i=0; i<indice; i++)
    {
	if(user == NULL)
	{
	    return 0;
	}
	user = user->next;
    }

    if(user != NULL)
    {
	return user->fd;
    }

    return 0;
}

/* Fonction qui verifie que le nom est correctement ecrit */
int cheickName(char name[], int size)
{
    int i;

    for(i=0; i<size; i++)
    {
	/* Pour le client Telnet */
	if((name[i] == '\r') && (i != 0))
	{
	    name[i] = '\0';
	    return 0;
	}
	if(isalnum(name[i]) == 0)
	{
	    return 1;
	}

    }

    return 0;
}

/* Fonction qui recupere le nom du client */
int setNameUser(ListUser list, CellUser* user)
{
    char name[SIZE];
    int size;

    memset(name, 0, SIZE);
    size = read(user->fd, name, SIZE);

    if(size == 0)
    {
	return 1;
    }
    if((size >= 30) || (name[0] == '\r') || 
       (name[0] == '\n') || (cheickName(name, size) == 1))
    {
	return 2;
    }
    
    /* On verifie que personne ne possede deja ce login */
    while(list != NULL)
    {
	if(strcmp(list->name, name) == 0)
	{
	    return 3;
	}
	list = list->next;
    }

    strcpy(user->name, name);
    return 0;
}


/* Fonction qui retourne le nombre de client connecté au serveur */
int getNbUser()
{
    return nbUser;
}


/* Fonction qui change le nombre de client connecté au serveur */
void setNbUser(int new)
{
    nbUser = new;
}


/* Fonction qui traite les connexions des clients au serveur */
int application(int sock, Argument arg)
{
    ListUser user = NULL;
    CellUser* curentUser;
    fd_set readfs;
    int idUser = 0;
    int test;
    int sock_client;
    int max = sock;
    int i;

    nbUser = 0;
    setArgument(arg);

    while(1)
    {	
	FD_ZERO(&readfs);
	FD_SET(sock, &readfs);
	addFdUser(user, &readfs);

	/* On attend qu'un client vienne parler */
	if(select(max + 1, &readfs, NULL, NULL, NULL) == -1)
	{
	    perror("select() ");
	    return -1;
	}

	/* Si un nouveau client se connecte au serveur */
	if(FD_ISSET(sock, &readfs))
	{
	    /* On accepte la connexion */
	    if((sock_client = accept(sock, NULL, NULL)) == -1)
	    {
		perror("accept() ");
		return -1;
	    }
	  
	    if(sock_client > max)
	    {
		max = sock_client;
	    }

	    idUser++;
	    nbUser++;

	    /* On ajoute le cient à la liste des clients */
	    addUser(&user, idUser, sock_client);

	    /* Affiche message d'accueil au client */
	    printWelcome(sock_client);
	}
	/* Sinon c'est un client deja connecté */
	else
	{
	    for(i=0; i<nbUser; i++)
	    {
		sock_client = getFdUser(user, i);

		if(FD_ISSET(sock_client, &readfs))
		{
		    /* On recupere l'utilisateur courant */
		    curentUser = getUserFd(&user, sock_client);
	
		    /* Si on attend le nom du client */
		    if(curentUser->statut == NEW)
		    {
			test = setNameUser(user, curentUser);
     			if(test == 1)
			{
			    commandeOUT(&user, curentUser);
			}
			else if(test == 2)
			{
			    fprintf(stderr, "Erreur : probleme "
				    "d'identification.\n");
			    printMessage("Nom invalide. Veuillez saisir"
					 " un nouveau nom.", curentUser->fd);
			}
			else if(test == 3)
			{
			    printMessage("Nom invalide. Un utilisateur porte"
					 " deja ce nom.", curentUser->fd);
			}
			else
			{
			    printIdClient(curentUser);
			    curentUser->statut = ONLINE;
			    fprintf(stdout, "%s===Connexion Client=== %d:%s\n",
			       getDate(), curentUser->idUser, curentUser->name);
			}
		    }
		    /* Sinon on attend une commande */
		    else
		    {
			commande(&user, curentUser);
		    }
		}
	    }
	}
    }

    return 1;
}
