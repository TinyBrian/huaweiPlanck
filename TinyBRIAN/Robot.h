#ifndef ROBOT_H
#define ROBOT_H

#include "main.h"

typedef struct node *SqLinkedList;
typedef enum
{
    Robot1 = 0,
    Robot2,
    Robot3,
    Robot4
} RobotID;

typedef struct r
{
    RobotID robotID;
    int workbench;
    int good;
    double timeValue;
    double knockValue;
    double angelVelocity;
    double linearVelocity[2];
    double faceTo;
    double coordinate[2];
    double potentialValue; // 该参数仅用于目标规划给出的买卖
    int targetIndex;
} Robot;

void Robot_init(Robot *robot, RobotID robotID, double x, double y);

void Robot_update(Robot *robot, double x, double y, int workbench,
                  int good,
                  double timeValue,
                  double knockValue,
                  double angelVelocity,
                  double linearVelocity[2],
                  double faceTo, int targetIndex);

#endif