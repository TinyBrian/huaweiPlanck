#include "main.h"

// #define debug_func_trade
#define LOCK 1
#define FORSELL 1
#define FORBUY 0

int money;
long currentFrame;
int k; // 工作台数
int a;
int robot_id;
Robot robots[4];

Workbench workbenchList[55];
SqLinkedList workbench9;
SqLinkedList aquireGood1;
SqLinkedList aquireGood2;
SqLinkedList aquireGood3;
SqLinkedList aquireGood4;
SqLinkedList aquireGood5;
SqLinkedList aquireGood6;
SqLinkedList aquireGood7;
SqLinkedList sellingGood1;
SqLinkedList sellingGood2;
SqLinkedList sellingGood3;
SqLinkedList sellingGood4;
SqLinkedList sellingGood5;
SqLinkedList sellingGood6;
SqLinkedList sellingGood7;

SqLinkedList *aquireList[7] = {&aquireGood1, &aquireGood2, &aquireGood3,
                               &aquireGood4, &aquireGood5, &aquireGood6,
                               &aquireGood7};

SqLinkedList *sellingList[7] = {&sellingGood1, &sellingGood2, &sellingGood3,
                                &sellingGood4, &sellingGood5, &sellingGood6,
                                &sellingGood7};

void inputProcess(char *l, double *t)
{
    int i = 1;
    char *p = strtok(l, " ");
    if (p)
    {
        t[0] = atof(p);
    }
    while (p = strtok(NULL, " "))
    {
        t[i++] = atof(p);
    }
}

bool readUntilOK()
{
    int i = 0; // use i to distingush lines
    char line[1024];
    double temp[10];

    while (fgets(line, sizeof line, stdin))
    {
        i++;
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
        inputProcess(line, temp);
        if (i == 1)
        {
            money = (int)temp[0];
#ifdef debug_attach
            fprintf(stderr, "money: %d\n", money);
#endif
        }
        else if (i == 2)
        {
            k = (int)temp[0];
#ifdef debug_attach
            fprintf(stderr, "k: %d\n", k);
#endif
        }
        else if (i > 2 && i < k + 3)
        {
            Workbench_update(i - 2, temp[0], temp[1], temp[2],
                             (int)temp[3], (int)temp[4], (int)temp[5]);
#ifdef debug_attach
            for (int i = 0; i < 6; i++)
                fprintf(stderr, ": %d ", temp[i]);
            fprintf(stderr, ": \n");
#endif
        }
        else if (i < k + 7)
        {
            double linearV[2] = {temp[5], temp[6]};
            Robot_update(&robots[i - k - 3], temp[8], temp[9],
                         (int)temp[0], (int)temp[1], temp[2],
                         temp[3], temp[4], linearV, temp[7], robots[i - k - 3].targetIndex);
#ifdef debug_attach
            for (int i = 0; i < 10; i++)
                fprintf(stderr, ": %d ", temp[i]);
            fprintf(stderr, ": \n");
#endif
        }
    }
    return false;
}

bool init()
{
    robot_id = 0;
    SqLinkedList_init(&workbench9);
    for (int i = 0; i < 7; i++)
    {
        SqLinkedList_init(aquireList[i]);
    }
    for (int i = 0; i < 7; i++)
    {
        SqLinkedList_init(sellingList[i]);
    }

    int numberOfWorkbench = 0;
    char line[1024];
    int j = 0; // the number of line in map
    while (fgets(line, sizeof line, stdin))
    {
        j++;
        for (int i = 1; i <= 100; i++)
        {

            if (line[i - 1] != '.' && line[i - 1] != 'A')
            {
                int type = (int)line[i - 1] - 48; // char '1' to int 1
                Workbench w = Workbench_init(type, numberOfWorkbench, 0, 0, 0, 0, 0);
                w.coordinate[0] = 0.5 * i - 0.25;
                w.coordinate[1] = 50.25 - 0.5 * j;
                w.locked = 0;
                w.lastProduceTime = -1;
                w.lure = 0;
                w.sourceSpaceStatus = 0;
                w.productSpaceStatus = 0;
                workbenchList[numberOfWorkbench++] = w;
                if (line[i - 1] == '9')
                {
                    SqLinkedList_add(&workbench9, w);
                }
            }

            if (line[i - 1] == 'A')
            {
                Robot_init(&robots[robot_id++], robot_id, 0.5 * i - 0.25, 50.25 - 0.5 * j);
            }
        }

        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
    }
    return false;
}

