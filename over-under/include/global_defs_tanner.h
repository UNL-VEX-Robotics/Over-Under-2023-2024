#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

#define STRAIGHT_LEFT_DRIVE 8
#define REVERSE_LEFT_DRIVE1 -20
#define REVERSE_LEFT_DRIVE2 -7

#define REVERSE_RIGHT_DRIVE -19
#define STRAIGHT_RIGHT_DRIVE1 3
#define STRAIGHT_RIGHT_DRIVE2 17

#define RIGHT_INTAKE 2
#define LEFT_INTAKE 13

#define IMU 16

#define RIGHT_ELEVATION -10
#define LEFT_ELEVATION 4

#define RIGHT_FLYWHEEL 11
#define LEFT_FLYWHEEL 6

#define INTAKE_ACTUATION 5

#define FLIPPERS 'F'

#define ELEVATION_LOCK 'V'

#define ELEVATION1 'B'



extern pros::controller_analog_e_t intakeButton;
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

extern pros::Motor intakeActuation;

extern pros::ADIDigitalOut flippers;
extern pros::ADIDigitalOut eleLock;
extern pros::ADIDigitalOut intakePneu;

extern pros::ADIDigitalOut elevation1;

extern float wheel_radius;
extern float encoder_wheel_radius;

extern int blue_ticks_per_rev;
extern int green_ticks_per_rev;
extern int red_ticks_per_rev;