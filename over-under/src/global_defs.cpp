#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "global_defs.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor topLeftDrive(STRAIGHT_LEFT_DRIVE);
pros::Motor midLeftDrive(REVERSE_LEFT_DRIVE1);
pros::Motor botLeftDrive(REVERSE_LEFT_DRIVE2);

pros::Motor topRightDrive(REVERSE_RIGHT_DRIVE);
pros::Motor midRightDrive(STRAIGHT_RIGHT_DRIVE1);
pros::Motor botRightDrive(STRAIGHT_RIGHT_DRIVE2);

pros::Motor intakeRight(RIGHT_INTAKE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor intakeLeft(LEFT_INTAKE, pros::E_MOTOR_GEAR_BLUE);
pros::Imu imu(IMU);

pros::Motor rightElevation(RIGHT_ELEVATION);
pros::Motor leftElevation(LEFT_ELEVATION);

pros::Motor leftFly(LEFT_FLYWHEEL);
pros::Motor rightFly(RIGHT_FLYWHEEL);
pros::Motor intakeActuation(INTAKE_ACTUATION);

pros::ADIDigitalOut eleLock(ELEVATION_LOCK);
pros::ADIDigitalOut flippers(FLIPPERS);

float wheel_radius=1.625;
float encoder_wheel_radius=2.75;

int blue_ticks_per_rev=300;
int green_ticks_per_rev=900;
int red_ticks_per_rev=1800;