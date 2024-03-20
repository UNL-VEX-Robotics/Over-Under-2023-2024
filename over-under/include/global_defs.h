#include "auton.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

#define TOP_LEFT_DRIVE 8
#define MID_LEFT_DRIVE -20
#define BOT_LEFT_DRIVE -7

#define TOP_RIGHT_DRIVE -19
#define MID_RIGHT_DRIVE 3
#define BOT_RIGHT_DRIVE 17

#define RIGHT_INTAKE 2
#define LEFT_INTAKE 13

#define IMU 16

#define RIGHT_ELEVATION -10
#define LEFT_ELEVATION 4

#define FLYWHEEL_L 6
#define FLYWHEEL_R 11

#define INTAKE_PNEU 'B'
#define ELEVATION_LOCK 'C'
#define FLIPPERS 'F'

extern pros::Controller master;

extern pros::Motor topLeftDrive;
extern pros::Motor midLeftDrive;
extern pros::Motor botLeftDrive;

extern pros::Motor midRightDrive;
extern pros::Motor botRightDrive;
extern pros::Motor topRightDrive;

extern pros::Motor intakeRight;
extern pros::Motor intakeLeft;
extern pros::Imu imu;

extern pros::Motor rightElevation;
extern pros::Motor leftElevation;

extern pros::Motor leftFly;
extern pros::Motor rightFly;

extern pros::ADIDigitalOut flippers;
extern pros::ADIDigitalOut eleLock;
extern pros::ADIDigitalOut intakePneu;
extern pros::ADIDigitalOut eleLock;

extern float wheel_radius;
extern float encoder_wheel_radius;

extern int blue_ticks_per_rev;
extern int green_ticks_per_rev;
extern int red_ticks_per_rev;
