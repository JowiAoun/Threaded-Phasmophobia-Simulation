#include "defs.h"

void initHunter(char* name, enum EvidenceType equipment,
                RoomType* room, EvidenceListType* evidenceList, HunterType** hunter){

  *hunter = (HunterType*)allocMemory(sizeof(HunterType));

  strcpy((*hunter)->name, name);
  (*hunter)->equipment = equipment;
  (*hunter)->room = room;
  (*hunter)->evidenceList = evidenceList;
  (*hunter)->fear = 0;
  (*hunter)->boredom = 0;
}

void collectEvidence(HunterType* hunter) {
  if (hunter->room->evidenceList->head != NULL) {
    int ret = removeEvidence(hunter->room->evidenceList, hunter->evidenceList, hunter->equipment);
    if (ret == 1) {
      //printf("--- Evidence taken\n"); //!temp
      addEvidence(hunter->evidenceList, hunter->equipment);
      l_hunterCollect(hunter->name, hunter->equipment, hunter->room->name);
    }
  } else {
    printf("No evidence in room\n"); //! temp
  }
}

void moveHunterRooms(HunterType* hunter) {
  // Select a random new room
  int roomIndex = randInt(0, hunter->room->roomList->size);
  RoomNodeType* newRoomNode = hunter->room->roomList->head;
  
  // Traverse to the selected room
  for (int i = 0; i < roomIndex; i++) {
    newRoomNode = newRoomNode->next;
  }

  RoomType* newRoom = newRoomNode->data;

  // Remove hunter from current room
  for (int i = 0; i < NUM_HUNTERS; i++) {
    if (hunter->room->hunters[i] == hunter) {
      hunter->room->hunters[i] = NULL;
      break;
    }
  }

  // Add hunter to new room
  for (int i = 0; i < NUM_HUNTERS; i++) {
    if (newRoom->hunters[i] == NULL) {
      newRoom->hunters[i] = hunter;
      hunter->room = newRoom;
      l_hunterMove(hunter->name, newRoom->name);
      return;
    }
  }
}


void reviewEvidence(HunterType* hunter) {
  
}

void* hunter_thread(void* arg) {
  HunterType* hunter = (HunterType*) arg;

  while (1) {
    usleep(HUNTER_WAIT);

    // 3.1
    if(hunter->room->ghost != NULL){
      hunter->fear += 1;
      hunter->boredom = 0;
    }
    else{
      hunter->boredom += 1;
    }
    // 3.2
    int action = randInt(0,2);
    
    switch(action){
      case 0:
        collectEvidence(hunter);
        break;
      case 1:
        moveHunterRooms(hunter);
        break;
      case 2:
        break;
    }

    // TODO: check if boredom_max should be > or >=
    if (hunter->boredom > BOREDOM_MAX) {
      l_hunterExit(hunter->name, LOG_BORED);
      return NULL;
    } else if (hunter->fear > FEAR_MAX) {
      l_hunterExit(hunter->name, LOG_FEAR);
      return NULL;
    }
  }
}