#include "defs.h"


RoomType* createRoom(char name[]){
  RoomType* room = (RoomType*)allocMemory(sizeof(RoomType));
  
  strncpy(room->name, name, MAX_STR);
  room->ghost = NULL;
  initRoomList(room->roomList);
  initEvidenceList(room->evidenceList);

  return room;
}

void connectRooms(RoomType* room1, RoomType* room2) {
  addRoom(room1->roomList, room2);
  addRoom(room2->roomList, room1);
}

void addRoom(RoomListType *roomList, RoomType *room) {
	// Create new node with ghost data to add at the end of linked list
	RoomNodeType* newNode = (RoomNodeType*)allocMemory(sizeof(RoomNodeType));
	
	// Set node's fields
	newNode->next = NULL;
	newNode->data = room;

	// Define if the list has 0 or more elements
	if (roomList->head == NULL) {
		// Case: 0 nodes
		roomList->head = newNode;
		roomList->tail = newNode;

	} else {
		// Case: >=1 nodes
		roomList->tail->next = newNode;
		roomList->tail = newNode;
	}
}

void populateRooms(HouseType* house) {
  // First, create each room

  // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
  // create functions are pretty typical, but it means errors are harder to return aside from NULL
  struct Room* van                = createRoom("Van");
  struct Room* hallway            = createRoom("Hallway");
  struct Room* master_bedroom     = createRoom("Master Bedroom");
  struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
  struct Room* bathroom           = createRoom("Bathroom");
  struct Room* basement           = createRoom("Basement");
  struct Room* basement_hallway   = createRoom("Basement Hallway");
  struct Room* right_storage_room = createRoom("Right Storage Room");
  struct Room* left_storage_room  = createRoom("Left Storage Room");
  struct Room* kitchen            = createRoom("Kitchen");
  struct Room* living_room        = createRoom("Living Room");
  struct Room* garage             = createRoom("Garage");
  struct Room* utility_room       = createRoom("Utility Room");

  // This adds each room to each other's room lists
  // All rooms are two-way connections
  connectRooms(van, hallway);
  connectRooms(hallway, master_bedroom);
  connectRooms(hallway, boys_bedroom);
  connectRooms(hallway, bathroom);
  connectRooms(hallway, kitchen);
  connectRooms(hallway, basement);
  connectRooms(basement, basement_hallway);
  connectRooms(basement_hallway, right_storage_room);
  connectRooms(basement_hallway, left_storage_room);
  connectRooms(kitchen, living_room);
  connectRooms(kitchen, garage);
  connectRooms(garage, utility_room);

  // Add each room to the house's room list
  addRoom(&house->roomList, van);
  addRoom(&house->roomList, hallway);
  addRoom(&house->roomList, master_bedroom);
  addRoom(&house->roomList, boys_bedroom);
  addRoom(&house->roomList, bathroom);
  addRoom(&house->roomList, basement);
  addRoom(&house->roomList, basement_hallway);
  addRoom(&house->roomList, right_storage_room);
  addRoom(&house->roomList, left_storage_room);
  addRoom(&house->roomList, kitchen);
  addRoom(&house->roomList, living_room);
  addRoom(&house->roomList, garage);
  addRoom(&house->roomList, utility_room);
}

void initHouse(HouseType* house) {
  HouseType* house = (HouseType*)allocMemory(sizeof(HouseType));
  
  initRoomList(house->roomList);
  initEvidenceList(house->evidenceList);
}

void initRoomList(RoomListType* roomList) {
  roomList->head = NULL;
	roomList->tail = NULL;
}

//* Random Room Assign Function
// RoomType* getRandomRoom(HouseType* house){
//   RoomNodeType* current = house->roomList->head;
//   int roomCount = 0;

//   while (current != NULL){
//     if(strcmp(current->data->name, "Van") != 0){
//       roomCount++;
//     }
//   }
//   if(roomCount == 0){
//     return NULL;
//   }
//   int roomIndex = randInt(0,roomCount);

//   current = house->roomList->head;
//   while (current != NULL && roomIndex >= 0){
//     if(strcmp(current->data->name, "Van") !=0){
//       if(roomIndex == 0){
//         return current->data;
//       }
//       roomIndex--;
//     }
//     current = current->next;
//   }

//   return NULL;

// }