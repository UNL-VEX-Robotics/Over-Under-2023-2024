#include "auton.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

#define STRAIGHT_LEFT_DRIVE 16
#define REVERSE_LEFT_DRIVE1 -8
#define REVERSE_LEFT_DRIVE2 -7

#define REVERSE_RIGHT_DRIVE -4
#define STRAIGHT_RIGHT_DRIVE1 3
#define STRAIGHT_RIGHT_DRIVE2 5

#define RIGHT_INTAKE -19
#define LEFT_INTAKE 14 

#define RIGHT_ELEVATION -2
#define LEFT_ELEVATION 6

#define RIGHT_FLYWHEEL 1
#define LEFT_FLYWHEEL 18

#define INTAKE_PNEU 'B'
#define FLIPPERS 'D'
#define ELEVATION_LOCK 'A'

#define IMU 20

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
extern pros::Motor intakeActuation;
extern pros::ADIDigitalOut eleLock;

extern float wheel_radius;
extern float encoder_wheel_radius;

extern int blue_ticks_per_rev;
extern int green_ticks_per_rev;
extern int red_ticks_per_rev;