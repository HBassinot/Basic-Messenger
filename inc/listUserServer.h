#ifndef H_LISTUSERSERVER
#define H_LISTUSERSERVER


/***** PROTOTYPES FONCTIONS *****/
CellUser* createUser(int fd, int id, char* name);

int addHeadListUser(ListUser* list, CellUser* cell);

void addUser(ListUser* user, int idUser, int sock_client);

CellUser* getUserFd(ListUser* user, int sock_client);

CellUser* getUserId(ListUser* user, int id);


#endif
