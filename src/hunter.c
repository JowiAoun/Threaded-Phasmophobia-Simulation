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

