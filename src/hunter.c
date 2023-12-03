#include "defs.h"

void initHunter(char* name, enum EvidenceType equipment,
                RoomType* room, EvidenceListType* evidenceList, HunterType** hunter){

  *hunter = (HunterType*)allocMemory(sizeof(HunterType));

  strcpy((*hunter)->name, name);
  (*hunter)->equipment = equipment;
  (*hunter)->room = room;
  (*hunter)->evidenceList = evidenceList;
  (*hunter)->fear = 0;
  (*hunter)->room = 0;
}

void collectEvidence(HunterType* hunter) {
  //TODO: Call addEvidence and removeEvidence
}

void moveHunterRooms(HunterType* hunter) {
  int roomIndex = randInt(0,hunter->room->roomList->size);
  RoomNodeType* currentRoom = hunter->room->roomList->head;

  // Look for the room to add the hunter in
  for(int i = 0; i <roomIndex; i++) {
    currentRoom = currentRoom->next;
  }

  // Assert: room to add hunter is found
  for(int i = 0; i < NUM_HUNTERS; i++) {
    // Search the hunter to move
    if (strcmp(currentRoom->data->hunters[i]->name, hunter->name) == 0) {
      // Hunter found
      hunter->room->hunters[i] = NULL; // Remove the hunter from the room
      hunter->room = currentRoom->data; // Set the hunter's new room
      currentRoom->data->hunters[i] = hunter; // Set the room's hunter list
      l_ghostMove(currentRoom->data->name);
    }
  }
}

void* hunter_thread(void* arg) {
  HunterType* hunter = (HunterType*) arg;

  while (1) {
    // 3.1
    if(hunter->room->ghost != NULL){
      hunter->fear += 1;
      hunter->boredom = 0;
    }
    else{
      hunter->boredom += 1;
    }
    


    usleep(HUNTER_WAIT);
  }

  l_hunterExit(hunter->name, LOG_FEAR); //TODO: change second argument to proper log
  return NULL;
}