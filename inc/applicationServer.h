#ifndef H_APPLICATIONSERVER
#define H_APPLICATIONSERVER


/***** CONSTANTES PREPROCESSEUR *****/
#define APPLICATION_ERROR -1


/***** PROTOTYPES FONCTIONS *****/
int application(int sock, Argument arg);

int getFdUser(ListUser user, int indice);

int getNbUser();

void setNbUser(int new);


#endif
