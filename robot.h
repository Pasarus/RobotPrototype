//
// Created by srj12 on 14/03/2018.
//

#ifndef ROBOTPROTOTYPE_ROBOT_C_H
#define ROBOTPROTOTYPE_ROBOT_C_H

#include "robot.c"
#include "states.h"
#include <stdio.h>
#include "allcode_api.h"

#define MOVE_NORTH 0
#define MOVE_EAST 1
#define MOVE_WEST 2
#define MOVE_SOUTH 3

int moveMent(int, struct robot);
int logicChoice(struct robot);
int isPossible(struct robot);

#endif //ROBOTPROTOTYPE_ROBOT_C_H
