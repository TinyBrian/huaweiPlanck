#include "Robot.h"

Robot robots[4];
extern SqLinkedList *aquireList[7];
extern SqLinkedList *sellingList[7];

void Robot_init(Robot *robot, RobotID robotID, double x, double y)
{

    robot->robotID = robotID;
    robot->coordinate[0] = x;
    robot->coordinate[1] = y;
    robot->angelVelocity = 0;
    robot->faceTo = 0;
    robot->good = 0;
    robot->knockValue = 0;
    robot->linearVelocity[0] = 0;
    robot->linearVelocity[1] = 0;
    robot->timeValue = 0;
    robot->workbench = 0;
    SqLinkedList l;
    SqLinkedList_init(&l);
    robot->potentialValue = 0;
    robot->targetIndex = 0;
}

void Robot_update(Robot *robot, double x, double y, int workbench,
                  int good,
                  double timeValue,
                  double knockValue,
                  double angelVelocity,
                  double linearVelocity[2],
                  double faceTo, int targetIndex)
{
    robot->coordinate[0] = x;
    robot->coordinate[1] = y;
    robot->workbench = workbench;
    robot->good = good;
    robot->angelVelocity = angelVelocity;
    robot->linearVelocity[0] = linearVelocity[0];
    robot->linearVelocity[1] = linearVelocity[1];
    robot->faceTo = faceTo;
    robot->knockValue = knockValue;
    robot->timeValue = timeValue;
    robot->targetIndex = targetIndex;
}

double getOrderedSellList(Robot robot)
{
    int good = robot.good;
}

int getProfit(int good)
{
    switch (good)
    {
    case 1:
        return 3000;
        break;
    case 2:
        return 3200;
        break;
    case 3:
        return 3400;
        break;
    case 4:
        return 7100;
        break;
    case 5:
        return 7800;
        break;
    case 6:
        return 8300;
        break;
    case 7:
        return 29000;
        break;
    }
}

double getDistance(Robot r, Workbench w)
{
    double x = r.coordinate[0], y = r.coordinate[1], x0 = w.coordinate[0], y0 = w.coordinate[1];
    return sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
}

int isRobotsJecked()
{
    for (int i = 0; i < 3; i++)
    {
    }
}
