#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "global_defs.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor topLeftDrive(TOP_LEFT_DRIVE);
pros::Motor midLeftDrive(MID_LEFT_DRIVE);
pros::Motor botLeftDrive(BOT_LEFT_DRIVE);

pros::Motor topRightDrive(TOP_RIGHT_DRIVE);
pros::Motor midRightDrive(MID_RIGHT_DRIVE);
pros::Motor botRightDrive(BOT_RIGHT_DRIVE);

pros::Motor intakeRight(RIGHT_INTAKE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor intakeLeft(LEFT_INTAKE, pros::E_MOTOR_GEAR_BLUE);
pros::Imu imu(IMU);

pros::Motor rightElevation(RIGHT_ELEVATION);
pros::Motor leftElevation(LEFT_ELEVATION);

pros::Motor leftFly(FLYWHEELL);
pros::Motor rightFly(FLYWHEELR);

pros::ADIDigitalOut intakePneu(INTAKE_PNEU);
pros::ADIDigitalOut eleLock(ELEVATION_LOCK);
pros::ADIDigitalOut flippers(FLIPPERS);

float wheel_radius=1.625;
float encoder_wheel_radius=2.75;

int blue_ticks_per_rev=300;
int green_ticks_per_rev=900;
int red_ticks_per_rev=1800;