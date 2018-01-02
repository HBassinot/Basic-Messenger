/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : argumentServer.c                                                 */
/******************************************************************************/
/* Description : fichier qui contient toutes les fonctions utiles à la        */
/*               gestion des arguments du programme.                          */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "server.h"
#include "listServer.h"
#include "argumentServer.h"


/* Fonction qui initialise le numero de port */
int initPort(Argument arg)
{
    if(arg.portnum == 0)
    {
	return PORT_NUMBER;
    }
    
    return arg.portnum;
}


/* Fonction qui initialise la structure argument */
void initArgument(Argument* arg)
{
    arg->portnum = 0;
    arg->dynamique = OFF;
}


/* Fonction qui recupere les arguments transmis au programme */
int getArgument(int argc, char** argv, Argument* arg)
{
    int opt;

    initArgument(arg);
    
    if(argc == 1)
    {
	return 0;
    }
 
    while((opt = getopt_long(argc, argv, CHAINE_ARGUMENT, NULL, 0)) != -1)
    {
	switch(opt)
	{
        /* Argument du numero de port */
	case ARGUMENT_PORT :
	    if(arg->portnum != 0)
	    {
		return ARGUMENT_ERROR;
	    }
	    arg->portnum = atoi(optarg);
	    break;

        /* Argument de l'option dynamique */
	case ARGUMENT_DYNAMIQUE :
	    if(arg->dynamique != 0)
	    {
		return ARGUMENT_ERROR;
	    }
	    arg->dynamique = ON;
	    break;
	    
	/* Options invalides */
	case '?' :
	    return ARGUMENT_ERROR;
	}
    }

    return 0;
}
