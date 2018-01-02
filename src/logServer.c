/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : logServer.c                                                      */
/******************************************************************************/
/* Description : Fichier qui contient toutes les fonctions qui gerent les     */
/*               logs du serveur.                                             */
/*                                                                            */
/******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "server.h"
#include "logServer.h"


static int socket;
static int logConnexion;
static int logError;
static char date[SIZE];


void setSocket(int fd)
{
    socket = fd;
}

/* Fonction appelée avant la fin du serveur */
void handler(int i)
{
    if(close(socket) == -1)
    {
	perror("close() ");
    }
    closeLog();

    exit(0);
}


/* Fonction qui ouvre les logs du serveur */
int openLog(int mode)
{
    char name1[SIZE];
    char name2[SIZE];

    if(mode == YES)
    {
	sprintf(name1, "%slog.txt", LOG);
	sprintf(name2, "%slogError.txt", LOG);
	
	logConnexion = open(name1, O_CREAT | O_WRONLY |O_TRUNC, 0644);
	logError = open(name2, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	
	if((logConnexion == -1) || (logError == -1))
	{
	    return LOG_ERROR;
	}
	
	dup2(logError, 2);
	dup2(logConnexion, 1);

	/* On debufferise la sortie */
	setvbuf(stdout, NULL, _IONBF , 0);

       	fprintf(stdout, "%s Fichier log des connexions :\n\n", getDate());
	fprintf(stderr, "%s Fichier log des erreurs :\n\n", getDate());
    }

    return 0;
}


/* Fonction qui ferme les logs du serveur */
void closeLog()
{
    fprintf(stdout, "\nFin du serveur.\n");

    if(close(logConnexion) == -1)
    {
	perror("close() ");
    }
    if(close(logError) == -1)
    {
	perror("close() ");
    }
}


/* Fonction qui retourne l'heure actuelle */
char* getDate()
{
    time_t intps;
    struct tm *dateheure;

    intps = time(NULL);
    dateheure = localtime(&intps);
    
    sprintf(date, "[%d:%d]", dateheure->tm_hour, dateheure->tm_min);
    return date;
}
