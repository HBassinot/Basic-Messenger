#ifndef H_ARGUMENTSERVER
#define H_ARGUMENTSERVER


/***** CONSTANTES PREPROCESSEUR *****/
#define ACTIVE 1
#define CHAINE_ARGUMENT "p:d"
#define ARGUMENT_ERROR -1
#define ARGUMENT_PORT 'p'
#define ARGUMENT_DYNAMIQUE 'd'


/***** STRUCTURES *****/
typedef struct 
{
    int portnum;
    Dynamique dynamique;
} Argument;


/***** PROTOTYPES FONCTIONS *****/
int initPort(Argument arg);

void initArgument(Argument *arg);

int getArgument(int argc, char** argv, Argument* arg);


#endif
