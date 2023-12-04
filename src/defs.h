#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100 //!temp test
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10 //!temp test
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

typedef struct Room RoomType;
typedef struct House HouseType;
typedef struct Ghost GhostType;
typedef struct Hunter HunterType;
typedef struct RoomNode RoomNodeType;
typedef struct RoomList RoomListType;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct EvidenceList EvidenceListType;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

enum C_ERROR { C_SUCCESS, C_MEMORY_ERROR, C_ARR_TOO_SMALL, C_DIVIDE_BY_ZERO };

//* Structures
struct Room {
  char              name[MAX_STR];
  HunterType*       hunters[NUM_HUNTERS];
  GhostType*        ghost;
  RoomListType*     roomList; // rooms connected to this one
  EvidenceListType* evidenceList;
};

struct House {
  HunterType*       hunters[NUM_HUNTERS];
  RoomListType*     roomList; // all rooms in the house
  EvidenceListType* evidenceList;
  int huntersWon; // 1 if hunters have won, 0 otherwise
  pthread_mutex_t mutex; // Mutex for synchronizing access to huntersWon
  int               activeHunters;
};

struct Ghost {
  GhostClass  ghostClass;
  RoomType*   currentRoom;
  int         boredom;
  HouseType*  house;
};

struct Hunter {
  char              name[MAX_STR];
  EvidenceType      equipment;
  RoomType*         room;
  EvidenceListType* evidenceList;
  int               fear;
  int               boredom;
  HouseType*        house;
};

struct RoomNode {
  RoomNodeType* next;
  RoomType*     data;
};

struct RoomList {
  RoomNodeType* head;
  RoomNodeType* tail;
  int           size;
};

struct EvidenceNode {
  EvidenceNodeType*  next;
  EvidenceType       data;
};

struct EvidenceList {
  EvidenceNodeType* head;
  EvidenceNodeType* tail;
  int               size;
};


//* Functions: utils.c
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter
void* allocMemory(size_t size);
void initEvidenceList(EvidenceListType** evidenceList);
void addEvidence(EvidenceListType* evidenceList, EvidenceType evType);
int removeEvidence(EvidenceListType* roomEvidenceList, EvidenceListType* hunterEvidenceList, EvidenceType evType);
void removeHunter(HunterType* hunter);


//* Functions: logger.c
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);


//* Functions: house.c
void initHouse(HouseType** house);
void initRoomList(RoomListType** roomList);
void populateRooms(HouseType* house);
RoomType* createRoom(char name[]);
void connectRooms(RoomType* room1, RoomType* room2);
/* 
  Function: Add Room
  Purpose:  Adds a RoomType to a RoomListType by allocating heap space for a RoomNodeType.
  Params:   
    Input: RoomListType* roomList - the list to add the room to
    Input: RoomType* room - the room to be added
*/
void addRoom(RoomListType** roomList, RoomType* room);


//* Functions: hunter.c
void initHunter(char* name, enum EvidenceType equipment, RoomType* room,
                EvidenceListType* evidenceList, HunterType** hunter, HouseType* house);
void* hunter_thread(void* arg);
void collectEvidence(HunterType* hunter);
void moveHunterRooms(HunterType* hunter);
void reviewEvidence(HunterType* hunter);



//* Functions: ghost.c
void initGhost(GhostType** ghost, HouseType* house);
void addGhost(RoomListType* roomList, GhostType** ghost);
void chooseGhostAction(GhostType* ghost, int action);
void moveGhostRooms(GhostType* ghost);
void leaveEvidence(GhostType* ghost);
void* ghost_thread(void* arg);