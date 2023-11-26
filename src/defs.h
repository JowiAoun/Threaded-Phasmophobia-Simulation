#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

typedef struct Room RoomType;
typedef struct House HouseType;
typedef struct Ghost GhostType;
typedef struct Hunter HunterType;
typedef struct RoomNode RoomNodeType;
typedef struct EvidenceNode EvidenceNodeType;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

//* Structures
struct Room {
  char              name[MAX_STR];
  HunterType*       hunters[NUM_HUNTERS];
  GhostType*        ghost;
  // RoomNodeType*     nextRoom;
  // RoomNodeType*     prevRoom;
  RoomNodeType*     roomConnections;
  EvidenceNodeType* nextEvidence;
  EvidenceNodeType* prevEvidence;
};

struct House {
  HunterType*       hunters[NUM_HUNTERS];
  RoomNodeType*     nextRoom;
  RoomNodeType*     prevRoom;
  EvidenceNodeType* nextEvidence;
  EvidenceNodeType* prevEvidence;
};

struct Ghost {
  GhostClass* ghostClass;
  RoomType*   currentRoom;
  int         boredom;
};

struct Hunter {
  char              name[MAX_STR];
  EvidenceType      equipment;
  RoomType*         room;
  EvidenceNodeType* nextEvidence;
  EvidenceNodeType* prevEvidence;
  int               fear;
  int               boredom;
};

struct RoomNode {
  RoomType* data;
  RoomType* next;
};

struct EvidenceNode {
  EvidenceType*  data;
  EvidenceType*  next;
};

//* Functions: utils.c
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter
void* allocMemory(size_t size);

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
void initHouse(HouseType* house);
void populateRooms(HouseType* house);
RoomType* createRoom(char name[]);
void connectRooms();
void addRoom();