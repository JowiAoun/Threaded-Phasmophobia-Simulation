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

void initEvidenceList(EvidenceListType* evidenceList) {
  evidenceList->head = NULL;
  evidenceList->tail = NULL;
  evidenceList->size = 0;
}

void addEvidence(EvidenceListType* evidenceList, EvidenceType evType) {
  EvidenceNodeType* newEvidenceNode = (EvidenceNodeType*)allocMemory(sizeof(EvidenceNodeType));

  newEvidenceNode->data = &evType;
  newEvidenceNode->next = NULL;

  evidenceList->tail->next = newEvidenceNode;
  evidenceList->size++;
}

void removeEvidence(EvidenceListType* evidenceList, EvidenceType evType) {
  EvidenceNodeType* currNode = evidenceList->head;

  while (currNode->next != NULL) {
    currNode = currNode->next;
    if (currNode->data == evType) {
      break;
    }
  }
}