#include "main.h"
#include "auton.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "global_defs_drew.h"
#include "driver.h"

void moveDriveDrew(){
	//Arcade Drive
	int drive = master.get_analog(ANALOG_LEFT_Y);
	int turn = master.get_analog(ANALOG_RIGHT_X);

	topRightDrive = drive - turn;
	midRightDrive = drive - turn;
	botRightDrive = drive - turn;

	topLeftDrive = drive + turn;
	midLeftDrive = drive + turn;
	botLeftDrive = drive + turn;
}




bool direction = true; //True = Forward, False = Reverse
bool isOn = false;
void intake_func(){
	if(master.get_digital_new_press(DIGITAL_L2)){
		direction = !direction;
	}
	if(master.get_digital_new_press(DIGITAL_L1)){
		isOn = !isOn;
	}
	if(isOn){
		if(direction){
			intakeRight = 127;
			intakeLeft = 127;
		}
		else if(!direction){
			intakeRight = -127;
			intakeLeft = -127;
		}
	}
	else{
		intakeRight.brake();
		intakeLeft.brake();
	}
}


//Flippers Buttons: R2 to Deploy and Pull Back
bool flipperToggle = false;
void activateFlippers(){
	if(master.get_digital_new_press(DIGITAL_R2)){
		flipperToggle = !flipperToggle;
		flippers.set_value(flipperToggle);
		pros::delay(300);
	}
}


void activateIntake180(){
	if(master.get_digital_new_press(DIGITAL_R1)){
		intakeActuation.move_relative(red_ticks_per_rev / 2.0, 65);
	}
}

bool eleToggle = false;
void activateElevation(){
	if(master.get_digital_new_press(DIGITAL_LEFT)){
		eleToggle = !eleToggle;
		elevation.set_value(eleToggle);
		pros::delay(300);
	}
}

//Flywheel set to run unless Y Button is pressed
bool isFlyOn = true; //Auto set to true so the flywheel starts on
void flywheelRun(){
	if(master.get_digital_new_press(DIGITAL_X)){
		isFlyOn = !isFlyOn;
	}
	if(isFlyOn){
		rightFly = 127;
		leftFly = 127;
	}
	else {
		rightFly = 0;
		leftFly = 0;
	}
}

//Toggle to Set Wheels at Brake Type hold
bool isWheelsBrake = true;
void wheelsBrake(){
	if(master.get_digital_new_press(DIGITAL_RIGHT)){
		isWheelsBrake = !isWheelsBrake;
	}
	if(isWheelsBrake){
		topLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		midLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		botLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		topRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		midRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		botRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	}
	else if(!isWheelsBrake){
		topLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		midLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		botLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		topRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		midRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		botRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	}
}
