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
void moveHunterRooms(HunterType* hunter){
  int roomIndex = randInt(0,hunter->room->roomList->size-1);
  RoomNodeType* currentRoom = hunter->room->roomList->head;
  for(int i = 0; i <roomIndex; i++){
    currentRoom = currentRoom->next;
  }
  for(int i = 0; i < NUM_HUNTERS; i++){
  currentRoom->data->hunter[i] = NULL;
  currentRoom->data->hunters

  }
  currentRoom->data->hunter = NULL;
  hunter->room = currentRoom->data;
  currentRoom->data->hunter = hunter;
  l_ghostMove(currentRoom->data->name);


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