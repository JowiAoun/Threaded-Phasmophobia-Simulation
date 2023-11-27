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