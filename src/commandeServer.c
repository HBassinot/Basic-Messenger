/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : commandeServer.c                                                 */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "server.h"
#include "listServer.h"
#include "argumentServer.h"
#include "logServer.h"
#include "commandeServer.h"
#include "displayServer.h"
#include "applicationServer.h"
#include "listUserServer.h"
#include "add.h"


static Dynamique dynamique;


/* Fonction qui configure le mode dynamique */
void setArgument(Argument arg)
{
    if(arg.dynamique == ON)
    {
	dynamique = ON;
    }
    else
    {
	dynamique = OFF;
    }
}


/* Fonction qui indique au client que la commande est fausse */
void commandeERROR(CellUser* user)
{
    printCommandeError(user->fd);
}


/* Fonction qui traite la commande NAM */
int commandeNAM(ListUser list, CellUser* user)
{
    char reponse[SIZE];
    char login[SIZE];
    int size;

    memset(reponse, 0, SIZE);
    memset(login, 0, SIZE);

    /* Si la liste des utilisateurs est vide */
    if(list == NULL)
    {
	printMessage("La liste des personnes connectées est vide.", user->fd);
	fprintf(stderr, "Erreur : la liste des clients est NULL.\n");
	return -1;
    }

    while(list != NULL)
    {
	if(list->statut == ONLINE)
	{
	    sprintf(login, "%d:%s\n", list->idUser, list->name);
	    strcat(reponse, login);
	}
	list = list->next;
    }

    size = strlen(reponse);
    write(user->fd, reponse, size);

    return 0;
}


/* Fonction qui execute la commande OUT qui permet de quitter le serveur */
int commandeOUT(ListUser* list, CellUser* user)
{
    ListUser tmp = *list;
    ListUser index = *list;
    int nbUser = getNbUser()-1;

    /* On diminue le nombre de client connecté */
    setNbUser(nbUser);

    while(tmp != NULL)
    {
	removeFollower(&(tmp->follower), user->idUser);
	tmp = tmp->next;
    }

    fprintf(stdout, "%s===Deconnexion Client=== %d:%s\n",
	    getDate(), user->idUser, user->name);

    /* Si c'est le premier element de la liste */
    if((index != NULL) && (index->fd == user->fd))
    {
	*list = (*list)->next;
	user->next = NULL;

	printGoodBye(user->fd);
	if(close(user->fd) == -1)
	{
	    perror("close() "); 
	}

	freeList(&(user->twitt));
	freeList(&(user->follower));
	free(user);
	
	return 0;
    }

    while(index != NULL)
    {
	if((index->next != NULL) && (index->next->fd == user->fd))
	{
	    index->next = user->next;
	    user->next = NULL;
	    printGoodBye(user->fd);

	    if(close(user->fd) == -1)
	    {
		perror("close() ");
	    }

	    freeList(&(user->twitt));
	    freeList(&(user->follower));
	    free(user);

	    return 0;
	}
	index = index->next;
    }

    fprintf(stderr, "Erreur : client introuvable dans la liste des clients.\n");

    return 1;
}


/* Affiche tous les twitts d'un utilisateur */
int printTwitt(List twitt, int sock_client)
{
    char *reponse;
    char ms[180];
    int size=0;
    int i=1;
    int len=1000;

    if((reponse = malloc(sizeof(char)*len)) == NULL)
    {
	fprintf(stderr, "Erreur : lors de l'allocation d'une chaine.");
	return -2;
    }
    memset(ms, 0, SIZE);
    memset(reponse, 0, len);

    /* Si la liste des twitts est vide */
    if(twitt == NULL)
    {
	return -1;
    }

    while(twitt != NULL)
    {
	if(size + SIZE > len)
	{
	    len *= 2;
	    if((reponse = realloc(reponse, sizeof(char)*len)) == NULL)
	    {
		fprintf(stderr, "Erreur : lors de la reallocation "
			"d'une chaine.");
		return -2;
	    }
	}

	size += sprintf(ms, "Twitt n°%d:%s", i, ((Twitt*)twitt->data)->msg);
	strcat(reponse, ms);

	twitt = twitt->next;
	i++;
    }

    write(sock_client, reponse, size);
    free(reponse);

    return 0;

}