void buyAt(Robot robot, Workbench workbench, int *flag_mark)
{
    if (workbench.productSpaceStatus == 1)
    {
        *flag_mark |= 1 << (int)robot.robotID;
#ifdef debug_func_trade
        fprintf(stderr, "robot:%d buy good %d\n", robot.robotID, robot.good);
#endif
    }
}

void sellTo(Robot robot, Workbench workbench, int *flag_mark)
{
    if (((workbench.sourceSpaceStatus >> robot.good) & 1) == 0) // has space for sell
    {
        *flag_mark |= 1 << (int)robot.robotID;
#ifdef debug_func_trade
        fprintf(stderr, "robot:%d sell good %d\n", robot.robotID, robot.good);
#endif
    }
}

void goodDestory(Robot robot, int *flag_mark)
{
    if (robot.good != 0)
    {
        *flag_mark |= 1 << (int)robot.robotID;
#ifdef debug_func_trade
        fprintf(stderr, "robot:%d destroy good %d\n", robot.robotID, robot.good);
#endif
    }
}

double faceTo(double theta_target, double theta_robot)
{
    double angular_velocity = 0;
    // 控制机器人朝向随机点
    if (fabs(theta_target - theta_robot) > 0)
    {
        if (theta_target > theta_robot)
        {
            if (theta_target - theta_robot <= PI)
                angular_velocity = K_ANGULAR * (theta_target - theta_robot);
            else
                angular_velocity = -K_ANGULAR * (2 * PI - theta_target + theta_robot);
        }
        else
        {
            if (theta_robot - theta_target <= PI)
                angular_velocity = -K_ANGULAR * (theta_robot - theta_target);
            else
                angular_velocity = K_ANGULAR * (2 * PI - theta_robot + theta_target);
        }
        // theta_robot += angular_velocity;

        // // 限制角度在[-pi,pi]范围内
        // if (theta_robot > PI)
        //     theta_robot -= 2 * PI;
        // if (theta_robot < -PI)
        //     theta_robot += 2 * PI;
    }
    return angular_velocity;
}

void goTo(Workbench w, Robot robot, double v[])
{
    double theta_target = atan2(w.coordinate[1] - robot.coordinate[1], w.coordinate[0] - robot.coordinate[0]);
    double theta_robot = robot.faceTo;
    v[1] = faceTo(theta_target, theta_robot);
    // v[0] = getLinearVelocity(w.coordinate[0], robot.coordinate[0], w.coordinate[1], robot.coordinate[1], robot.faceTo);
    v[0] = getLinearVelocity2(robot, w);
    // fprintf(stderr, "goTo:linearV: %f angularV: %f\n", v[0], v[1]);
}

void getRobotTail(Robot r, double cor[2])
{

    // 计算圆心到后方点的距离
    double dx = -radius * sin(r.faceTo);
    double dy = radius * cos(r.faceTo);

    // 计算后方点的坐标
    cor[0] = r.coordinate[0] + dx;
    cor[1] = r.coordinate[1] + dy;
}

/**
 * @brief  r2 -> r1
 *
 * @param r1
 * @param r2
 * @param v
 */
void follow(Robot r1, Robot r2, double v[])
{
    double coor_tail[2];
    getRobotTail(r1, coor_tail);
    double theta_target = atan2(coor_tail[1] - r2.coordinate[1],
                                coor_tail[0] - r2.coordinate[0]);
    double theta_r2 = r2.faceTo;
    v[1] = faceTo(theta_target, theta_r2);
    // v[0] = getLinearVelocity(coor_tail[0], robot.coordinate[0], coor_tail[1], robot.coordinate[1], robot.faceTo);
    v[0] = getLinearVelocity(coor_tail[0], coor_tail[1],
                             r2.coordinate[0], r2.coordinate[1], r2.faceTo);
    // fprintf(stderr, "goTo:linearV: %f angularV: %f\n", v[0], v[1]);
}

