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
  addEvidence(hunter->evidenceList, hunter->equipment);
  removeEvidence(hunter->room->evidenceList, hunter->evidenceList, hunter->equipment);
  l_hunterCollect(hunter->name, hunter->equipment, hunter->room->name);
}

void moveHunterRooms(HunterType* hunter) {
  int roomIndex = randInt(0,hunter->room->roomList->size + 1);
  RoomNodeType* currentRoom = hunter->room->roomList->head;

  // Look for the room to add the hunter in
  for(int i = 0; i < roomIndex; i++) {
    currentRoom = currentRoom->next;
  }

  // Assert: room to add hunter is found
  for(int i = 0; i < NUM_HUNTERS; i++) {
    // Search the hunter to move
    printf("Moving Hunter %d: %s from room %s\n", i, hunter->name, hunter->room->name); //! temp
    if (hunter->room->hunters[i] != NULL) {
      printf("hunter name: %s\n", hunter->room->hunters[i]->name);
      if (strcmp(hunter->room->hunters[i]->name, hunter->name) == 0) {
        // Hunter found
        hunter->room->hunters[i] = NULL; // Remove the hunter from the room
        hunter->room = currentRoom->data; // Set the hunter's new room
        currentRoom->data->hunters[i] = hunter; // Set the room's hunter list
        l_hunterMove(hunter->name, currentRoom->data->name);
        break;
      }
    }
  }
}

// void reviewEvidence(HunterType* hunter) {

// }

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
        //collectEvidence(hunter);
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