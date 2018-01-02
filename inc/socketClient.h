#ifndef H_SOCKETCLIENT
#define H_SOCKETCLIENT


/***** CONSTANTES PREPROCESSEUR *****/
#define ADDR_SERVER "127.0.0.1"
#define SOCKET_ERROR -1


/***** PROTOTYPES FONCTIONS *****/
int createSocketClient(int port, char addresse[]);

struct sockaddr_in createStructClient(int port, char addresse[]);


#endif
