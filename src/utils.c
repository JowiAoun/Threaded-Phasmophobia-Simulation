#include "defs.h"

/*
  Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
    in:   lower end of the range of the generated number
    in:   upper end of the range of the generated number
  return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
  return (int) randFloat(min, max);
}

/*
  Returns a pseudo randomly generated floating point number.
  A few tricks to make this thread safe, just to reduce any chance of issues using random
    in:   lower end of the range of the generated number
    in:   upper end of the range of the generated number
  return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
  static __thread unsigned int seed = 0;
  if (seed == 0) {
    seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
  }

  float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
  float diff = max - min;
  float r = random * diff;
  return min + r;
}

/* 
  Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
  return (enum GhostClass) randInt(0, GHOST_COUNT);
}

/*
  Returns the string representation of the given enum EvidenceType.
    in: type - the enum EvidenceType to convert
    out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
  switch (type) {
    case EMF:
      strcpy(str, "EMF");
      break;
    case TEMPERATURE:
      strcpy(str, "TEMPERATURE");
      break;
    case FINGERPRINTS:
      strcpy(str, "FINGERPRINTS");
      break;
    case SOUND:
      strcpy(str, "SOUND");
      break;
    default:
      strcpy(str, "UNKNOWN");
      break;
  }
}

/* 
  Returns the string representation of the given enum GhostClass.
    in: ghost - the enum GhostClass to convert
    out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
  switch(ghost) {
    case BANSHEE:
      strcpy(buffer, "Banshee");
      break;
    case BULLIES:
      strcpy(buffer, "Bullies");
      break;
    case PHANTOM:
      strcpy(buffer, "Phantom");
      break;
    case POLTERGEIST:
      strcpy(buffer, "Poltergeist");
      break;
    default:
      strcpy(buffer, "Unknown");
      break;
    
  }
}

/* 
  Function: Allocate memory
  Purpose:  Allocates memory for a number of bytes and returns a pointer to it.
            If allocation fails, exits program with error code 1 (EXIT_FAILURE)
            This function was created by Jowi, for the assignment 4
  Params:
    Input: size_t size - the size of the memory to allocate
*/
void* allocMemory(size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) {
    printf("FATAL ERROR: Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void initEvidenceList(EvidenceListType** evidenceList) {
  *evidenceList = (EvidenceListType*)allocMemory(sizeof(EvidenceListType));
  (*evidenceList)->head = NULL;
  (*evidenceList)->tail = NULL;
  (*evidenceList)->size = 0;
}

void addEvidence(EvidenceListType* evidenceList, EvidenceType evType) {
  EvidenceNodeType* newEvidenceNode = (EvidenceNodeType*)allocMemory(sizeof(EvidenceNodeType));

  newEvidenceNode->data = evType;
  newEvidenceNode->next = NULL;

  // Define if the list has 0 or more elements
  if (evidenceList->head == NULL) {
    // Case: 0 nodes
    evidenceList->head = newEvidenceNode;
    evidenceList->tail = newEvidenceNode;

  } else {
    // Case: >=1 nodes
    evidenceList->tail->next = newEvidenceNode;
    evidenceList->tail = newEvidenceNode;
  }
  evidenceList->size++;
}

int removeEvidence(EvidenceListType* roomEvidenceList, EvidenceListType* hunterEvidenceList, EvidenceType evType) {
  if (roomEvidenceList->size < 1) {
    // Case: no evidence in the list
    return 0;
  }
  
  // Set the current evidence as the first one in the list
  EvidenceNodeType* currRoomEvidence = roomEvidenceList->head;
  EvidenceNodeType* prevRoomEvidence = NULL;

  // Iterate through the evidence list
  for (int i = 0; i < roomEvidenceList->size; i++) {
    if (currRoomEvidence->data == evType) {
      // Case: evidence found
      addEvidence(hunterEvidenceList, currRoomEvidence->data);
      
      if (i == 0) {
        // Case 0: evidence is at the head of the list
        roomEvidenceList->head = roomEvidenceList->head->next;
        if (roomEvidenceList->size < 2) {
          // Case: it is the only one in the room
          roomEvidenceList->tail = NULL;
        }
      } else if (i < roomEvidenceList->size-1) {
        // Case 1: evidence is in the middle of the list
        prevRoomEvidence->next = currRoomEvidence->next;
      } else {
        // Case 2: evidence is the last element of the list
        roomEvidenceList->tail = prevRoomEvidence;
        prevRoomEvidence->next = NULL;
      }
      
      roomEvidenceList->size--;
      free(currRoomEvidence);
      return 1;
    }

    prevRoomEvidence = currRoomEvidence;
    currRoomEvidence = currRoomEvidence->next;
  }

  return 0;
}