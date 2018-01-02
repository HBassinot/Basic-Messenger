/******************************************************************************/
/* BASSINOT Hervé                                          Licence Maths/Info */
/*                                                             L3.2 2009/2010 */
/* Projet : Mini Twitter                                            Systeme 2 */
/******************************************************************************/
/* Fichier : listServer.c                                                     */
/******************************************************************************/
/* Description :                                                              */
/*                                                                            */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "listServer.h"


/* Fonction qui créee une nouvelle structure User */
Twitt* createTwitt(int id, char* msg)
{
    Twitt* twitt = (Twitt*)malloc(sizeof(Twitt));

    twitt->idTwitt = id;
    if(msg != NULL)
    {
	sprintf(twitt->msg, "%s", msg);
    }

    return twitt;
}


/* Fonction qui créee une nouvelle structure User */
Follower* createFollower(int id)
{
    Follower* follower = (Follower*)malloc(sizeof(Follower));

    follower->id = id;

    return follower;
}


/* Fonction qui créee une nouvelle cellule dans la liste */
List createCell(void* element, Type type)
{
    Cell* cell;
    
    cell = (Cell*)malloc(sizeof(Cell));

    if(cell == NULL)
    {
	return NULL;
    }

    cell->next = NULL;
    cell->type = type;
    cell->data = element;

    return cell;
}


/* Fonction qui ajoute un element en tete de liste */
int addHeadList(List* list, void* element, Type type)
{
    List tmp = createCell(element, type);
    
    if(tmp == NULL)
    {
	return 1;
    }

    tmp->next = *list;
    *list = tmp;

    return 0;
}


/* Compare les données de data */
int compare(Type type, void* element, int id)
{
    if(type == TWITT)
    {
	if(((Twitt*)element)->idTwitt == id)
	{
	    return 0;
	}
    }
    if(type == FOLLOWER)
    {
	if(((Follower*)element)->id == id)
	{
	    return 0;
	}
    }

    return 1;
}


/* Fonction qui cherche un element dans une liste et le retourne */
void* findList(List list, int id, Type type)
{
    while(list != NULL)
    {
	if(compare(type, list->data, id) == 0)
	{
	    return list->data;
	}
	list = list->next;
    }
    return NULL;
}


/* Fonction qui affiche le contenu d'une liste */
void showList(List list, Type type)
{
    while(list != NULL)
    {
	showElement(list->data, type);
	list = list->next;
    }
}


/* Fonction qui affiche le contenu d'un element */
void showElement(void* element, Type type)
{
    if(type == TWITT)
    {
	fprintf(stdout, "id Twitt = %d, Message = %s\n",
	       ((Twitt*)element)->idTwitt, ((Twitt*)element)->msg);
    }   
    else if(type == FOLLOWER)
    {
	fprintf(stdout, "id follower = %d\n",
		((Follower*)element)->id);
    }
}


/* Fonction qui libere une liste */
void freeList(List* list)
{
    if(*list == NULL)
    {
	return;
    }

    freeList(&((*list)->next));
    free((*list)->data);

    free(*list);
    *list = NULL;
}
