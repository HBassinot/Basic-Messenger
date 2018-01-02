/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : argumentClient.c                                                 */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "argumentClient.h"
#include "socketClient.h"
#include "server.h"


/* Fonction qui retourne le numero de port a utiliser */
int initPort(Argument arg)
{
    if(arg.portnum != 0)
    {
	return arg.portnum;
    }

    return PORT_NUMBER;
}


/* Fonction qui retourne le numero de port a utiliser */
void initAddr(Argument arg, char dest[])
{
    if(arg.URI[0] != '\0')
    {
	strcpy(dest, arg.URI);
    }
    else
    {
	strcpy(dest, ADDR_SERVER);
    }
}


/* Fonction qui initialise la structure argument */
void initArgument(Argument *arg)
{
    arg->portnum = 0;
    arg->URI[0] = '\0';
}


/* Fonction qui recupere les argument transmis au programme */
int getArguument(int argc, char** argv, Argument* arg)
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
	case ARGUMENT_PORT :
	    arg->portnum = atoi(optarg);
	    break;
	    
	    /* Options invalides */
	case '?' :
	    return ARGUMENT_ERROR;
	}
    }
    
    /* Nom ou adresse du serveur URI */
    if(argc - optind == 1) 
    {
	strcpy(arg->URI, argv[optind]);  
    } 
    else if(argc - optind != 0) 
    {
	return ARGUMENT_ERROR;
    }

    return 0;
}
