#include "defs.h"

void initGhost(GhostType** ghost, HouseType* house) {
  *ghost = (GhostType*)allocMemory(sizeof(GhostType));

  (*ghost)->ghostClass = randomGhost();
  (*ghost)->currentRoom = NULL;
  (*ghost)->boredom = 0;
  (*ghost)->house = house;
  (*ghost)->house->ghostClass = (*ghost)->ghostClass;
}

void addGhost(RoomListType* roomList, GhostType** ghost) {
  // Check if there is less than 2 rooms
  if (roomList->size < 2) {
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

  l_ghostInit((*ghost)->ghostClass, (*ghost)->currentRoom->name);
}

void chooseGhostAction(GhostType* ghost, int action) {
  //TODO: optimize (and function call in ghost_thread)
  switch (action) {
    case 0:
      break; // do nothing
    case 1:
      leaveEvidence(ghost);
      break;
    case 2:
      moveGhostRooms(ghost);
      break;
  }
}

void moveGhostRooms(GhostType* ghost) {
  // 2.3 - Move to an adjacent room
  int roomIndex = randInt(0,ghost->currentRoom->roomList->size);
  RoomNodeType* currentRoom = ghost->currentRoom->roomList->head;

  for(int i = 0; i <roomIndex; i++){
    currentRoom = currentRoom->next;
  }

  ghost->currentRoom->ghost = NULL;
  ghost->currentRoom = currentRoom->data;
  currentRoom->data->ghost = ghost;
  l_ghostMove(currentRoom->data->name);
}

void leaveEvidence(GhostType* ghost) {
    // 2.4 - Leave evidence
    EvidenceType evTypeArray[3] = {EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN}; // Initialize with unknown evidence types

    // Assign evidence types based on ghost class
    switch (ghost->ghostClass) {
        case POLTERGEIST:
            evTypeArray[0] = EMF;
            evTypeArray[1] = TEMPERATURE;
            evTypeArray[2] = FINGERPRINTS;
            break;
        case BANSHEE:
            evTypeArray[0] = EMF;
            evTypeArray[1] = TEMPERATURE;
            evTypeArray[2] = SOUND;
            break;
        case BULLIES:
            evTypeArray[0] = EMF;
            evTypeArray[1] = FINGERPRINTS;
            evTypeArray[2] = SOUND;
            break;
        case PHANTOM:
            evTypeArray[0] = TEMPERATURE;
            evTypeArray[1] = FINGERPRINTS;
            evTypeArray[2] = SOUND;
            break;
        default:
            printf("Error: Unknown ghost class\n");
            exit(C_FALSE); // Handle unknown ghost class
    }

    // Randomly select an evidence type from the array
    int evIndex = randInt(0, 3);
    EvidenceType selectedEvType = evTypeArray[evIndex];

    // Add the selected evidence type to the room's evidence list
    addEvidence(ghost->currentRoom->evidenceList, selectedEvType);

    // Log the evidence left
    l_ghostEvidence(selectedEvType, ghost->currentRoom->name);
}

void* ghost_thread(void* arg) {
  GhostType* ghost = (GhostType*) arg;
  int action;

  while (ghost->boredom < BOREDOM_MAX) {
    usleep(GHOST_WAIT);
    pthread_mutex_lock(&(ghost->house->mutex));
    int won = ghost->house->huntersWon;
    pthread_mutex_unlock(&(ghost->house->mutex));

    if (won) {
      break;
    }

    if (ghost->currentRoom->hunters[0] != NULL) {
      
      // 2.1 - Hunter in the room: leave evidence OR do nothing
      ghost->boredom = 0;
      action = randInt(0, 2);
      chooseGhostAction(ghost, action);
    } else {
      // 2.2 - Hunter not in the room: leave evidence OR move rooms OR do nothing
      ghost->boredom++;
      action = randInt(0, 3);
      chooseGhostAction(ghost, action);
    }
  }

  // 2.5 - Exit thread statements
  ghost->currentRoom->ghost = NULL;
  l_ghostExit(LOG_BORED);
  return NULL;
}