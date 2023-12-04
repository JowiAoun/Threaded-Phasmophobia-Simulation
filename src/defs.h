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
  int               huntersWon; // 1 if hunters have won, 0 otherwise
  pthread_mutex_t   mutex; // Mutex for synchronizing access to huntersWon
  GhostClass        ghostClass;
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
/*
  Function: randInt
  Purpose: Generates a random Int within a specified range.
  Params:
    Input: int min - a numeric value representing min.
    Input: int max - a numeric value representing max.
*/
int randInt(int,int);        // Pseudo-random number generator function
/*
  Function: randFloat
  Purpose: Generates a random Float within a specified range.
  Params:
    Input: float min - a numeric value representing min.
    Input: float max - a numeric value representing max.
*/
float randFloat(float, float);  // Pseudo-random float generator function
/*
  Function: randomGhost
  Purpose: Generates a random omGhost within a specified range.
  Params:
*/
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
/*
  Function: ghostToString
  Purpose: Performs the specified operation for ghostToString.
  Params:
    Input: enum GhostClass ghost - an enumerated type representing ghost.
    Input: char* buffer - a pointer/reference to char.
*/
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
/*
  Function: evidenceToString
  Purpose: Performs the specified operation for evidenceToString.
  Params:
    Input: enum EvidenceType type - a pointer/reference to enum Evidence.
    Input: char* str - a pointer/reference to char.
*/
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter
/*
  Function: allocMemory
  Purpose: Performs the specified operation for allocMemory.
  Params:
    Input: size_t size - [describe the parameter].
*/
void* allocMemory(size_t size);
/*
  Function: initEvidenceList
  Purpose: Initializes and allocates memory for a new EvidenceList.
  Params:
    Input: EvidenceListType** evidenceList - a pointer/reference to EvidenceList.
*/
void initEvidenceList(EvidenceListType** evidenceList);
/*
  Function: addEvidence
  Purpose: Adds or connects elements in the Evidence structure.
  Params:
    Input: EvidenceListType* evidenceList - a pointer/reference to EvidenceList.
    Input: EvidenceType evType - a pointer/reference to Evidence.
*/
void addEvidence(EvidenceListType* evidenceList, EvidenceType evType);
/*
  Function: removeEvidence
  Purpose: Performs the specified operation for removeEvidence.
  Params:
    Input: EvidenceListType* roomEvidenceList - a pointer/reference to EvidenceList.
    Input: EvidenceListType* hunterEvidenceList - a pointer/reference to EvidenceList.
    Input: EvidenceType evType - a pointer/reference to Evidence.
*/
int removeEvidence(EvidenceListType* roomEvidenceList, EvidenceListType* hunterEvidenceList, EvidenceType evType);
/*
  Function: removeHunter
  Purpose: Performs the specified operation for removeHunter.
  Params:
    Input: HunterType* hunter - a pointer/reference to Hunter.
*/
void removeHunter(HunterType* hunter);


//* Functions: logger.c
/*
  Function: l_hunterInit
  Purpose: Logs information related to hunterInit.
  Params:
    Input: char *hunter - a character array or string.
    Input: enum EvidenceType equipment - a pointer/reference to enum Evidence.
*/
void l_hunterInit(char* name, enum EvidenceType equipment);
/*
  Function: l_hunterMove
  Purpose: Logs information related to hunterMove.
  Params:
    Input: char *hunter - a character array or string.
    Input: char *room - a character array or string.
*/
void l_hunterMove(char* name, char* room);
/*
  Function: l_hunterReview
  Purpose: Logs information related to hunterReview.
  Params:
    Input: char *hunter - a character array or string.
    Input: enum LoggerDetails result - an enumerated type representing result.
*/
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
/*
  Function: l_hunterCollect
  Purpose: Logs information related to hunterCollect.
  Params:
    Input: char *hunter - a character array or string.
    Input: enum EvidenceType evidence - a pointer/reference to enum Evidence.
    Input: char *room - a character array or string.
*/
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
/*
  Function: l_hunterExit
  Purpose: Logs information related to hunterExit.
  Params:
    Input: char *hunter - a character array or string.
    Input: enum LoggerDetails reason - an enumerated type representing reason.
*/
void l_hunterExit(char* name, enum LoggerDetails reason);
/*
  Function: l_ghostInit
  Purpose: Logs information related to ghostInit.
  Params:
    Input: enum GhostClass ghost - an enumerated type representing ghost.
    Input: char *room - a character array or string.
*/
void l_ghostInit(enum GhostClass type, char* room);
/*
  Function: l_ghostMove
  Purpose: Logs information related to ghostMove.
  Params:
    Input: char *room - a character array or string.
*/
void l_ghostMove(char* room);
/*
  Function: l_ghostEvidence
  Purpose: Logs information related to ghostEvidence.
  Params:
    Input: enum EvidenceType evidence - a pointer/reference to enum Evidence.
    Input: char *room - a character array or string.
*/
void l_ghostEvidence(enum EvidenceType evidence, char* room);
/*
  Function: l_ghostExit
  Purpose: Logs information related to ghostExit.
  Params:
    Input: enum LoggerDetails reason - an enumerated type representing reason.
*/
void l_ghostExit(enum LoggerDetails reason);


