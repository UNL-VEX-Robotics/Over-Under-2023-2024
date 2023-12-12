// Test Bot Ports
// const int left_front = 13;
// const int left_back = 16;
// const int right_front = 17;
// const int right_back = 20;
// const int radius = 4;

#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

#define TOP_LEFT_DRIVE 17 
#define MID_LEFT_DRIVE 5 
#define BOT_LEFT_DRIVE 2

#define TOP_RIGHT_DRIVE 11
#define MID_RIGHT_DRIVE 15
#define BOT_RIGHT_DRIVE 20

#define LEFT_INTAKE 7
#define RIGHT_INTAKE 10 

#define EVEVATION 8 

#define LEFT_CAT 3 
#define RIGHT_CAT 14

#define IMU 12

#define LIMIT_SWITCH 'A'
#define INTAKE 'G'
#define FLIPPERS 'H'

#define IMU 1

const float radius = 1.625;

const int blue_ticks_per_rev = 300;
const int green_ticks_per_rev = 900;
const int red_ticks_per_rev = 1800;

double x_offset;
double y_offset;
double angle_offset;
int shooting = 0;
pros::Controller Controller1(pros::E_CONTROLLER_MASTER);
pros::Controller Controller2(pros::E_CONTROLLER_PARTNER);
pros::Motor intake_motor(10);
pros::Motor cata(21);
pros::Distance cata_stop(19);
pros::ADIDigitalOut piston_boost(7);

extern double x_offset;
extern double y_offset;
extern double angle_offset;
extern int shooting;
extern pros::Controller Controller1;
extern pros::Controller Controller2;
extern pros::Motor intake_motor;
extern pros::Motor cata;
extern pros::Distance cata_stop;
extern pros::ADIDigitalOut piston_boost;


void shoot_loop();