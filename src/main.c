#include "defs.h"

int main() {
  // Initialize the random number generator
  srand(time(NULL));

  // Create the house: You may change this, but it's here for demonstration purposes
  // Note: This code will not compile until you have implemented the house functions and structures
  HouseType house;
  initHouse(&house);
  populateRooms(&house);

  char hunterName[MAX_STR];
  for (int i = 1; i < 5; i++) {
    printf("\nEnter the name of hunter %d:", i);
    scanf("%s", hunterName);
    while (getchar() != '\n');
  }
  

  return C_TRUE;
}
