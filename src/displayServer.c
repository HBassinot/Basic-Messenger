/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : display.c                                                        */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
#include "listServer.h"
#include "displayServer.h"


/* Fonction qui affiche le message de bienvenue au client */
void printWelcome(int sock_client)
{
    char home[SIZE];
    int size;

    memset(home, 0, SIZE);
    size = sprintf(home, "%s\n", HOME);
    write(sock_client, home, size);
}


/* Fonction qui indique au client qu'il est deconnecté */
void printGoodBye(int sock_client)
{
    char reponse[SIZE];
    int size;

    memset(reponse, 0, SIZE);
    size = sprintf(reponse, "%s\n", BYE);
    write(sock_client, reponse, size);
}


/* Fonction qui affiche succes de la commande */
void printSuccess(int sock_client)
{
    char message[SIZE];
    int size;

    memset(message, 0, SIZE);
    size = sprintf(message, "%s\n", SUCCESS);
    write(sock_client, message, size);
}


/* Fonction qui affiche echec de la commande */
void printEchec(int sock_client)
{
    char message[SIZE];
    int size;

    memset(message, 0, SIZE);
    size = sprintf(message, "%s\n", ECHEC);
    write(sock_client, message, size);
}


/* Fonction qui affiche un message */
void printMessage(char* msg, int sock_client)
{
    char message[SIZE];
    int size;

    memset(message, 0, SIZE);
    size = sprintf(message, "%s\n", msg);
    write(sock_client, message, size);
}

/* Fonction qui affiche que la commande est invalide */
void printCommandeError(int sock_client)
{
    char message[SIZE];
    int size;

    memset(message, 0, SIZE);
    size = sprintf(message, "1\n");
    write(sock_client, message, size);
}


/* Fonction qui affiche les identifiants du client */
void printIdClient(ListUser user)
{
    char message[SIZE];
    int size;
 
    size = sprintf(message, "Bonjour '%s'. Votre identifiant est : %d\n"
		   ,user->name, user->idUser);
    write(user->fd, message, size);
}