//* Functions: house.c
/*
  Function: initHouse
  Purpose: Initializes and allocates memory for a new House.
  Params:
    Input: HouseType** house - a pointer/reference to House.
*/
void initHouse(HouseType** house);
/*
  Function: initRoomList
  Purpose: Initializes and allocates memory for a new RoomList.
  Params:
    Input: RoomListType** roomList - a pointer/reference to RoomList.
*/
void initRoomList(RoomListType** roomList);
/*
  Function: populateRooms
  Purpose: Performs the specified operation for populateRooms.
  Params:
    Input: HouseType* house - a pointer/reference to House.
*/
void populateRooms(HouseType* house);
/*
  Function: createRoom
  Purpose: Initializes and allocates memory for a new teRoom.
  Params:
    Input: char name[] - a character array or string.
*/
RoomType* createRoom(char name[]);
/*
  Function: connectRooms
  Purpose: Adds or connects elements in the nectRooms structure.
  Params:
    Input: RoomType* room1 - a pointer/reference to Room.
    Input: RoomType* room2 - a pointer/reference to Room.
*/
void connectRooms(RoomType* room1, RoomType* room2);
/* 
  Function: addRoom
  Purpose:  Adds a RoomType to a RoomListType by allocating heap space for a RoomNodeType.
  Params:   
    Input: RoomListType* roomList - the list to add the room to
    Input: RoomType* room - the room to be added
*/
void addRoom(RoomListType** roomList, RoomType* room);
/* 
  Function: cleaupHouse
  Purpose:  Frees up allocated memory.
  Params:   
    Input: HouseType* house - a pointer/reference to House.
*/
void cleanupHouse(HouseType* house);


//* Functions: hunter.c
/*
  Function: initHunter
  Purpose: Initializes and allocates memory for a new Hunter.
  Params:
    Input: char* name - a character array or a string
    Input: enum EvidenceType equipment - a pointer/reference to enum equipment.
    Input: RoomType* room - the room to be added.
    Input: EvidenceListType* evidenceList - a pointer/reference to EvidenceList.
    Input: HunterType** hunter - a double pointer/reference to Hunter.
    Input: HouseType* house - a pointer/reference to House.
*/
void initHunter(char* name, enum EvidenceType equipment, RoomType* room,
                EvidenceListType* evidenceList, HunterType** hunter, HouseType* house);
/*
  Function: hunter_thread
  Purpose: Performs the specified operation for hunter_thread.
  Params:
    Input: void* arg - a pointer/reference to void.
*/
void* hunter_thread(void* arg);
/*
  Function: collectEvidence
  Purpose: Collects or gathers data for Evidence.
  Params:
    Input: HunterType* hunter - a pointer/reference to Hunter.
*/
void collectEvidence(HunterType* hunter);
/*
  Function: moveHunterRooms
  Purpose: Performs the specified operation for moveHunterRooms.
  Params:
    Input: HunterType* hunter - a pointer/reference to Hunter.
*/
void moveHunterRooms(HunterType* hunter);
/*
  Function: reviewEvidence
  Purpose: Performs the specified operation for reviewEvidence.
  Params:
    Input: HunterType* hunter - a pointer/reference to Hunter.
  Return: integer 1 if evidence is sufficient, 0 if not
*/
int reviewEvidence(HunterType* hunter);



//* Functions: ghost.c
/*
  Function: initGhost
  Purpose: Initializes and allocates memory for a new Ghost.
  Params:
    Input: GhostType** ghost - a pointer/reference to Ghost.
    Input: HouseType* house - a pointer/reference to House.
*/
void initGhost(GhostType** ghost, HouseType* house);
/*
  Function: addGhost
  Purpose: Adds or connects elements in the Ghost structure.
  Params:
    Input/Output: RoomListType* roomList - a pointer/reference to RoomList.
    Input: GhostType** ghost - a pointer/reference to Ghost.
*/
void addGhost(RoomListType* roomList, GhostType** ghost);
/*
  Function: chooseGhostAction
  Purpose: Performs the specified operation for chooseGhostAction.
  Params:
    Input: GhostType* ghost - a pointer/reference to Ghost.
    Input: int action - a numeric value representing action.
*/
void chooseGhostAction(GhostType* ghost, int action);
/*
  Function: moveGhostRooms
  Purpose: Performs the specified operation for moveGhostRooms.
  Params:
    Input: GhostType* ghost - a pointer/reference to Ghost.
*/
void moveGhostRooms(GhostType* ghost);
/*
  Function: leaveEvidence
  Purpose: Performs the specified operation for leaveEvidence.
  Params:
    Input: GhostType* ghost - a pointer/reference to Ghost.
*/
void leaveEvidence(GhostType* ghost);
/*
  Function: ghost_thread
  Purpose: Performs the specified operation for ghost_thread.
  Params:
    Input: void* arg - a pointer/reference to void.
*/
void* ghost_thread(void* arg);