double getBeamLength(double x0, double y0, double faceTo)
{
    // 计算射线方向向量
    double dx = 0.01 * cos(faceTo);
    double dy = 0.01 * sin(faceTo);

    // 迭代计算与边界相交的点
    double x = x0;
    double y = y0;
    while (x >= 0.0 && x <= 50.0 && y >= 0.0 && y <= 50.0)
    {
        // 前进一步
        x += dx;
        y += dy;
    }
    // 计算射线长度
    return sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
}

double getRTRBeamLength(Robot r1, Robot r2)
{
    // // 计算射线方向向量
    // double dx = 0.01 * cos(r1.faceTo);
    // double dy = 0.01 * sin(r2.faceTo);

    // // 迭代计算与r2相交的点
    // double x = r1.coordinate[0];
    // double y = r1.coordinate[1];

    // while (x >= 0.0 && x <= 50.0 && y >= 0.0 && y <= 50.0)
    // {
    //     // 前进一步
    //     x += dx;
    //     y += dy;
    // }
    // 计算射线长度
    return distance(r2.coordinate[0], r2.coordinate[1], r1.coordinate[0], r1.coordinate[1]);
}

/**
 * @brief Get the Linear Velocity object
 *
 * @param x1 target x
 * @param x2 robot x
 * @param y1 target y
 * @param y2 robot y
 * @param faceTo robot face agular
 * @return double
 */

double getLinearVelocity(double x1, double x2, double y1, double y2, double faceTo)
{
    double velocity = 0;
    double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    double beamLength = getBeamLength(x2, y2, faceTo);

    // fprintf(stderr, "beamdistance:%f\n", beamLength);

    if (beamLength > ROBOT_MAX_R + 4.5)
    {
        if (distance > 0)
        {
            velocity = K_DISTANCE * distance;
        }
    }
    else
        velocity = K_BEAM_DISTANCE * (beamLength - ROBOT_MAX_R - 0.25);
    return velocity; // 朝向小于0.01当作已经面朝
}

double getLinearVelocity2(Robot robot, Workbench w)
{
    double velocity = 0;
    double r_x = robot.coordinate[0];
    double w_x = w.coordinate[0];
    double r_y = robot.coordinate[1];
    double w_y = w.coordinate[1];
    double dis = distance(r_x, r_y, w_x, w_y);
    double beamLength = getBeamLength(r_x, r_y, robot.faceTo);
    double beam2 = beamLength;

    double rValue = getGoodValue(robot.good);
    for (int i = 0; i < 3; i++)
    {
        if (i == robot.robotID)
            continue;

        double otherRV = getGoodValue(robots[i].good);
        if (rValue < otherRV)
        {
            beam2 = getRTRBeamLength(robot, robots[i]);
        }
    }

    // beamLength = beam2 < beamLength ? beam2 : beamLength;

    if (beam2 > ROBOT_MAX_R + 4.5)
    {
        if (dis > 0)
        {
            velocity = K_DISTANCE * dis;
        }
    }
    else
        velocity = K_BEAM_DISTANCE * (beam2 - ROBOT_MAX_R - 0.25);

    return velocity; // 朝向小于0.01当作已经面朝
}

// #define DEBUG 1
// #define debug_attach

