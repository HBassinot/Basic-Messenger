#ifndef H_SOCKETSERVER
#define H_SOCKETSERVER


/***** CONSTANTES PREPROCESSEUR *****/
#define SOCKET_ERROR -1


/***** PROTOTYPES FONCTIONS *****/
int createSocketServer(int port);

struct sockaddr_in createStructServer(int port);


#endif
