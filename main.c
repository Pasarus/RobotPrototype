#include "robot.h"

//todo work on angles of the rotation of the robot.
//todo rewrite logic to handle going right before all else likely need to redo the whole orientation methods

int main() {
    FA_RobotInit();

    FA_BTSendString("Robot Start!!!!\n", 40);

    FA_PlayNote(100,100);


    struct robot robot = {0};
    robot.yCoord = STARTING_Y;
    robot.xCoord = STARTING_X;
    robot.facing = STARTING_FACE;
    int pathingComplete = false;

    while(!pathingComplete) {
        FA_LCDClear();

        int isMazeCompleted = mapCompleted(&robot);
        //If map is not completed, then map the maze etc.
        if (!isMazeCompleted) {
            FA_LCDPrint("Mapping!",10,0,0,FONT_NORMAL, LCD_OPAQUE);
            FA_DelayMillis(500);
            mapMaze(&robot);
        } // Then if the maze is completed path to the shaded area
        else {
            //DO NOTHING
            //Once shade found leave a "breadcrumb" on the previous cell, then follow this breadcrumb trail.
            FA_LCDPrint("Pathing!",10,0,0,FONT_NORMAL, LCD_OPAQUE);
            FA_DelayMillis(500);
            pathingBack(&robot);
            if(isShaded()){
                pathingComplete = true;
                FA_PlayNote(100,1000);
            }
        }
    }
}