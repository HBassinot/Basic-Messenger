#ifndef H_LOGSERVER
#define H_LOGSERVER


/***** CONSTANTES PREPROCESSEUR *****/
#define LOG "log/"
#define LOG_ERROR -1
#define YES 1
#define NO 0


/***** PROTOTYPES FONCTIONS *****/
int openLog();

void closeLog();

void handler(int i);

char* getDate();

void setSocket(int fd);


#endif
