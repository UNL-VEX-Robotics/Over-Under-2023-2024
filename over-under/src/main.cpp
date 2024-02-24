#include "main.h"
#include "auton.h"
#include "api.h"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <string>
#include "global_defs.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2); 
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "mac gets bitches");
	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor topLeftDrive(TOP_LEFT_DRIVE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor midLeftDrive(MID_LEFT_DRIVE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor botLeftDrive(BOT_LEFT_DRIVE, pros::E_MOTOR_GEAR_BLUE);

pros::Motor topRightDrive(TOP_RIGHT_DRIVE);
pros::Motor midRightDrive(MID_RIGHT_DRIVE);
pros::Motor botRightDrive(BOT_RIGHT_DRIVE);

pros::Motor intake(INTAKE, pros::E_MOTOR_GEAR_BLUE);

pros::Motor rightElevation(RIGHT_EVEVATION, pros::E_MOTOR_GEAR_RED, true);
pros::Motor leftElevation(LEFT_ELEVATION);

pros::Motor flywheel(FLYWHEEL);

pros::ADIDigitalOut intakePneu(INTAKE_PNEU);
pros::ADIDigitalOut eleLock(ELEVATION_LOCK);
pros::ADIDigitalOut flippers(FLIPPERS);

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
int convert(int degrees){
	return degrees + 23;
}


void autonomous() {
	

}

void leftButton(){
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::clear();
	}
}

void rightButton(){
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

//Function For Drive Code: Sticks
void moveDrive(){
	
	//Arcade Drive
	int left = (0.75 * master.get_analog(ANALOG_LEFT_Y));
	int right = (0.75 * master.get_analog(ANALOG_RIGHT_Y));

	topRightDrive = -right;
	midRightDrive = -right;
	botRightDrive = right;

	topLeftDrive = -left;
	midLeftDrive = -left;
	botLeftDrive = left;
	
}


//Code for Elevation Button: X for up,  B for down
void elevate(){
	if (master.get_digital(DIGITAL_L1)){ //Comes out of storage
		rightElevation = 100;
		leftElevation = 100;
	} else if(master.get_digital(DIGITAL_L2)){ //Climbs
		rightElevation = -100;
		leftElevation = -100;
	} else{
		rightElevation = 0;
		leftElevation = 0;
	}
}

bool isOnFor = false;
bool isOnRev = false;
void intake_func(){
     if(master.get_digital_new_press(DIGITAL_R1)){
        isOnFor = !isOnFor;
        isOnRev = false;
    }else if(master.get_digital_new_press(DIGITAL_R2)){
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

//Old Code for the Intake keep incase Tanner wants it like this
/*
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
			intake = 127;
		}
		else if(!direction){
			intake = -127;
		}
	}
	else{
		intake.brake();
	}
}
*/

//Flippers Buttons: R2 to Deploy and Pull Back
bool flipperToggle = false;
void actiavteFlippers(){
	if(master.get_digital_new_press(DIGITAL_Y)){
		flipperToggle = !flipperToggle;
		flippers.set_value(flipperToggle);
		pros::delay(300);
	}
}

//Intake Activation Buttons: R1 to Deploy and Pull Back
bool intakeToggle = false;
void activateIntake(){
	if(master.get_digital_new_press(DIGITAL_A)){
		intakeToggle = !intakeToggle;
		intakePneu.set_value(intakeToggle);
		pros::delay(300);
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
		isFlyOn = true;
	}
	if(master.get_digital_new_press(DIGITAL_B)){
		isFlyOn = false;
	}
	if(isFlyOn){
		flywheel = 127;
	}
	else if(!isFlyOn){
		flywheel = 0;
	}
}


void opcontrol() {
	master.clear();
	rightElevation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	leftElevation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	int i = 0;
	while (true) {
		//Tank Drive Code Sticks
		moveDrive();

		//Elevation Button: X for up,  B for down
		elevate();

		//Intake Button: L1 for in, L2 for out
		intake_func();

		//Flippers Button: R2
		actiavteFlippers();

		//Intake Activation Button: R1
		activateIntake();

		//Flywheel On by default
		flywheelRun();

		//Elevation Lock on Button: Left
		activateElevation();

		pros::delay(2);
	}
}