#include "pid.h"
void shoot(int num);
void intake_in(int time);
void intake_out(int time);
void flywheel_in(int time);
void flywheel_out(int time);
void elevate_up(int time);
void elevate_down(int time);
void activateIntake180GO();
void intakeUpDown();
void match_drew(PID left, PID right, PID turnP);
void test_route(PID left, PID right, PID turn);
void match_drew_MONEY(PID leftPID, PID rightPID, PID turnPID);
void match_drew_MONEY_2(PID leftPID, PID rightPID, PID turnPID);