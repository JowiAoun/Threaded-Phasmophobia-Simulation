#include "defs.h"

void initGhost(GhostType** ghost) {
  *ghost = (GhostType*)allocMemory(sizeof(GhostType));

  (*ghost)->ghostClass = randomGhost();
  (*ghost)->currentRoom = NULL;
  (*ghost)->boredom = 0;
}

void addGhost(RoomListType* roomList, GhostType** ghost) {
  // Check if there is less than 2 rooms
  if(roomList->size<2){
    printf("Error: Need atleast 2 rooms");
    exit(C_FALSE);
  }

  // Iterate over room list
  int roomIndex = randInt(1,roomList->size-1);
  RoomNodeType* currentNode = roomList->head;

  for(int i = 0; i<roomIndex; i++) {
    currentNode = currentNode->next;
  }

  currentNode->data->ghost = *ghost;
  (*ghost)->currentRoom = currentNode->data;
}

void chooseGhostAction(GhostType* ghost, int options) {
  //TODO: optimize (and function call in ghost_thread)
  switch (options) {
    case 0:
      break; // do nothing
    case 1:
      leaveEvidence(ghost);
      break;
    case 2:
      moveRooms(ghost);
      break;
  }
}

void moveRooms(GhostType* ghost) {
  // 2.3 - Move to an adjacent room
  int roomIndex = randInt(0,ghost->currentRoom->roomList->size-1);
  RoomNodeType* currentRoom = ghost->currentRoom->roomList->head;

  for(int i = 0; i <roomIndex; i++){
    currentRoom = currentRoom->next;
  }
  currentRoom->data->ghost = NULL;
  ghost->currentRoom = currentRoom->data;
  currentRoom->data->ghost = ghost;
  l_ghostMove(currentRoom->data->name);
}

void leaveEvidence(GhostType* ghost) {
  // 2.4 - Leave evidence
  int evType = randInt(0, 2);
  EvidenceType evTypeArray[3];

  switch (ghost->ghostClass) {
    case POLTERGEIST:
      evTypeArray[0] = EMF;
      evTypeArray[1] = TEMPERATURE;
      evTypeArray[2] = FINGERPRINTS;
      addEvidence(ghost->currentRoom->evidenceList, evTypeArray[evType]);
      break;
    case BANSHEE:
      evTypeArray[0] = EMF;
      evTypeArray[1] = TEMPERATURE;
      evTypeArray[2] = SOUND;
      addEvidence(ghost->currentRoom->evidenceList, evTypeArray[evType]);
      break;
    case BULLIES:
      evTypeArray[0] = EMF;
      evTypeArray[1] = FINGERPRINTS;
      evTypeArray[2] = SOUND;
      addEvidence(ghost->currentRoom->evidenceList, evTypeArray[evType]);
      break;
    case PHANTOM:
      evTypeArray[0] = TEMPERATURE;
      evTypeArray[1] = FINGERPRINTS;
      evTypeArray[2] = SOUND;
      addEvidence(ghost->currentRoom->evidenceList, evTypeArray[evType]);
      break;
  }
}

void* ghost_thread(void* arg) {
  GhostType* ghost = (GhostType*) arg;

  while (ghost->boredom < BOREDOM_MAX) {
    usleep(GHOST_WAIT);

    if (ghost->currentRoom->hunters[0] != NULL) {
      // 2.1 - Hunter in the room: leave evidence OR do nothing
      ghost->boredom = 0;
      int action = randInt(0, 1);
      chooseGhostAction(ghost, action);
    } else {
      // 2.2 - Hunter not in the room: leave evidence OR move rooms OR do nothing
      ghost->boredom++;
      int action = randInt(0, 2);
      chooseGhostAction(ghost, action);
    }
  }

  // 2.5 - Exit thread statements
  l_ghostExit(LOG_BORED);
  return NULL;
}