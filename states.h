//
// Created by srj12 on 14/03/2018.
//

#ifndef ROBOTPROTOTYPE_ROBOT_H
#define ROBOTPROTOTYPE_ROBOT_H

#include <stdbool.h>

#define FACING_NORTH 0
#define FACING_EAST 1
#define FACING_WEST 2
#define FACING_SOUTH 3

#define WALL_IS_THERE 1
#define WALL_ISNT_THERE 0

#define STARTING_X 2 //Because it's the third cell along
#define STARTING_Y 0
#define STARTING_FACE 3

//If greater than or equal to IS_WALL we will assume there is a wall there else not
#define IS_WALL 100

#define LIGHT_SHADE 100

#define TURN_RIGHT_ANGLE 90
#define TURN_LEFT_ANGLE 89

#define IS_CELL_LINE 20

#define TOO_CLOSE_WALL 300
#define STOP_FORWARD_DISTANCE 700

/*
 * int northWall = 0;
 * int eastWall = 1;
 * int westWall = 1;
 * int southWall = 0;
 * int shade = 0;
 * int visited = 1;
 *
 * Means that the cell has to walls present one of the left and right of the robot
 * If int shade = 1; then there is shade on the cell
 * If int visited = 1; then the cell has been visited before
 */

typedef struct cell {
    int northWall;
    int eastWall;
    int westWall;
    int southWall;
    int shade;
    int visited;
    int breadCrumb;
};

/*
 * The basic data structure representing the robot's data
 * facing = 0; North
 * facing = 1; East
 * facing = 2; West
 * facing = 3; South
 */
typedef struct robot {
    struct cell maze[4][4];
    int facing;
    int xCoord;
    int yCoord;
    int currentBreadCrumbNumber;
};

#endif //ROBOTPROTOTYPE_ROBOT_H