/* Fonction qui recupere l'id contenus dans la cammande */
int getMessageId(char* message)
{
    char msg[SIZE];
    int i=4;

    while(message[i] != '\0')
    {
	msg[i-4] = message[i];
	i++;
    }
    msg[i-4] = '\0';

    return atoi(msg);
}


/* Fonction qui execute la commande GET */
int commandeGET(ListUser list, char* message, CellUser* user)
{
    int id;
    CellUser* tmp;

    id =  getMessageId(message);
    tmp = getUserId(&list, id);

    if(tmp == NULL)
    {  
	printMessage("l'identifiant n'existe pas !", user->fd);
	return -1;
    }

    if(printTwitt(tmp->twitt, user->fd) == -1)
    {
	printMessage("la liste des twitts est vide.", user->fd);
    }
    
    return 0;
}


/* Fonction qui rajoute un client a la liste des suivi */
int addFollower(List* follow, int id, int sock_client)
{
    Follower* follower;
      
    /* Si le client fait deja parti de la liste des follower */
    if(findList(*follow, id, FOLLOWER) != NULL)
    {
	return -1;
    }

    follower = createFollower(id);
    if(follower == NULL)
    {
	return -1;
    }

    /* On ajoute l'id à la liste des follower */
    addHeadList(follow, follower, FOLLOWER);

    return 0;
}


/* Fonction qui execute la commande FOL */
int commandeFOL(ListUser list, char* message, CellUser* user)
{
    int id;

    id = getMessageId(message);

    if(user == NULL)
    {  
	printMessage("20", user->fd);
	return 20;
    }
   
     /* Si le client qu'on ajoute n'est pas referencé */
    if(getUserId(&list, id) == NULL)
    {
	printMessage("21", user->fd);
	return 21;
    }

    /* On ajoute l'utilisateur à la liste des follower */
    if(addFollower(&(user->follower), id, user->fd) == -1)
    {
	printMessage("22", user->fd);
	return 22;
    }
  
    printMessage("0", user->fd);
    
    return 0;
}


/* Fonction qui supprime un client de la liste de follower */
int removeFollower(List* follower, int id)
{
    List tmp = NULL;
    List index = *follower;

    if((index != NULL) && (((Follower*)index->data)->id == id))
    {
	tmp = *follower;
	*follower = (*follower)->next;
	tmp->next = NULL;

	return 1;
    }

    while(index != NULL)
    {
	if((index->next != NULL) && (((Follower*)index->next->data)->id == id))
	{
	    tmp = index->next;
	    index->next = tmp->next;
	    tmp->next = NULL;
	    free(tmp);

	    return 1;
	}
	index = index->next;
    }

    return 0;
}


/* Fonction qui execute la commande FQR */
int commandeFQR(ListUser list, char* message, CellUser* user)
{
    int id;

    id = getMessageId(message);

     /* Si le client qu'on retire n'est pas referencé dans la liste */
    if(getUserId(&list, id) == NULL)
    {
	printMessage("31", user->fd);
	return 31;
    }

    /* Si le client qu'on retire n'est suivit par le client */
    if(removeFollower(&(user->follower), id) == 0)
    {
	printMessage("32", user->fd);
	return 32;
    }
 
    printMessage("0", user->fd);
   
    return 0;
}


/* Fonction qui execute la commande CLE */
int commandeCLE(CellUser* user)
{
    if(user == NULL)
    {
	printMessage("40",user->fd);
    }

    freeList(&(user->follower));
    user->follower = NULL;

    printMessage("0", user->fd);

    return 0;
}


/* Fonction qui recupere tous les twitts d'un client en fonction de son id */
int getMessagesId(ListUser user, int id, char** reponse, int* len)
{
    char message[SIZE];
    CellUser* tmp;
    List twitt;
    int size = 0;

    tmp = getUserId(&user, id);
    if(tmp == NULL)
    {
	fprintf(stderr, "Erreur : commande ALL id following inconnu.");
	return -1;
    }

    memset(message, 0, SIZE);
    memset(*reponse, 0, *len);

    twitt = tmp->twitt;

    while(twitt != NULL)
    {
	if(size + SIZE > *len)
	{
	    *len *= 2;
	    if((*reponse = realloc(*reponse, sizeof(char)*(*len))) == NULL)
	    {
		fprintf(stderr, "Erreur : lors de l'allocation d'une chaine.");
		return -2;
	    }
	}
	size += sprintf(message, "%d:%s", id, ((Twitt*)twitt->data)->msg);
	strcat(*reponse, message);

	twitt = twitt->next;
    }

    return size;
}


