#include "robot.h"

int main() {
    FA_RobotInit;
    struct robot robot;
    while(1){
        logicChoice(robot);
    }
}