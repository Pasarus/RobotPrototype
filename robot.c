//
// Created by srj12 on 14/03/2018.
// So I should follow the right hand wall until I have completed the map, beeeeeeeeeeeeeeep,
// then at the point the map is completed go and find the shaded cover, then stop and beeeeeeeeeeep then stop totally.
#include "allcode_api.h"
#include "robot.h"
#include "states.h"

//todo when shaded hard code reverse decision
int isShaded(){
    char message[40];
    sprintf(message, "Light Level: %d\n", FA_ReadLight());
    FA_BTSendString(message, 40);
    return FA_ReadLight() <= LIGHT_SHADE;
}



/*
 * It is a wall if the IR sensor is greater than the value
 */
int isAWall(struct robot robot, int direction){
    //The logic with this function is that it check
    switch(direction){
        //Checking North wall
        case FACING_NORTH:
            //And Facing North
            if(robot.facing == FACING_NORTH){
                //Check Front IR
                return FA_ReadIR(IR_FRONT) >= IS_WALL;
            } else if (robot.facing == FACING_EAST){
                //Check Left IR
                return FA_ReadIR(IR_LEFT) >= IS_WALL;
            } else if (robot.facing == FACING_WEST){
                //Check Right IR
                return FA_ReadIR(IR_RIGHT) >= IS_WALL;
            } else if (robot.facing == FACING_SOUTH){
                //Check Rear IR
                return FA_ReadIR(IR_REAR) >= IS_WALL;
            }
            break;
        //Checking East Wall
        case FACING_EAST:
            if(robot.facing == FACING_NORTH){
                //Check Right IR
                return FA_ReadIR(IR_RIGHT) >= IS_WALL;
            } else if (robot.facing == FACING_EAST){
                //Check Front IR
                return FA_ReadIR(IR_FRONT) >= IS_WALL;
            } else if (robot.facing == FACING_WEST){
                //Check Rear IR
                return FA_ReadIR(IR_REAR) >= IS_WALL;
            } else if (robot.facing == FACING_SOUTH){
                //Check Left IR
                return FA_ReadIR(IR_LEFT) >= IS_WALL;
            }
            break;
        //Checking West Wall
        case FACING_WEST:
            if(robot.facing == FACING_NORTH){
                //Check Left IR
                return FA_ReadIR(IR_LEFT) >= IS_WALL;
            } else if (robot.facing == FACING_EAST){
                //Check Rear IR
                return FA_ReadIR(IR_REAR) >= IS_WALL;
            } else if (robot.facing == FACING_WEST){
                //Check Front IR
                return FA_ReadIR(IR_FRONT) >= IS_WALL;
            } else if (robot.facing == FACING_SOUTH){
                //Check Right IR
                return FA_ReadIR(IR_RIGHT) >= IS_WALL;
            }
            break;
        //Checking South Wall
        case FACING_SOUTH:
            if(robot.facing == FACING_NORTH){
                //Check rear IR
                return FA_ReadIR(IR_REAR) >= IS_WALL;
            } else if (robot.facing == FACING_EAST){
                //Check Right IR
                return FA_ReadIR(IR_RIGHT) >= IS_WALL;
            } else if (robot.facing == FACING_WEST){
                //Check Left IR
                return FA_ReadIR(IR_LEFT) >= IS_WALL;
            } else if (robot.facing == FACING_SOUTH){
                //Check Front IR
                return FA_ReadIR(IR_FRONT) >= IS_WALL;
            }
            break;
        default:
            //There is an error so BEEEEEEP
            FA_PlayNote(100, 100);
            break;
    }
    return false;
}

