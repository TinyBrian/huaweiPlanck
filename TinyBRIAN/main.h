#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Robot.h"
#include "Workbench.h"
typedef struct w Workbench;
typedef struct r Robot;

// #define DEBUG
#define MAX_LENGTH_OF_OPERATORS_PER_FRAME 20
#define MAX_FORWORD_SPEED 6
#define MAX_BACKWARD_SPEED -2
#define MIN_OPEATE_DISTANCE 0.4
#define MAX_ROTATE_SPEED PI
#ifndef PI
#define PI 3.1415926
#endif
#define ROBOT_MASS_N 0.6361725015 // 机器人空载质量
#define ROBOT_MASS_L 0.8824733613 // 机器人负载质量
#define ROBOT_MIN_R 0.45
#define ROBOT_MAX_R 0.53

#define K_FOR7 0
#define K_DIS 70

#define K_VALUE 1
#define K_SELL 1
#define K_LURE 10
#define K_BALACNE 15000

#define LOCK 1
#define FORSELL 1
#define FORBUY 0

#define K_ANGULAR 10 // 角加速度系数

#define radius 2 // 机器人尾巴点半径

#define K_DISTANCE 2.7      // 机器人速度距离系数
#define K_BEAM_DISTANCE 1.1 // 机器人速度墙壁距离系数
#define TOO_CLOSE_DIS 1.5   // 判定机器人过近系数

double faceTo(double, double);

double getLinearVelocity(double x1, double x2, double y1, double y2, double faceTo);
double getLinearVelocity2(Robot robot, Workbench w);
void goTo(Workbench w, Robot robot, double v[]);
double faceTo(double theta_target, double theta_robot);
bool init();
bool readUntilOK();
void inputProcess(char *l, double *t);

double distance(double x1, double y1, double x2, double y2);
double estDistance(double x1, double y1, double x2, double y2);

int Workbench_update(int index, int type,
                     double x, double y, int lastProduceTime,
                     int sourceSpaceStatus, int productSpaceStatus);

Workbench getTargetForSell(Robot robot, int lock);
Workbench getTargetForPurchase(Robot robot);
int getPotenialValue(int r_good, int w_type);
double getSellToPotentialValue(int good, double r_x, double r_y, Workbench w);
double getPurchaseFromPotentialValue(double r_x, double r_y, Workbench w);
double getPurchaseFromPotentialValue2(Robot r, Workbench w);

int getGoodValue(int good);
int getGoodPrice(int good);
int isGettingTooClose(Robot r1, Robot r2);

#endif