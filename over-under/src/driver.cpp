#include "main.h"
#include "auton.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "global_defs.h"

//Function For Drive Code: Sticks
void moveDrive(){
	
	//Arcade Drive
	int drive = (1.00 * master.get_analog(ANALOG_LEFT_Y));
	int turn = (1.00 * master.get_analog(ANALOG_RIGHT_X));

	topRightDrive = drive - turn;
	midRightDrive = drive - turn;
	botRightDrive = drive - turn;

	topLeftDrive = drive + turn;
	midLeftDrive = drive + turn;
	botLeftDrive = drive + turn;
	
}


//Code for Elevation Button: X for up,  B for down
void elevate(){
	if (master.get_digital(DIGITAL_UP)){ //Comes out of storage
		rightElevation = -100;
		leftElevation = -100;
	} else if(master.get_digital(DIGITAL_DOWN)){ //Climbs
		rightElevation = 100;
		leftElevation = 100;
	} else{
		rightElevation = 0;
		leftElevation = 0;
	}
}

bool intakeSpinningForward = false;
bool intakeSpinningReverse = false;
void intake_func(){
    if(master.get_digital_new_press(DIGITAL_L2)){ //on spin-forward-button pressed
        intakeSpinningForward = !intakeSpinningForward;
    }
	if(master.get_digital_new_press(DIGITAL_L1)){ //on spin-reverse-button pressed
        intakeSpinningReverse = !intakeSpinningReverse;
    }
    if (intakeSpinningForward && intakeSpinningReverse){ // handles the case where both forward and backward are true.
		//This isn't entirely necessary, but without it the button presses won't feel as intuitive.
		intakeSpinningForward = false; 
		intakeSpinningReverse = false;
    }
	//set intake motor speed to 127 if in forward mode, -127 if in reverse mode, and 0 if both false or both true
	intakeRight = 127 * intakeSpinningForward + -127 * intakeSpinningReverse;
	intakeLeft = -127 * intakeSpinningForward + 127 * intakeSpinningReverse;
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
		intakeActuation.move_relative(red_ticks_per_rev / 2.0, 150);
	}
}
//Elevation Lock Activation Buttons: Left to Deploy and Pull Back
bool elevationToggle = false;
void activateElevation(){
	if(master.get_digital_new_press(DIGITAL_LEFT)){
		elevationToggle = !elevationToggle;
		eleLock.set_value(elevationToggle);
		pros::delay(300);
	}
}

//Flywheel set to be always running unless turned off Button: X On, B Off
bool isFlyOn = true;
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
		intakeActuation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	}
	else{
		topLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		midLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		botLeftDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		topRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		midRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		botRightDrive.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	}
}