void fillInCell(struct robot *robot){
    int xCoord = robot->xCoord;
    int yCoord = robot->yCoord;
    //Check the North Wall
    robot->maze[xCoord][yCoord].northWall = isAWall(*robot, FACING_NORTH);
    //Check the East Wall
    robot->maze[xCoord][yCoord].eastWall = isAWall(*robot, FACING_EAST);
    //Check the West Wall
    robot->maze[xCoord][yCoord].westWall = isAWall(*robot, FACING_WEST);
    //Check the South Wall
    robot->maze[xCoord][yCoord].southWall = isAWall(*robot, FACING_SOUTH);
    //Update whether or not to add a breadcrumb tracker
    if (robot->currentBreadCrumbNumber >= 1){
        robot->currentBreadCrumbNumber++;
        robot->maze[robot->xCoord][robot->yCoord].breadCrumb = robot->currentBreadCrumbNumber;
    }
    //Update Shade
    if (isShaded()){
        robot->maze[xCoord][yCoord].shade = true;
        robot->maze[xCoord][yCoord].breadCrumb = 1;
        robot->currentBreadCrumbNumber = 1;
    }
    //Update visited
    robot->maze[xCoord][yCoord].visited = 1;
}

/*
 * Returns 0 if false
 * 1 if true
 */
int mapCompleted(struct robot *robot){
    int i, j;
    for(i = 0; i<4; i++){
        for(j = 0; j<4; j++){
            if(robot->maze[i][j].visited == 0){
                //Then maze is not completed
                FA_BTSendString("Maze not complete!\n", 40);
                return false;
            }
        }
    }
    //If the maze has been completed then return true.
    FA_BTSendString("Maze is complete!\n", 40);
    return true;
}

/*
 * Returns 0 if false
 * 1 if true or cell is invalid
 */
int hasNeighbourBeenVisited(int direction ,struct robot robot){
    //If neighbour is out of bounds return true as a method of error checking
    switch(direction){
        case FACING_NORTH:
            //If checking North
            return robot.maze[robot.yCoord-1][robot.xCoord].visited;
        case FACING_EAST:
            //If checking East
            return robot.maze[robot.yCoord][robot.xCoord+1].visited;
        case FACING_SOUTH:
            //If checking South
            return robot.maze[robot.yCoord+1][robot.xCoord].visited;
        case FACING_WEST:
            //If checking West
            return robot.maze[robot.yCoord][robot.xCoord-1].visited;
        default:
            FA_PlayNote(100,100);
            FA_DelayMillis(500);
            FA_PlayNote(100,100);
            return false;
    }
}

void moveToNextCell(){
    //Move forward given distance to next cell
    //We are going to assume that the next cell is 15cm forward of the current location so we just want to move 15 cm right?
    //Move us towards the edge of the cell
    // Read wheel encoder current value
    // 1 unit = 0.32 mm of travel
    // 150 / 0.32 = 468.75 Therefore roughly 469

    FA_SetDriveSpeed(30);
    FA_Forwards(150);

    /*FA_ResetEncoders();
    FA_SetMotors(MOTOR_SPEED_LEFT,MOTOR_SPEED_RIGHT);

    //If Encoder value > 15cm rough equivelant Do nothing!!!!
    while(FA_ReadEncoder(0) <= 468 || FA_ReadEncoder(1) <= 468){
        //Do Nothing Wait for Encoders
    }

    //Stop now
    FA_SetMotors(0,0);
    FA_ResetEncoders();*/
}
//Moving North = y-1
//Moving South = y+1
//Moving West = x-1
//Moving East = x+1
void moveEast(struct robot *robot){
    //If facing North rotate 90 right then forwards
    //If facing East rotate 0 then forwards
    //If facing West rotate 180 right then forwards
    //If facing South rotate 90 left then forwards
    FA_BTSendString("Moving East!\n",40);
    if(robot->facing == FACING_NORTH){
        FA_Right(TURN_RIGHT_ANGLE);
        moveToNextCell();
        robot->xCoord = robot->xCoord+1;
    }else if(robot->facing == FACING_EAST){
        moveToNextCell();
        robot->xCoord = robot->xCoord+1;
    }else if(robot->facing == FACING_WEST){
        FA_Right(TURN_RIGHT_ANGLE*2);
        moveToNextCell();
        robot->xCoord = robot->xCoord+1;
    }else if(robot->facing == FACING_SOUTH){
        FA_Left(TURN_LEFT_ANGLE);
        moveToNextCell();
        robot->xCoord = robot->xCoord+1;
    }
    robot->facing = FACING_EAST;
}