int main()
{
#ifdef DEBUG
    int y = 0;
    scanf("%d", &y);
    while (y == 0)
    {
        ;
    }
#endif
    init();
    puts("OK");
    fflush(stdout);
    while (scanf("%d", &currentFrame) != EOF)
    {
        double a[2] = {0};
        double b[2] = {0};
        double c[2] = {0};
        double d[2] = {0};
        readUntilOK();
        printf("%d\n", currentFrame);
        int flag_buy = 0;
        int flag_sell = 0;
        int flag_destory = 0;
        Workbench w1, w2, w3, w4;
        w1.index = 0;
        w2.index = 0;
        w3.index = 0;
        w4.index = 0;
        if (robots[0].good == 0)
        {
            w1 = getTargetForPurchase(robots[0]);
            goTo(w1, robots[0], a);
            if (robots[0].workbench == w1.index - 1)
            {

                buyAt(robots[0], workbenchList[w1.index], &flag_buy);
            }
        }
        else
        {
            w1 = getTargetForSell(robots[0], LOCK);
            goTo(w1, robots[0], a);
            if (robots[0].workbench == w1.index - 1)
            {
                sellTo(robots[0], workbenchList[w1.index], &flag_sell);
            }
        }

        if (robots[1].good == 0)
        {
            w2 = getTargetForPurchase(robots[1]);
            if (w2.index == w1.index && isGettingTooClose(robots[0], robots[1]))
                follow(robots[0], robots[1], b);
            else
                goTo(w2, robots[1], b);
            if (robots[1].workbench == w2.index - 1)
            {

                buyAt(robots[1], workbenchList[w2.index], &flag_buy);
            }
        }
        else
        {
            w2 = getTargetForSell(robots[1], LOCK);
            if (w2.index == w1.index && isGettingTooClose(robots[0], robots[1]))
                follow(robots[0], robots[1], b);
            else
                goTo(w2, robots[1], b);
            if (robots[1].workbench == w2.index - 1)
            {
                sellTo(robots[1], workbenchList[w2.index], &flag_sell);
            }
        }

        if (robots[2].good == 0)
        {
            w3 = getTargetForPurchase(robots[2]);
            if (w3.index == w2.index && isGettingTooClose(robots[1], robots[2]))
                follow(robots[1], robots[2], b);
            else
                goTo(w3, robots[2], c);
            if (robots[2].workbench == w3.index - 1)
            {

                buyAt(robots[2], workbenchList[w3.index], &flag_buy);
            }
        }
        else
        {
            w3 = getTargetForSell(robots[2], LOCK);
            if (w3.index == w2.index && isGettingTooClose(robots[1], robots[2]))
                follow(robots[1], robots[2], b);
            else
                goTo(w3, robots[2], c);
            if (robots[2].workbench == w3.index - 1)
            {
                sellTo(robots[2], workbenchList[w3.index], &flag_sell);
            }
        }

        if (robots[3].good == 0)
        {
            w4 = getTargetForPurchase(robots[3]);
            if (w4.index == w3.index && isGettingTooClose(robots[3], robots[2]))
                follow(robots[2], robots[3], d);
            else
                goTo(w4, robots[3], d);
            if (robots[3].workbench == w4.index - 1)
            {

                buyAt(robots[3], workbenchList[w4.index], &flag_buy);
            }
        }
        else
        {
            w4 = getTargetForSell(robots[3], LOCK);
            if (w4.index == w3.index && isGettingTooClose(robots[3], robots[2]))
                follow(robots[2], robots[3], d);
            else
                goTo(w4, robots[3], d);
            if (robots[3].workbench == w4.index - 1)
            {
                sellTo(robots[3], workbenchList[w4.index], &flag_sell);
            }
        }

        printf("forward %d %f\n", 0, a[0]);
        printf("rotate %d %f\n", 0, a[1]);
        printf("forward %d %f\n", 1, b[0]);
        printf("rotate %d %f\n", 1, b[1]);
        printf("forward %d %f\n", 2, c[0]);
        printf("rotate %d %f\n", 2, c[1]);
        printf("forward %d %f\n", 3, d[0]);
        printf("rotate %d %f\n", 3, d[1]);
#ifdef debug_func_trade
        fprintf(stderr, "flag_bug %d \n", flag_buy);
#endif
        for (int a = 3; a >= 0; a--)
        {
            if ((flag_buy >> a) & 1 == 1)
            {
                printf("buy %d \n", a);
#ifdef debug_func_trade
                fprintf(stderr, "buy %d \n", a);
#endif
                continue;
            }
            if ((flag_sell >> a) & 1 == 1)
            {
                printf("sell %d \n", a);
                continue;
            }
            if ((flag_destory >> a) & 1 == 1)
                printf("destory %d \n", a);
        }

        // fprintf(stderr, "linearV: %f angularV: %f\n", c[0], c[1]);
        printf("OK\n", currentFrame);
        fprintf(stderr, "currentFrame: %d\n", currentFrame);
        fflush(stdout);
    }
    return 0;
}

