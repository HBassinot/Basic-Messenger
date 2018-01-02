#ifndef H_DISPLAYSERVER
#define H_DISPLAYSERVER


/***** CONSTANTES PREPROCESSEUR *****/
#define HOME "Bienvenue : Veuillez saisir un login."
#define BYE  "Deconnexion : Au revoir."
#define SUCCESS "l'opération s'est effectuée avec succès."
#define ECHEC "l'opération a échoué (raison inattendue)."
#define ERROR "Commande invalide."


/***** PROTOTYPES FONCTIONS *****/
void printWelcome(int sock_client);

void printGoodBye(int sock_client);

void printSuccess(int sock_client);

void printEchec(int sock_client);

void printMessage(char* msg, int sock_client);

void printCommandeError(int sock_client);

void printIdClient(ListUser user);


#endif