void moveNorth(struct robot *robot){
    //If facing North rotate 0 then forwards
    //If facing East rotate 90 left then forwards
    //If facing West rotate 90 right then forwards
    //If facing South rotate 180 right then forwards
    FA_BTSendString("Moving North!\n", 40);
    if(robot->facing == FACING_NORTH){
        moveToNextCell();
        robot->yCoord = robot->yCoord-1;
    }else if(robot->facing == FACING_EAST){
        FA_Left(TURN_LEFT_ANGLE);
        moveToNextCell();
        robot->yCoord = robot->yCoord-1;
    }else if(robot->facing == FACING_WEST){
        FA_Right(TURN_RIGHT_ANGLE);
        moveToNextCell();
        robot->yCoord = robot->yCoord-1;
    }else if(robot->facing == FACING_SOUTH){
        FA_Right(TURN_RIGHT_ANGLE*2);
        moveToNextCell();
        robot->yCoord = robot->yCoord-1;
    }
    robot->facing = FACING_NORTH;
}

void moveWest(struct robot *robot){
    //If facing North rotate 90 left then forwards
    //If facing East rotate 180 right then forwards
    //If facing West rotate 0 then forwards
    //If facing South rotate 90 right then forwards
    FA_BTSendString("Moving West!\n", 40);
    if(robot->facing == FACING_NORTH){
        FA_Left(TURN_LEFT_ANGLE);
        moveToNextCell();
        robot->xCoord = robot->xCoord -1;
    }else if(robot->facing == FACING_EAST){
        FA_Right(TURN_RIGHT_ANGLE*2);
        moveToNextCell();
        robot->xCoord = robot->xCoord -1;
    }else if(robot->facing == FACING_WEST){
        moveToNextCell();
        robot->xCoord = robot->xCoord -1;
    }else if(robot->facing == FACING_SOUTH){
        FA_Right(TURN_RIGHT_ANGLE);
        moveToNextCell();
        robot->xCoord = robot->xCoord -1;
    }
    robot->facing = FACING_WEST;
}

void moveSouth(struct robot *robot){
    //If facing North rotate 180 right then forwards
    //If facing East rotate 90 right then forwards
    //If facing West rotate 90 left then forwards
    //If facing South rotate 0 then forwards
    FA_BTSendString("Moving South!\n", 40);
    if(robot->facing == FACING_NORTH){
        FA_Right(TURN_RIGHT_ANGLE*2);
        moveToNextCell();
        robot->yCoord = robot->yCoord+1;
    }else if(robot->facing == FACING_EAST){
        FA_Right(TURN_RIGHT_ANGLE);
        moveToNextCell();
        robot->yCoord = robot->yCoord+1;
    }else if(robot->facing == FACING_WEST){
        FA_Left(TURN_LEFT_ANGLE);
        moveToNextCell();
        robot->yCoord = robot->yCoord+1;
    }else if(robot->facing == FACING_SOUTH){
        moveToNextCell();
        robot->yCoord = robot->yCoord+1;
    }
    robot->facing = FACING_SOUTH;
}

/*
 * Handles moving to the relevant cell
 * direction = 0; means North
 * direction = 1; means East
 * direction = 2; means West
 * direction = 3; means South
 */