/* Fonction qui execute la commande ALL */
int commandeALL(ListUser list, CellUser* user)
{
    char* reponse;
    char* message;
    List follow;
    int id;
    int size=0;
    int lenReponse=100;
    int lenMessage=100;
    int sizeTmp;

    if((reponse = malloc(sizeof(char)*lenReponse)) == NULL)
    {
	fprintf(stderr, "Erreur : lors de l'allocation d'une chaine.");
	return -2;
    }
    if((message = malloc(sizeof(char)*lenMessage)) == NULL)
    {
	fprintf(stderr, "Erreur : lors de l'allocation d'une chaine.");
	return -2;
    }
    memset(reponse, 0, lenReponse);
    memset(message, 0, lenMessage);

    follow = user->follower;

    if(follow != NULL)
    {
	while(follow != NULL)
	{
	    id = ((Follower*)follow->data)->id;
	    sizeTmp = getMessagesId(list, id, &message, &lenMessage);
	    
	    if(sizeTmp < 0)
	    {
		printMessage("1", user->fd);
		return -1;
	    }
	    
	    size += sizeTmp;

	    if(size >= lenReponse)
	    {
		lenReponse = 2*size;
		if((reponse = realloc(reponse, sizeof(char)*lenReponse)) == NULL)
		{
		    fprintf(stderr, "Erreur : lors de la reallocation d'une chaine.");
		    return -2;
		}
	    }
	    strcat(reponse, message);

	    follow = follow->next;
	}
	
	if(size == 0)
	{
	    printMessage("Il n'y a aucun message.", user->fd);
	}
	else
	{
	    write(user->fd, reponse, size);
	}
    }
    else
    {
	printMessage("Votre liste de following est vide.", user->fd);
    }
    
    free(message);
    free(reponse);

    return 0;
}


/* Fonction qui execute la commande LIS */
int commandeLIS(CellUser* user)
{
    if(dynamique == OFF)
    {
	printMessage("50", user->fd);
    }
    else
    {
	user->dynamique = ON;
/*	printMessage("Mode dynamique activé.", user->fd);*/
	printMessage("0", user->fd);
    }

    return 0;
}


/* Fonction qui execute la commande STI */
int commandeSTI(CellUser* user)
{
    if(dynamique == OFF)
    {
	printMessage("60", user->fd);
    }
    else
    {
	user->dynamique = OFF;
/*	printMessage("Mode dynamique desactivé.", user->fd);*/
	printMessage("0", user->fd);
    }

    return 0;
}


/* Fonction qui execute la commande HLP */
void commandeHLP(CellUser* user)
{
    char help[1500];
    int size;

    memset(help, 0, 1500);
    strcat(help,
	   "************** Aide sur les commandes disponibles sur le serveur ***************\n"
	   "Commandes supplémentaires du serveur.\n"
	   "\tSe deconnecter du serveur  : OUT\n"
	   "\tAfficher l'aide de serveur : HLP\n\n");

    strcat(help,
	    "Commandes d'affichage.\n"
	   "\tAfficher la liste des personnes connectées                     : NAM\n"
	   "\tAfficher les twitts des utilisateurs de la liste des Following : ALL\n"
	   "\tAfficher l'ensemble des twitts d'un utilisateur                : GET id\n"
	   "\t\toù id est l'identifiant de l'utilisateur.\n\n");

    strcat(help,
	   "Commandes d'ajout.\n"
	   "\tAjouter un twitt                                     : ADD twitt\n"
	   "\t\toù twitt est le message que l'on ajoute.\n"
	   "\tAjouter un utilisateur à votre liste de Following    : FOL id\n"
	   "\t\toù id est l'identifiant de l'utilisateur.\n"
	   "\tSupprimer un utilisateur de votre liste de Following : FQR id\n"
            "\t\toù id est l'identifiant de l'utilisateur.\n"
	   "\tSupprimer toute la liste des Following               : CLE\n\n");

    strcat(help,
	   "Commandes bonus.\n"
	   "\tActiver le mode dynamique    : LIS\n"
	   "\tDesactiver le mode dynamique : STI\n"
	   "******************************** Fin de l'aide. ********************************\n\n");

    size = strlen(help);
      
    write(user->fd, help, size);
}


