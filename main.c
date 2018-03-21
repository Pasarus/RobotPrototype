#include "robot.h"

int main() {
    FA_RobotInit;

    struct robot robot;
    robot.yCoord = STARTING_Y;
    robot.xCoord = STARTING_X;
    robot.facing = STARTING_FACE;

    int isMazeCompleted = mapCompleted(robot);
    //If map is not completed, then map the maze etc.
    if(!isMazeCompleted){
        mapMaze(&robot);
    } // Then if the maze is completed path to the shaded area
    else{

    }
}