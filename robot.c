//
// Created by srj12 on 14/03/2018.
// So I should follow the right hand wall until I have completed the map, beeeeeeeeeeeeeeep,
// then at the point the map is completed go and find the shaded cover, then stop and beeeeeeeeeeep then stop totally.

#include "allcode_api.h"
#include "robot.h"

int isAWall(struct robot robot, int sensorValue){
    
}

void fillInCell(struct robot *robot){

}

/*
 * Returns 0 if false
 * 1 if true
 */
int mapCompleted(struct robot robot){
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            if(robot.maze[i][j].visited == 0){
                //Then maze is not completed
                return 0;
            }
        }
    }
    //If the maze has been completed then return true.
    return 1;
}

/*
 * Returns 0 if false
 * 1 if true
 */
int hasNeighbourBeenVisited(int direction ,struct robot robot){

}

void moveToNextCell(){
    //Move forward given distance to next cell
}

void moveEast(struct robot robot){
    //If facing North rotate 90 right then forwards
    //If facing East rotate 0 then forwards
    //If facing West rotate 180 right then forwards
    //If facing South rotate 90 left then forwards
    if(robot.facing == FACING_NORTH){
        FA_Right(90);
        moveToNextCell();
    }else if(robot.facing == FACING_EAST){
        moveToNextCell();
    }else if(robot.facing == FACING_WEST){
        FA_Right(180);
        moveToNextCell();
    }else if(robot.facing == FACING_SOUTH){
        FA_Left(90);
        moveToNextCell();
    }
}

void moveNorth(struct robot robot){
    //If facing North rotate 0 then forwards
    //If facing East rotate 90 left then forwards
    //If facing West rotate 90 right then forwards
    //If facing South rotate 180 right then forwards
    if(robot.facing == FACING_NORTH){
        moveToNextCell();
    }else if(robot.facing == FACING_EAST){
        FA_Left(90);
        moveToNextCell();
    }else if(robot.facing == FACING_WEST){
        FA_Right(90);
        moveToNextCell();
    }else if(robot.facing == FACING_SOUTH){
        FA_Right(180);
        moveToNextCell();
    }
}

void moveWest(struct robot robot){
    //If facing North rotate 90 left then forwards
    //If facing East rotate 180 right then forwards
    //If facing West rotate 0 then forwards
    //If facing South rotate 90 right then forwards
    if(robot.facing == FACING_NORTH){
        FA_Left(90);
        moveToNextCell();
    }else if(robot.facing == FACING_EAST){
        FA_Right(180);
        moveToNextCell();
    }else if(robot.facing == FACING_WEST){
        moveToNextCell();
    }else if(robot.facing == FACING_SOUTH){
        FA_Right(90);
        moveToNextCell();
    }
}

void moveSouth(struct robot robot){
    //If facing North rotate 180 right then forwards
    //If facing East rotate 90 right then forwards
    //If facing West rotate 90 left then forwards
    //If facing South rotate 0 then forwards
    if(robot.facing == FACING_NORTH){
        FA_Right(180);
        moveToNextCell();
    }else if(robot.facing == FACING_EAST){
        FA_Right(90);
        moveToNextCell();
    }else if(robot.facing == FACING_WEST){
        FA_Left(90);
        moveToNextCell();
    }else if(robot.facing == FACING_SOUTH){
        moveToNextCell();
    }
}

/*
 * Handles moving to the relevant cell
 * direction = 0; means North
 * direction = 1; means East
 * direction = 2; means West
 * direction = 3; means South
 */
int moveMent(int direction, struct robot robot){
    switch(direction){
        case 0:
            //Move forward
            moveNorth(robot);
            break;
        case 1:
            //Move left
            moveEast(robot);
            break;
        case 2:
            //Move right
            moveWest(robot);
            break;
        case 3:
            //Move backwards
            moveSouth(robot);
            break;
        default:
            //Everything is broken so make it beep once
            FA_PlayNote(100, 100);
            break;
    }
    return 0;
}

/*
 * Passes in the robot
 * The main choice function, aka a large if else set of statements that chooses what the robot should do next.
 * Returns a direction i.e.
 * forward = 0;
 * left = 1;
 * right = 2;
 * backwards = 3;
 */
void mapMaze(struct robot *robot){
    while(1) {
        fillInCell(robot);
        if(mapCompleted(*robot)){
            return;
        }
        //If Facing North and North is open then forwards and not visited then forwards
        //If facing North and North closed but East open then east
        //If facing North and North, East close but West open then west
        //If facing North and North, East West closed but South open
        //If facing North and North and been visited then forwards
        if (robot->facing == FACING_NORTH) {
            if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_NORTH, *robot)) {
                moveMent(MOVE_NORTH, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE) {
                moveMent(MOVE_EAST, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE) {
                moveMent(MOVE_WEST, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE) {
                moveMent(MOVE_SOUTH, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
                moveMent(MOVE_NORTH, *robot);
            } else {
                //Robot is stuck
                FA_PlayNote(100, 100);
                FA_DelayMillis(500);
                FA_PlayNote(100, 100);
            }
            //If facing South and South and not been visited is open the forwards
            //If facing South and East is open then forwards
            //If facing South and West is open then forwards
            //If facing South and North is open the forwards
            //If facing South and South has been visited then forwards
        } else if (robot->facing == FACING_SOUTH) {
            if(robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_SOUTH, *robot)){
                moveMent(MOVE_SOUTH, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_EAST, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
                moveMent(MOVE_WEST, *robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){

            }
            //If facing West and West and not been visited then forwards
            //If facing West and North is open then north
            //If facing West and South is open then south
            //If facing West and East is open then east
            //If facing West and West is open and has been visited then forwards
        } else if (robot->facing == FACING_WEST){
            if(robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_WEST, *robot)){
                moveMent(MOVE_WEST, *robot);
            }
            //If facing East and East is open and has not been visited then forwards
            //If facing East and South is open then South
            //If facing East and North is open then North
            //If facing East and West is open then West
            //If facing East and East is open and has been visited then forwards
        } else if (robot->facing == FACING_EAST){
            if(robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_EAST, *robot)){
                moveMent(MOVE_EAST, *robot);
            }
        }
    }
}