/* Fonction qui renvoie la valeur de la globale dynamique */
int getDynamique()
{
    return dynamique;
}


/* Fonction qui execute les commandes entrées par le client */
int commande(ListUser* user, CellUser* curent)
{
    int commande;
    char message[SIZE];
    int size;

    memset(message, 0, SIZE);
    size = read(curent->fd, message, SIZE);

    /* Si on a perdu la conenction avec le client */
    if(size == 0)
    {
	commandeOUT(user, curent);
	return 1;	
    }

    commande = getCommande(message, size);

    switch(commande)
    {
    case NUM_ADD : /* Commande ADD : ajoute un twitt au client */
	commandeADD(*user, message, curent);
	break;

    case NUM_NAM : /* Commande NAM : liste les logins et id des clients */
	commandeNAM(*user, curent);
	break;

    case NUM_GET : /* Commande GET : recupere tous les twitts d'un id */
	commandeGET(*user, message, curent);
	break;

    case NUM_FOL : /* Commande FOL : suit un autre client du serveur */
	commandeFOL(*user, message, curent);
	break;

    case NUM_ALL : /* Commande ALL : affiche les twitts des clients suivis */
	commandeALL(*user, curent);
	break;

    case NUM_CLE : /* Commande CLE : vide la liste des followings */
	commandeCLE(curent);
	break;

    case NUM_FQR : /* Commande FQR : supprime un client des followings */
	commandeFQR(*user, message, curent);
	break;


    case NUM_STI : /* Commande STI : ferme le mode dynamique du serveur */ 
	commandeSTI(curent);
	break;

    case NUM_LIS : /* Commande LIS : active le mode dynamique du serveur */ 
	commandeLIS(curent);
	break;


    case NUM_OUT : /* Commande OUT : permet de quitter le serveur */
	commandeOUT(user, curent);
	return 1;

    case NUM_HLP : /* Commande HLP : permet d'afficher l'aide du serveur */
	commandeHLP(curent);
	return 1;

    default :
	commandeERROR(curent);

    }

    return 0;
}


/* Fonction qui verifie la commande tapé par le client */
int cheickCommande(char commande[], int size)
{
    int i;
   
    for(i=0; i<size-1; i++)
    {
	if(commande[i] == '\r')
	{
	    return 0;
	}
	if(isprint(commande[i]) == 0)
	{
	    return 1;
	}
    }
    
    return 0;
}


/* Fonction qui retourne la commande taper par l'utilisateur */
int getCommande(char msg[], int size)
{
    int i;
    char commande[5];

    if(size < 3)
    {
	return COMMANDE_ERROR;
    }

    if(cheickCommande(msg, size) == 1)
    {
	return COMMANDE_ERROR;
    }

    /* On recupere les 3 premiers caracteres */
    for(i=0; i<3; i++)
    {
	commande[i] = msg[i];
    }
    commande[i] = '\0';

    if(strcmp(commande, ADD) == 0)
    {
	return NUM_ADD;
    }
    if((strcmp(commande, NAM) == 0) && (size <= 5))
    {
	return NUM_NAM;
    }
    if(strcmp(commande, GET) == 0)
    {
	return NUM_GET;
    }
    if(strcmp(commande, FOL) == 0)
    {
	return NUM_FOL;
    }
    if((strcmp(commande, CLE) == 0) && (size <= 5))
    {
	return NUM_CLE;
    }
    if(strcmp(commande, FQR) == 0)
    {
	return NUM_FQR;
    }
    if((strcmp(commande, ALL) == 0) && (size <= 5))
    {
	return NUM_ALL;
    }


    if((strcmp(commande, LIS) == 0) && (size <= 5))
    {
	return NUM_LIS;
    }
    if((strcmp(commande, STI) == 0) && (size <= 5))
    {
	return NUM_STI;
    }


    if((strcmp(commande, OUT) == 0) && (size <= 5))
    {	
	return NUM_OUT;
    }
    if((strcmp(commande, HLP) == 0) && (size <= 5))
    {	
	return NUM_HLP;
    }

    return COMMANDE_ERROR;
}
