#include "defs.h"

int main() {
  // Initialize the random number generator
  srand(time(NULL));

  // Create the house: You may change this, but it's here for demonstration purposes
  // Note: This code will not compile until you have implemented the house functions and structures
  HouseType house;
  initHouse(&house);
  populateRooms(&house); // 1.2

  // 1.1 & 1.3 - Get user input and place hunters in van room
  HunterType** hunter = NULL;
  char hunterName[MAX_STR];
  for (int i = 0; i < 4; i++) {
    printf("\nEnter the name of hunter %d: ", i+1);
    scanf("%s", hunterName);
    while (getchar() != '\n');
    
    initHunter(hunterName, 0, house.roomList->head->data, house.evidenceList, hunter); //? temp: replace argument 2 with specifications
    house.hunters[i] = *hunter;
  }

  // 1.4 - Initialize and add ghost to a room
  GhostType** ghost = NULL;
  initGhost(ghost);
  addGhost(house.roomList, ghost);

  return C_TRUE;
}