int Workbench_update(int index, int type,
                     double x, double y, int lastProduceTime,
                     int sourceSpaceStatus, int productSpaceStatus)
{
    Workbench w;
    w.index = index;
    w.type = type;
    w.coordinate[0] = x;
    w.coordinate[1] = y;
    w.lastProduceTime = lastProduceTime;
    w.productSpaceStatus = productSpaceStatus;
    w.sourceSpaceStatus = sourceSpaceStatus;
    w.locked = 0;
    w.lure = 0;

    if (type == 4)
    {
        if (((sourceSpaceStatus >> 1) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood1, w))
            {
                SqLinkedList_add(&aquireGood1, w);
            }
        }
        else
        {
            if (SqLinkedList_contains(aquireGood1, w))
            {
                Workbench_delete(&aquireGood1, w);
            }
            w.lure += getGoodValue(1);
        }

        if (((sourceSpaceStatus >> 2) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood2, w))
                SqLinkedList_add(&aquireGood2, w);
        }
        else
        {
            if (SqLinkedList_contains(aquireGood2, w))
            {
                Workbench_delete(&aquireGood2, w);
            }
            w.lure += getGoodValue(2);
        }
    }
    if (type == 5)
    {
        if ((((sourceSpaceStatus >> 1) & 1)) == 0)
        {
            if (!SqLinkedList_contains(aquireGood1, w))
                SqLinkedList_add(&aquireGood1, w);
        }
        else
        {

            if (SqLinkedList_contains(aquireGood1, w))
            {
                Workbench_delete(&aquireGood1, w);
            }
            w.lure += getGoodValue(1);
        }

        if (((sourceSpaceStatus >> 3) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood3, w))
                SqLinkedList_add(&aquireGood3, w);
        }
        else
        {

            if (SqLinkedList_contains(aquireGood3, w))
            {
                Workbench_delete(&aquireGood3, w);
            }
            w.lure += getGoodValue(3);
        }
    }
    if (type == 6)
    {
        if (((sourceSpaceStatus >> 2) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood2, w))
                SqLinkedList_add(&aquireGood2, w);
        }
        else
        {
            if (SqLinkedList_contains(aquireGood2, w))
            {
                Workbench_delete(&aquireGood2, w);
            }
            w.lure += getGoodValue(2);
        }

        if (((sourceSpaceStatus >> 3) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood3, w))
                SqLinkedList_add(&aquireGood3, w);
        }
        else
        {

            if (SqLinkedList_contains(aquireGood3, w))
            {
                Workbench_delete(&aquireGood3, w);
            }
            w.lure += getGoodValue(3);
        }
    }
    if (type == 7)
    {
        if (((sourceSpaceStatus >> 4) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood4, w))
                SqLinkedList_add(&aquireGood4, w);
        }
        else
        {

            if (SqLinkedList_contains(aquireGood4, w))
            {
                Workbench_delete(&aquireGood4, w);
            }
            w.lure += getGoodValue(4);
        }

        if (((sourceSpaceStatus >> 5) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood5, w))
                SqLinkedList_add(&aquireGood5, w);
        }
        else
        {

            if (SqLinkedList_contains(aquireGood5, w))
            {
                Workbench_delete(&aquireGood5, w);
            }
            w.lure += getGoodValue(5);
        }

        if (((sourceSpaceStatus >> 6) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood6, w))
                SqLinkedList_add(&aquireGood6, w);
        }
        else
        {

            if (SqLinkedList_contains(aquireGood6, w))
            {
                Workbench_delete(&aquireGood6, w);
            }
            w.lure += getGoodValue(6);
        }
    }
    if (type == 8)
    {
        if (((sourceSpaceStatus >> 7) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood7, w))
                SqLinkedList_add(&aquireGood7, w);
            // w.lure += 10;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood7, w))
                Workbench_delete(&aquireGood7, w);
        }
    }

    if (type == 9)
    {
        if (((sourceSpaceStatus >> 1) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood1, w))
                SqLinkedList_add(&aquireGood1, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood1, w))
                Workbench_delete(&aquireGood1, w);
        }
        if (((sourceSpaceStatus >> 2) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood2, w))
                SqLinkedList_add(&aquireGood2, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood2, w))
                Workbench_delete(&aquireGood2, w);
        }

        if (((sourceSpaceStatus >> 3) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood3, w))
                SqLinkedList_add(&aquireGood3, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood3, w))
                Workbench_delete(&aquireGood3, w);
        }

        if (((sourceSpaceStatus >> 4) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood4, w))
                SqLinkedList_add(&aquireGood4, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood4, w))
                Workbench_delete(&aquireGood4, w);
        }
        if (((sourceSpaceStatus >> 5) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood5, w))
                SqLinkedList_add(&aquireGood5, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood5, w))
                Workbench_delete(&aquireGood5, w);
        }
        if (((sourceSpaceStatus >> 6) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood6, w))
                SqLinkedList_add(&aquireGood6, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood6, w))
                Workbench_delete(&aquireGood6, w);
        }

        if (((sourceSpaceStatus >> 7) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood7, w))
                SqLinkedList_add(&aquireGood7, w);
            // w.lure++;
        }
        else
        {

            if (SqLinkedList_contains(aquireGood7, w))
                Workbench_delete(&aquireGood7, w);
        }

        // for (int i = 0; i < 7; i++)
        // {
        //     if (((sourceSpaceStatus >> (i + 1)) & 1) == 0)
        //     {
        //         if (!SqLinkedList_contains(*aquireList[i], w))
        //             SqLinkedList_add(aquireList[i], w);
        //     }
        //     else
        //     {

        //         if (SqLinkedList_contains(*aquireList[i], w))
        //             Workbench_delete(aquireList[i], w);
        //     }
        // }
    }
    if (type != 8 && type != 9)
    {
        // fprintf(stderr, "type:%d  productSpaceStatus:%d \n", type, productSpaceStatus);
        if (productSpaceStatus == 1)
        {
            if (!SqLinkedList_contains(*sellingList[type - 1], w))
                SqLinkedList_add(sellingList[type - 1], w);
        }
        else if (productSpaceStatus == 0)
        {

            if (lastProduceTime <= 50 && lastProduceTime >= 0)
            {
                // fprintf(stderr, "type:%d  lastProduceTime:%d \n", type, lastProduceTime);
                if (!SqLinkedList_contains(*sellingList[type - 1], w))
                    SqLinkedList_add(sellingList[type - 1], w);
            }
            else
            {

                if (SqLinkedList_contains(*sellingList[type - 1], w))
                    Workbench_delete(sellingList[type - 1], w);
            }
        }
    }
    workbenchList[index] = w;
}
double distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
double estDistance(double x1, double y1, double x2, double y2)
{
    return abs(x1 - x2) + abs(y1 - y2); // 减少运算量
}

