#include "pid.h"
void shoot(int num);
void intake_in(int time);
void intake_out(int time);
void flywheel_in(int time);
void flywheel_out(int time);
double relative_math(double inches);
void moveRelative(double inches, double velo = 100);
void activateIntake180GO();
void intakeUpDown();
void match_drew(PID left, PID right, PID turnP);
void test_route(PID left, PID right, PID turn);
void match_drew_MONEY(PID leftPID, PID rightPID, PID turnPID);
void drew_AWP(PID leftPID, PID rightPID, PID turnPID);
void get_over_and_score(PID leftPID, PID rightPID, PID turnPID);
void drew_touch_elevation(PID leftPID, PID rightPID, PID turnPID);
void drew_AWP_gen(PID leftPID, PID rightPID, PID turnPID);