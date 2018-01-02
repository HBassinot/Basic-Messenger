/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : applicationClient.c                                              */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dlfcn.h>

#include "server.h"
#include "applicationClient.h"
#include "client.h"
#include "listServer.h"
#include "argumentServer.h"
#include "commandeServer.h"
#include "displayServer.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>


static int socket_server;


/* Fonction qui se lance si le programme recoit un signal */
void handler(int arg)
{
    if(close(socket_server) == -1)
    {
	perror("close() "); 
    }
    exit(0);
}


/* Fonction qui recupere l'identifiant du client dans le message du serveur */
int getId(char *message, int size)
{
    char id[10];
    int i=size-2;
    int j=0;

    while((message[i] <= '9') && (message[i] >= '0'))
    {
	i--;
    }

    for(; i<size; i++)
    {
	id[j] = message[i];
	j++;
    }
    id[j] = '\0';

    return atoi(id);
}


/* Fonction qui vide le buffer d'entrée */
void clean()
{
    int c;

    do
    {
	c = getchar();
    }while(c != '\n' && c != EOF);
}


/* Fonction qui identifie l'utilisateur sur le serveur */
int identification(int fd_socket, User* user)
{
    char msg[SIZE];
    char name[SIZE];
    int size;

    memset(msg, 0, SIZE);
    memset(name, 0, SIZE);

    /* On recupere le message d'accueil du serveur et on l'affiche */
    size = read(fd_socket, msg, SIZE);
    write(1, msg, size);

    do
    {
	/* On tape le nom du client */
	fgets(name, SIZE, stdin);
	size = strlen(name);

	if(size != 1)
	{	    
	    name[size-1] = '\0';
	    write(fd_socket, name, size-1);
	    
	    if(size >= SIZE-10)
	    {
		clean();
	    }
	    memset(msg, 0, SIZE);
	    
	    /* On recupere la reponse du serveur contenant l'id du client */
	    size = read(fd_socket, msg, SIZE);
	    write(1, msg, size);
	}
    }while((msg[0] == 'N') || (size == 1));
    
    strcpy(user->name, name);
    user->idUser = getId(msg, size);

    return 0;
}


/* Fonction qui control si le client veux se deconnecter */
int controlExit(char* commande)
{
    int i=0;
    int size = strlen(commande);
    char cmd[5];

    while((i<3) && (i<size))
    {
	cmd[i] = commande[i];
	i++;
    }
    cmd[i] = '\0';

    if((strcmp(cmd, OUT) == 0) && (size <= 5))
    {
	return 1;
    }

    return 0;
}


/* Fonction qui affiche le message d'accueil du client */
void printHome()
{
    printf(ESC "[0;36;49m");

    fprintf(stdout, 
	    "********************************************************************************\n");
    fprintf(stdout, 
	    "*                            Programme Mini Twitter                            *\n");
    fprintf(stdout, 
	    "********************************************************************************\n");
 
   printf(ESC "[0;39;49m");
}


/* Fonction qui affiche le prompt */
void prompt(User user)
{
    fprintf(stdout, "===");
 
    printf(ESC "[0;32;49m");
    fprintf(stdout, "%d:%s", user.idUser, user.name);
    printf(ESC "[0;39;49m");

    fprintf(stdout, "=== ");
}


/* Fonction qui affiche le message de retour du serveur */
void printRetour(char message[])
{
    int id = message[0];
    int i;

    /* Si le message est un code de retour */
    if((id <= '9') && (id >= '0'))
    {
	switch(id)
	{
	case '0': fprintf(stdout, SUCCESS);	
	    break;

        /* Retour commande ADD ou invalide */
	case '1': i = message[1];
	    if(i == '0')
	    {
		fprintf(stdout, ECHEC);
	    }
	    else if(i == '\n')
	    {
		fprintf(stdout, "commande invalide.");
	    }
	    break;

	/* Retour commande FOL */
	case '2': i = message[1];
	    if(i == '0')
	    {
		fprintf(stdout, ECHEC);	
	    }
	    if(i == '1')
	    {
		fprintf(stdout, "utilisateur id invalide.");	
	    }
	    if(i == '2')
	    {
		fprintf(stdout, "utilisateur id déjà following.");	
	    }
	    break;

        /* Retour commande FQR */
	case '3': i = message[1];
	    if(i == '0')
	    {
		fprintf(stdout, ECHEC);	
	    }
	    if(i == '1')
	    {
		fprintf(stdout, "utilisateur id invalide");	
	    }
	    if(i == '2')
	    {
		fprintf(stdout, "utilisateur id pas following.");	
	    }
	    break;

	/* Retour commande CLE */
	case '4': i = message[1];
	    if(i == '0')
	    {
		fprintf(stdout, ECHEC);	
	    }
	    break;

	/* Retour commande LIS */
	case '5': i = message[1];
	    if(i == '0')
	    {
		fprintf(stdout, "Mode dynamique non activé avec ce serveur.");	
	    }
	    break;

	/* Retour commande STI */
	case '6': i = message[1];
	    if(i == '0')
	    {
		fprintf(stdout, "Mode dynamique non activé avec ce serveur.");	
	    }
	    break;
	}
	fprintf(stdout, "\n");		
    }
}



/* Fonction qui fait tourner l'application client */
int application(int fd_socket)
{
    User user;
    char commande[SIZE];
    char message[SIZE];
    int out = 0;
    int size = 0;
    fd_set readfs;

    printHome();
    socket_server = fd_socket;
    setvbuf(stdout, NULL, _IONBF , 0);

    /* Tant qu'on est pas identifié sur le serveur */
    identification(fd_socket, &user);

    /* Tant que le client reste */
    while(out == 0)
    {
	FD_ZERO(&readfs);
	FD_SET(fd_socket, &readfs);
	FD_SET(1, &readfs);

	/* Si le serveur a encore des choses a dire */
	if(size != SIZE)
	{
	    prompt(user);
	}
	
	memset(commande, 0, SIZE);
	
	/* On attent qu'un des descripteurs recoivent l'info */
	if(select(fd_socket+1, &readfs, NULL, NULL, NULL) == -1)
	{
	    perror("select() ");
	    return -1;
	}

	/* Si cest le serveur qui parle */
	if(FD_ISSET(fd_socket, &readfs))
	{
	    memset(message, 0, SIZE);
	    size = read(fd_socket, message, SIZE);
	    write(1, message, size);

	    if(size == 0)
	    {
		fprintf(stderr, "\nErreur : la connection au serveur a echoué.\n");
		out = 1;
	    }
	}

	/* Si cest le client qui parle */
	else if(FD_ISSET(1, &readfs))
	{
	    /* On envoi la commande au serveur */	  
	    fgets(commande, SIZE, stdin);
	    write(fd_socket, commande, strlen(commande));
	    
	    /* On verifie si le client veut se deconnecter */
	    out = controlExit(commande);
	    
	    /* On recupere la reponse du serveur */
	    memset(message, 0, SIZE);
	    size = read(fd_socket, message, SIZE);
	    write(1, message, size);
	    printRetour(message);
	}
    }

    return 0;
}
