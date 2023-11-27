#include "defs.h"

void initGhost(GhostType** ghost) {
  *ghost = (GhostType*)allocMemory(sizeof(GhostType));

  (*ghost)->ghostClass = randomGhost();
  (*ghost)->currentRoom = NULL;
  (*ghost)->boredom = 0;
}

void addGhost(RoomListType* roomList, GhostType** ghost) {
  (*ghost)->currentRoom = roomList->head->next->data; //? temp: replace with random room which isn't Van
  roomList->head->next->data->ghost = *ghost;
}