#define TOO_CLOSE_DIS 1.5
int isGettingTooClose(Robot r1, Robot r2)
{
    return distance(r1.coordinate[0], r1.coordinate[1], r2.coordinate[0], r2.coordinate[1]) <= TOO_CLOSE_DIS;
}

Workbench getTargetForSell(Robot robot, int lock)
{
    int good = robot.good;
    double currentX = robot.coordinate[0];
    double currentY = robot.coordinate[1];
    Workbench w;
    node *p = NULL;
    double maxPotentialValue = -100000000;
    node *c = NULL; // point to minDistance node
    for (p = (*aquireList[good - 1])->next; p != NULL; p = p->next)
    {
        double Hx = getSellToPotentialValue(good, currentX, currentY, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    if (c != NULL)
    {
        if (lock)
        {
            // c->data.locked = 1;
            // workbenchList[c->data.index].locked = 1;
            // robots[robot.robotID].targetIndex = c->data.index;
            if ((int)robot.robotID >= 0)
                fprintf(stderr, "robot :%d targeting to sellTo type==%d \n", robot.robotID, c->data.type);
        }
        return c->data;
    }
    else
    {
        w.type = 0;
        fprintf(stderr, "err robot :%d targeting to sellTo type==fail \n", robot.robotID);
        return w;
    }
    // else
    // {
    //     c = workbench9->next;
    //     fprintf(stderr, "robot :%d buyfrom c==NULL\n", robot.robotID);
    // }
}
/**
 * @brief Get the Potenial Value object
 *
 * @param r_good the good that robot currently carring
 * @param w_type the type of the target selling to
 * @return int
 */
int getPotenialValue(int r_good, int w_type)
{
    switch (r_good)
    {
    case 1:
        if (w_type == 4)
            return 7100;
        if (w_type == 5)
            return 7800;
        if (w_type == 9)
            return 3000;
        break;
    case 2:
        if (w_type == 4)
            return 7100;
        if (w_type == 6)
            return 8300;
        if (w_type == 9)
            return 3200;
        break;
    case 3:
        if (w_type == 5)
            return 7800;
        if (w_type == 6)
            return 8300;
        if (w_type == 9)
            return 3400;
        break;
    case 4:
        if (w_type == 7)
            return 29000;
        if (w_type == 9)
            return 7100;
        break;
    case 5:
        if (w_type == 7)
            return 29000;
        if (w_type == 9)
            return 7800;
        break;
    case 6:
        if (w_type == 7)
            return 29000;
        if (w_type == 9)
            return 8300;
        break;
    case 7:
        if (w_type == 8)
            return 29000;
        if (w_type == 9)
            return 29000;
        break;
    }
}

int getGoodValue(int good)
{
    switch (good)
    {
    case 0:
        return 0;
        break;
    case 1:
        return 6000;
        break;
    case 2:

        return 7600;
        break;
    case 3:

        return 9200;
        break;
    case 4:

        return 22500;
        break;
    case 5:

        return 25000;
        break;
    case 6:

        return 27500;
        break;
    case 7:
        return 105000;
        break;
    }
}

int getGoodPrice(int good)
{
    switch (good)
    {
    case 1:
        return 3000;
        break;
    case 2:

        return 4400;
        break;
    case 3:

        return 5800;
        break;
    case 4:

        return 15400;
        break;
    case 5:

        return 17200;
        break;
    case 6:

        return 19200;
        break;
    case 7:
        return 76000;
        break;
    }
}
/*-----------------------------------------------------------------------------------------------*/

double getSellToPotentialValue(int good, double r_x, double r_y, Workbench w)
{
    int sellable = 1;
    double balance = 100;
    int numberOfSourceSlat = SqLinkedList_length(*aquireList[good - 1]);
    int sameGoodOnOtherRobot = 0;
    for (int i = 0; i < 3; i++)
    {
        if (robots[i].good == good)
            sameGoodOnOtherRobot++;
        // if (workbenchList[robots[i].targetIndex].type == good)
        //     sameGoodOnOtherRobot++;
    }
    if (numberOfSourceSlat - sameGoodOnOtherRobot <= 0)
    {
        sellable = 0;
    }
    int i = SqLinkedList_length(workbench9);
    double dis = estDistance(r_x, r_y, w.coordinate[0], w.coordinate[1]);
    double Value = getGoodValue(good);
    if (w.type >= 4 && w.type <= 7)
        balance = SqLinkedList_length(*sellingList[w.type - 1]);

    if (i != 0)
        balance = 20;

    return sellable * K_VALUE * Value - K_DIS * dis + sellable * K_LURE * w.lure + sellable * K_BALACNE * (20 - balance);
}

double getPurchaseFromPotentialValue(double r_x, double r_y, Workbench w)
{
    int sellable = 1;
    double p_dis = estDistance(r_x, r_y, w.coordinate[0], w.coordinate[1]);
    Robot r;
    r.robotID = -1;
    r.good = w.type;
    r.coordinate[0] = r_x;
    r.coordinate[1] = r_y;
    Workbench planToSell = getTargetForSell(r, 0); // 仅为计划用不能上锁
    if (planToSell.type == 0)
    {
        sellable = 0;
    }
    double s_p = getSellToPotentialValue(w.type, r_x, r_y, planToSell);
    double s_v = getGoodPrice(w.type);
    // int sameGoodOnOtherRobot = 0;
    // for (int i = 0; i < 3; i++)
    // {
    //     if (robots[i].good == w.type)
    //         sameGoodOnOtherRobot++;
    //     if (workbenchList[robots[i].targetIndex].type == w.type)
    //         sameGoodOnOtherRobot++;
    // }
    // int numberOfVacant = SqLinkedList_length(*aquireList[w.type - 1]);
    // if (numberOfVacant - sameGoodOnOtherRobot <= 1)
    // {
    //     int sellable = 0;
    // }
    double pp = sellable * K_SELL * s_p - K_DIS * p_dis - K_VALUE * s_v;
    return pp;
}

double getPurchaseFromPotentialValue2(Robot robot, Workbench w_planToBuy)
{
    double x_plan = w_planToBuy.coordinate[0];
    double y_plan = w_planToBuy.coordinate[1];
    double buy_dis = estDistance(robot.coordinate[0], robot.coordinate[1], x_plan, y_plan);
    double s_v = 0; // 购买商品a成本
    double s_p = 0; // 变现a收益
    Robot r;
    r.robotID = -1;
    r.good = w_planToBuy.type;
    r.coordinate[0] = x_plan;
    r.coordinate[1] = y_plan;
    Workbench planToSell = getTargetForSell(r, 0); // 仅为计划用不能上锁
    s_p = getSellToPotentialValue(w_planToBuy.type, x_plan, y_plan, planToSell);
    s_v = getGoodPrice(w_planToBuy.type);
    double pp = K_SELL * s_p - K_DIS * buy_dis - K_VALUE * s_v;
    return pp;
}

/*-----------------------------------------------------------------------------------------------*/

Workbench getTargetForPurchase(Robot robot)
{
    if (currentFrame >= 8800) // 最后3秒不再买东西
    {
        return Workbench_init(0, 0, 0, 0, 0, 0, 0);
    }
    double currentX = robot.coordinate[0];
    double currentY = robot.coordinate[1];
    Workbench w;
    node *p = NULL;
    double maxPotentialValue = -10000000;
    node *c = NULL; // point to minDistance node

    for (p = (*sellingList[6])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    if (c != NULL)
    {
        c->data.locked = 1;
        workbenchList[c->data.index].locked = 1;
        // robots[robot.robotID].targetIndex = c->data.index;
        fprintf(stderr, "robot :%d targeting to buyfrom type==%d \n", robot.robotID, c->data.type);
        return c->data;
    }
    for (p = (*sellingList[5])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    if (c != NULL)
    {
        c->data.locked = 1;
        workbenchList[c->data.index].locked = 1;
        // robots[robot.robotID].targetIndex = c->data.index;
        fprintf(stderr, "robot :%d targeting to buyfrom type==%d \n", robot.robotID, c->data.type);
        int a = SqLinkedList_length(workbench9);
        if (a > 0)
            return c->data;
        else
        {
            int numberOfSourceSlat = SqLinkedList_length(*aquireList[5]);
            int sameGoodOnOtherRobot = 0;
            for (int i = 0; i < 3; i++)
            {
                if (robots[i].good == 6)
                    sameGoodOnOtherRobot++;
            }
            if (numberOfSourceSlat - sameGoodOnOtherRobot >= 1)
            {
                return c->data;
            }
        }
    }
    for (p = (*sellingList[4])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    for (p = (*sellingList[3])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    for (p = (*sellingList[2])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    for (p = (*sellingList[1])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }
    for (p = (*sellingList[0])->next; p != NULL; p = p->next)
    {
        // double Hx = getPurchaseFromPotentialValue(currentX, currentY, p->data);
        double Hx = getPurchaseFromPotentialValue2(robot, workbenchList[p->data.index]);
        if (Hx > maxPotentialValue)
        {
            if (!workbenchList[p->data.index].locked)
            {
                c = p;
                maxPotentialValue = Hx;
                robot.potentialValue = maxPotentialValue;
            }
        }
    }

    if (c != NULL)
    {
        c->data.locked = 1;
        workbenchList[c->data.index].locked = 1;
        // robots[robot.robotID].targetIndex = c->data.index;
        fprintf(stderr, "robot :%d targeting to buyfrom type==%d \n", robot.robotID, c->data.type);
        return c->data;
    }
    else
    {
        w.index = 0;
        node n;
        n.data = w;
        c = &n;
        // return c->data;
        fprintf(stderr, "err: robot :%d targeting to buyfrom c==NULL maxPotentialV:%d \n", robot.robotID, maxPotentialValue);
    }
}
