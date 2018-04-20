//
// Created by srj12 on 14/03/2018.
// So I should follow the right hand wall until I have completed the map, beeeeeeeeeeeeeeep,
// then at the point the map is completed go and find the shaded cover, then stop and beeeeeeeeeeep then stop totally.
#include "allcode_api.h"
#include "robot.h"
#include "states.h"

int isShaded(){
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
    //Update Shade
    robot->maze[xCoord][yCoord].shade = isShaded();
    //Update visited
    robot->maze[xCoord][yCoord].visited = 1;
}

/*
 * Returns 0 if false
 * 1 if true
 */
int mapCompleted(struct robot robot){
    int i, j;
    for(i = 0; i<4; i++){
        for(j = 0; j<4; j++){
            if(robot.maze[i][j].visited == 0){
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
            if(robot.yCoord-1 > 3 || robot.yCoord-1 < 0){
                return true;
            }
            return robot.maze[robot.yCoord-1][robot.xCoord].visited;
        case FACING_EAST:
            //If checking East
            if(robot.xCoord+1 > 3 || robot.xCoord+1 < 0){
                return true;
            }
            return robot.maze[robot.yCoord][robot.xCoord+1].visited;
        case FACING_SOUTH:
            //If checking South
            if(robot.yCoord+1 > 3 || robot.yCoord+1 < 0){
                return true;
            }
            return robot.maze[robot.yCoord+1][robot.xCoord].visited;
        case FACING_WEST:
            //If checking West
            if(robot.yCoord-1 > 3 || robot.yCoord-1 < 0){
                return true;
            }
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

    FA_SetDriveSpeed(20);
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
        FA_BTSendString(message,40);


        fillInCell(robot);
        if(mapCompleted(*robot)){
            return;
        }

        //ADDED LOGIC THAT MAY NEED REMOVING STARTS ----------------------------
        //todo add a check to this code to check for a wall on west
        //If Facing North and West is not visited and no west wall then go west
        if(robot->facing == FACING_NORTH && !hasNeighbourBeenVisited(FACING_WEST, *robot) &&
                robot->maze[robot->yCoord][robot->xCoord].westWall == WALL_ISNT_THERE){
            moveMent(MOVE_WEST, robot);
        }
        //If Facing West and South not visited and no south wall then go south
        else if (robot->facing == FACING_WEST && !hasNeighbourBeenVisited(FACING_SOUTH, *robot) &&
                robot->maze[robot->yCoord][robot->xCoord].southWall == WALL_ISNT_THERE){
            moveMent(MOVE_SOUTH, robot);
        }
        //If Facing South and East not visited and no east wall then go east.
        else if (robot->facing == FACING_SOUTH && !hasNeighbourBeenVisited(FACING_EAST, *robot) &&
                robot->maze[robot->yCoord][robot->xCoord].eastWall == WALL_ISNT_THERE){
            moveMent(MOVE_EAST, robot);
        }
        //If Facing East and North not visited and no north wall then go North
        else if (robot->facing == FACING_EAST && !hasNeighbourBeenVisited(FACING_NORTH, *robot) &&
                robot->maze[robot->yCoord][robot->xCoord].northWall == WALL_ISNT_THERE){
            moveMent(MOVE_NORTH, robot);
        }
        //ADDED LOGIC THAT MAY NEED REMOVING ENDS --------------------------------

        //If Facing North and North is open then forwards and not visited then forwards
        //If facing North and North closed but East open then east
        //If facing North and North, East close but West open then west
        //If facing North and North, East West closed but South open
        //If facing North and North and been visited then forwards
        else if (robot->facing == FACING_NORTH) {
            if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_NORTH, *robot)) {
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
            if(robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_SOUTH, *robot)){
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_EAST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE){
                moveMent(MOVE_WEST, robot);
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
            if(robot->maze[robot->xCoord][robot->yCoord].westWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_WEST, *robot)){
                moveMent(MOVE_WEST, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].northWall == WALL_ISNT_THERE){
                moveMent(MOVE_NORTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].southWall == WALL_ISNT_THERE){
                moveMent(MOVE_SOUTH, robot);
            } else if (robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE){
                moveMent(MOVE_SOUTH, robot);
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
            if(robot->maze[robot->xCoord][robot->yCoord].eastWall == WALL_ISNT_THERE && !hasNeighbourBeenVisited(MOVE_EAST, *robot)){
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