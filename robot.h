//
// Created by srj12 on 14/03/2018.
//

#ifndef ROBOTPROTOTYPE_ROBOT_C_H
#define ROBOTPROTOTYPE_ROBOT_C_H

#include "states.h"
#include <stdio.h>
#include "allcode_api.h"

#define MOVE_NORTH 0
#define MOVE_EAST 1
#define MOVE_WEST 2
#define MOVE_SOUTH 3

int moveMent(int, struct robot*);
int isShaded();
int isAWall(struct robot, int);
void fillInCell(struct robot*);
int mapCompleted(struct robot);
int hasNeighbourBeenVisited(int ,struct robot);
void moveToNextCell();
void moveEast(struct robot*);
void moveWest(struct robot*);
void moveNorth(struct robot*);
void moveSouth(struct robot*);
void mapMaze(struct robot*);



#endif //ROBOTPROTOTYPE_ROBOT_C_H
