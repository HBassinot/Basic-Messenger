#ifndef H_ARGUMENTCLIENT
#define H_ARGUMENTCLIENT


/***** CONSTANTES PREPROCESSEUR *****/
#define ACTIVE 1
#define CHAINE_ARGUMENT "p:"
#define ARGUMENT_ERROR -1
#define ARGUMENT_PORT 'p'


/***** STRUCTURES *****/
typedef struct 
{
    int portnum;
    char URI[SIZE];
} Argument;


/***** PROTOTYPES FONCTIONS *****/
void initArgument(Argument *arg);

int getArguument(int argc, char** argv, Argument* arg);

int initPort(Argument arg);

void initAddr(Argument arg, char dest[]);


#endif
