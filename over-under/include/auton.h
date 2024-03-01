#include "pid.hpp"

void turn(double degrees, PID turnPID);
void go(double inches, PID leftStraightPid, PID rightStraightPid);
double convert(double degrees);

void reset_positions();
void set_all_voltage(int voltage);
void set_left_voltage(float voltage);
void set_right_voltage(float voltage);

void turn_right_relative_debug(double degrees, PID turnPID);
void turn_left_relative_debug(double degress, PID turnPID);
