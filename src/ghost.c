#include "defs.h"

void initGhost(GhostType** ghost) {
  *ghost = (GhostType*)allocMemory(sizeof(GhostType));

  (*ghost)->ghostClass = randomGhost();
  (*ghost)->currentRoom = NULL;
  (*ghost)->boredom = 0;
}

void addGhost(RoomListType* roomList, GhostType** ghost) {
  if(roomList->size<2){
    printf("Error: Need atleast 2 rooms");
    exit(C_FALSE);
  }

  int roomIndex = randInt(1,roomList->size-1);
  RoomNodeType* currentRoom = roomList->head;

  for(int i = 0; i<roomIndex; i++){
    
  }
  (*ghost)->currentRoom = roomList->head->next->data; //? temp: replace with random room which isn't Van
  
  roomList->head->next->data->ghost = *ghost;
}