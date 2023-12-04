#include "defs.h"

int main() {
  // Initialize the random number generator
  srand(time(NULL));

  // Create the house: You may change this, but it's here for demonstration purposes
  // Note: This code will not compile until you have implemented the house functions and structures
  HouseType* house = NULL;
  initHouse(&house);
  populateRooms(house); // 1.2

  // 1.1 & 1.3 - Get user input and place hunters in van room
  HunterType* hunter = NULL;
  char hunterName[MAX_STR];
  for (int i = 0; i < NUM_HUNTERS; i++) {
    printf("\nEnter the name of hunter %d: ", i+1);
    scanf("%s", hunterName);
    while (getchar() != '\n');
    //TODO: Handle the case where hunters have the same name
    initHunter(hunterName, i, house->roomList->head->data, house->evidenceList, &hunter, house);
    house->hunters[i] = hunter;
    l_hunterInit(hunter->name, hunter->equipment);
  }
  printf("\n\n");

  // 1.4 - Initialize and add ghost to a room
  GhostType* ghost = NULL;
  initGhost(&ghost, house);
  addGhost(house->roomList, &ghost);

  // 1.5 - Create threads
  pthread_t threads[NUM_HUNTERS+1]; // Indexes 0-4: hunters - Index 5: ghost.
  for (int i = 0; i < NUM_HUNTERS; i++) {
    pthread_create(&threads[i], NULL, hunter_thread, (void*) house->hunters[i]);
  }
  pthread_create(&threads[NUM_HUNTERS], NULL, ghost_thread, (void *) ghost); // ghost thread

  // Join threads
  for (int i = 0; i < NUM_HUNTERS+1; i++) {
    pthread_join(threads[i], NULL);
  }

  // Define who won
  printf("=======================================\n");
  printf("All done! Let's tally the results...   \n");
  printf("=======================================\n");
  printf("---------------------------------------\n");

  if (house->huntersWon == 1) {
    char ghostClassString[MAX_STR];
    ghostToString(ghost->ghostClass, ghostClassString);
    printf("It seems the ghost has been discovered!\nThe hunters have won the game!\n");
    printf("The hunters concluded the ghost is a %s using the evidence they found:\n", ghostClassString);
  } else {
    printf("TEMP: GHOST HAS WON"); //! temp
  }

  cleanupHouse(house);
  free(ghost);

  return C_TRUE;
}