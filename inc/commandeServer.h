#ifndef H_COMMANDESERVER
#define H_COMMANDESERVER



/***** CONSTANTES PREPROCESSEUR *****/
#define ADD "ADD"
#define NAM "NAM"
#define GET "GET"
#define FOL "FOL"
#define CLE "CLE"
#define ALL "ALL"
#define FQR "FQR"

#define LIS "LIS"
#define STI "STI"

#define OUT "OUT"
#define HLP "HLP"

#define NUM_ADD 1
#define NUM_NAM 2
#define NUM_GET 3
#define NUM_FOL 4
#define NUM_CLE 5
#define NUM_ALL 6
#define NUM_FQR 7

#define NUM_LIS 8
#define NUM_STI 9

#define NUM_OUT 10
#define NUM_HLP 11

#define COMMANDE_ERROR -1


/***** PROTOTYPES FONCTIONS *****/
int commande(ListUser *user, CellUser* curent);

int getCommande(char msg[], int size);

void commandeError(int sock_client);

void setArgument(Argument arg);

int getDynamique();

int commandeOUT(ListUser* list, CellUser* user);

int removeFollower(List* follower, int id);


#endif
