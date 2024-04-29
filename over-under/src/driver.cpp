#include "main.h"
#include <iostream>
#include "auton.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "global_defs.h"
#include "driver.h"

//Function For Drive Code: Two Stick Arcade
void moveDrive(){
	
	//Arcade Drive
	int drive = master.get_analog(ANALOG_LEFT_Y);
	int turn = master.get_analog(ANALOG_RIGHT_X);

	topRightDrive = drive - turn;
	midRightDrive = drive - turn;
	botRightDrive = drive - turn;
//
	topLeftDrive = drive + turn;
	midLeftDrive = drive + turn;
	botLeftDrive = drive + turn;
	
}


//Old Code for the Intake keep incase Drew wants it like this
/*
bool isOnFor = false;
bool isOnRev = false;
void intake_func(){
     if(master.get_digital_new_press(DIGITAL_L1)){
        isOnFor = !isOnFor;
        isOnRev = false;
    }else if(master.get_digital_new_press(DIGITAL_L2)){
        isOnFor = false;
        isOnRev = !isOnRev;
    }
    if (isOnFor){
        intake = 127;
    }
    if (isOnRev){
        intake = -127;
    }
    if(!isOnFor && !isOnRev){
        intake = 0;
    }
}
*/
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
		if(flipperToggle){
			flippers.set_value(1);
		} else {
			flippers.set_value(0);
		}
		pros::delay(300);
	}
}
bool armToggle = false;
void activateArms(){
	if(master.get_digital_new_press(DIGITAL_RIGHT)|| master.get_digital_new_press(DIGITAL_LEFT)){
		armToggle = !armToggle;	
		if(armToggle){
			arm.set_value(1);
		} else {
			arm.set_value(0);
		}
		pros::delay(300);
	}
}



void activateIntake180(){
	if(master.get_digital_new_press(DIGITAL_R1)){
		intakeActuation.move_relative(red_ticks_per_rev / 2.0, 65);
	}
}


//Flywheel set to run unless Y Button is pressed
bool isFlyOn = false; //Auto set to true so the flywheel starts on
void flywheelRun(float percent){
	if(master.get_digital_new_press(DIGITAL_X)){
		isFlyOn = !isFlyOn;
	}
	if(isFlyOn){
		float volt = percent * 127;
		rightFly = volt;
		leftFly = volt;
	}
	else {
		rightFly = 0;
		leftFly = 0;
	}
}
