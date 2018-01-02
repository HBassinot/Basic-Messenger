#ifndef H_LISTSERVEUR
#define H_LISTSERVEUR


/***** STRUCTURES *****/
typedef enum
{
    OFF,
    ON
} Dynamique;


typedef enum
{
    TWITT,
    FOLLOWER
} Type;


typedef enum
{
    NEW,
    ONLINE,
    OFFLINE
} Statut;

typedef struct _follower
{
    int id;
} Follower;


typedef struct _twitt
{
    int idTwitt;
    char msg[SIZE];
} Twitt;


typedef struct _list
{
    void* data;
    Type type;
    struct _list* next;
} Cell, *List;


typedef struct _user
{
    int fd;
    int idUser;
    char name[SIZE];
    Statut statut;
    Dynamique dynamique;

    List twitt;
    List follower;

    struct _user* next;
} CellUser, *ListUser;


/***** PROTOTYPES FONCTIONS *****/
Follower* createFollower(int id);

Twitt* createTwitt(int id, char* msg);

List createCell(void* element, Type type);

int addHeadList(List* list, void* element, Type type);

int compare(Type type, void* element, int id);

void* findList(List list, int id, Type type);

void showElement(void* element, Type type);

void showList(List list, Type type);

void freeList(List* list);


#endif