int moveMent(int direction, struct robot *robot){
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

        //Send currentCoords and facing to Serial
        char message[40];
        sprintf(message,"X_%d_Y_%d_FACE_%d\n",
                robot->xCoord,
                robot->yCoord,
                robot->facing
        );

        char message2[40];
        sprintf(message2, "BreadCrumb number: %d\n", robot->maze[robot->xCoord][robot->yCoord].breadCrumb);
        FA_BTSendString(message2,40);

        FA_BTSendString(message,40);

        fillInCell(robot);
        if(mapCompleted(robot)){
            return;
        }

        if (robot->maze[robot->xCoord][robot->yCoord].shade == true){
            FA_BTSendString("The cell is shaded!!!!!\n",40);
            if(robot->facing == FACING_NORTH){
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->facing == FACING_EAST){
                moveMent(MOVE_WEST, robot);
            } else if (robot->facing == FACING_WEST){
                moveMent(MOVE_EAST, robot);
            } else if (robot->facing == FACING_SOUTH){
                moveMent(MOVE_NORTH, robot);
            } else {
                FA_BTSendString("The robot face is wrong\n",40);
            }
        }

        //ADDED LOGIC THAT MAY NEED REMOVING STARTS ----------------------------
        //Currently this focuses on making the robot turn right if it can and it has not been visited, if not then it shall do the normal stuff
        else if (robot->facing == FACING_NORTH && robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Right!\n", 40);
            moveMent(FACING_EAST, robot);
        } else if (robot->facing == FACING_EAST && robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Right!\n", 40);
            moveMent(FACING_SOUTH, robot);
        } else if (robot->facing == FACING_WEST && robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Right!\n", 40);
            moveMent(FACING_NORTH, robot);
        } else if (robot->facing == FACING_SOUTH && robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Right!\n", 40);
            moveMent(FACING_WEST, robot);
        }

        //ADDED EXTRA LOGIC THAT MAY NEED REMOVING STARTS -----------------------------------
/*
        else if(robot->facing == FACING_NORTH && robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Left!\n", 40);
            moveMent(FACING_WEST, robot);
        } else if (robot->facing == FACING_EAST && robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Left!\n", 40);
            moveMent(FACING_NORTH, robot);
        } else if (robot->facing == FACING_WEST && robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Left!\n", 40);
            moveMent(FACING_SOUTH, robot);
        } else if (robot->facing == FACING_SOUTH && robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
            FA_BTSendString("Turning Left!\n", 40);
            moveMent(FACING_EAST, robot);
        }
        */
        //ADDED LOGIC THAT MAY NEED REMOVING ENDS --------------------------------

        //If Facing North and North is open then forwards and not visited then forwards
        //If facing North and North closed but East open then east
        //If facing North and North, East close but West open then west
        //If facing North and North, East West closed but South open
        //If facing North and North and been visited then forwards
        else if (robot->facing == FACING_NORTH) {
            if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE) {
                moveMent(MOVE_NORTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE) {
                moveMent(MOVE_EAST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE) {
                moveMent(MOVE_WEST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE) {
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
                moveMent(MOVE_NORTH, robot);
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
            if(robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
                moveMent(MOVE_WEST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_EAST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
                moveMent(MOVE_NORTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
                moveMent(MOVE_SOUTH, robot);
            } else {
                //Robot is stuck
                FA_PlayNote(100, 100);
                FA_DelayMillis(500);
                FA_PlayNote(100, 100);
            }
            //If facing West and West and not been visited then forwards
            //If facing West and North is open then north
            //If facing West and South is open then south
            //If facing West and East is open then east
            //If facing West and West is open and has been visited then forwards
        } else if (robot->facing == FACING_WEST){
            if(robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
                moveMent(MOVE_WEST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
                moveMent(MOVE_NORTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_EAST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
                moveMent(MOVE_WEST, robot);
            } else {
                //Robot is stuck
                FA_PlayNote(100, 100);
                FA_DelayMillis(500);
                FA_PlayNote(100, 100);
            }
            //If facing East and East is open and has not been visited then forwards
            //If facing East and South is open then South
            //If facing East and North is open then North
            //If facing East and West is open then West
            //If facing East and East is open and has been visited then forwards
        } else if (robot->facing == FACING_EAST){
            if(robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_EAST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
                moveMent(MOVE_NORTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
                moveMent(MOVE_WEST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_EAST, robot);
            } else {
                //Robot is stuck
                FA_PlayNote(100, 100);
                FA_DelayMillis(500);
                FA_PlayNote(100, 100);
            }
        }
        //Take a breather so it doesn't look stupid
        FA_DelayMillis(1000);
    }
}

//todo optimise to follow the lowest breadcrumb in neighbouring cells - currently just follows lowest
void pathingBack(struct robot *robot){
//Moving North = y-1
//Moving South = y+1
//Moving West = x-1
//Moving East = x+1
    while(1){
        int x = robot->xCoord;
        int y = robot->yCoord;
        struct cell cell = robot->maze[x][y];

        //Send currentCoords and facing to Serial
        char message[40];
        sprintf(message,"X_%d_Y_%d_FACE_%d\n",
                robot->xCoord,
                robot->yCoord,
                robot->facing
        );

        char message2[40];
        sprintf(message2, "BreadCrumb number: %d\n", robot->maze[robot->xCoord][robot->yCoord].breadCrumb);
        FA_BTSendString(message2,40);

        FA_BTSendString(message,40);


        //If shaded return out of function
        if(cell.shade){
            return;
        }
        //Else do all the other pathing code
        //IF north wall clear and north wall has breadcrumb 1 less to current breadcrumb then go here
        /*
        if (cell.northWall == WALL_ISNT_THERE && robot->maze[x][y-1].breadCrumb == robot->currentBreadCrumbNumber-1){
            moveMent(MOVE_NORTH, robot);
            robot->currentBreadCrumbNumber = robot->maze[robot->xCoord][robot->yCoord].breadCrumb;
        } else if (cell.westWall == WALL_ISNT_THERE && robot->maze[x-1][y].breadCrumb == robot->currentBreadCrumbNumber-1){
            moveMent(MOVE_WEST, robot);
            robot->currentBreadCrumbNumber = robot->maze[robot->xCoord][robot->yCoord].breadCrumb;
        } else if (cell.eastWall == WALL_ISNT_THERE && robot->maze[x+1][y].breadCrumb == robot->currentBreadCrumbNumber-1){
            moveMent(MOVE_EAST, robot);
            robot->currentBreadCrumbNumber = robot->maze[robot->xCoord][robot->yCoord].breadCrumb;
        } else if (cell.southWall == WALL_ISNT_THERE && robot->maze[x][y+1].breadCrumb == robot->currentBreadCrumbNumber-1){
            moveMent(MOVE_SOUTH, robot);
            robot->currentBreadCrumbNumber = robot->maze[robot->xCoord][robot->yCoord].breadCrumb;
        }*/

        int north, east, west, south;

        if(robot->maze[x][y].northWall == WALL_ISNT_THERE) north = robot->maze[x][y-1].breadCrumb; else north = 0;
        if(robot->maze[x][y].eastWall == WALL_ISNT_THERE) east = robot->maze[x+1][y].breadCrumb; else east = 0;
        if(robot->maze[x][y].westWall == WALL_ISNT_THERE) west = robot->maze[x-1][y].breadCrumb; else west = 0;
        if(robot->maze[x][y].southWall == WALL_ISNT_THERE) south = robot->maze[x][y+1].breadCrumb; else south = 0;

        if (north == 0) north = 9999;
        if (east == 0) east = 9999;
        if (west == 0) west = 9999;
        if (south == 0) south = 9999;

        char message3[70];
        sprintf(message3,
                "North: %d\nEast: %d\nWest: %d\nSouth%d\n",
                north, east, west, south);
        FA_BTSendString(message3, 70);

        //decide the largest one
        if(north < east && north < west && north < south){
            moveMent(MOVE_NORTH, robot);
        } else if (east < west && east < south){
            moveMent(MOVE_EAST, robot);
        } else if ( west < south){
            moveMent(MOVE_WEST, robot);
        } else {
            moveMent(MOVE_SOUTH, robot);
        }

    }

}