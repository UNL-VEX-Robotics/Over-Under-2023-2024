#include "main.h"
#include "auton.h"
#include "api.h"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "global_defs.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
int autonSelector = 0;
bool skillsAuton = false;


void on_center_button() { //Count Up
	autonSelector ++; 
	std::string ba = "" + std::to_string(autonSelector);
	pros::lcd::set_text(3, ba);
}

void on_right_button() {
	autonSelector --; 
	std::string ba = "" + std::to_string(autonSelector);
	pros::lcd::set_text(3, ba);
}

void on_left_button() {
	skillsAuton = !skillsAuton;
	std::string ba = "";
	if (skillsAuton){
		ba = "SKILLS AUTON ON";
	}
	else if (!skillsAuton){
		ba = "MATCH AUTON ON";
	}
	pros::lcd::set_text(2, ba);
}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "WE RESPECT WOMEN");
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

pros::Motor topLeftDrive(TOP_LEFT_DRIVE);
pros::Motor midLeftDrive(MID_LEFT_DRIVE);
pros::Motor botLeftDrive(BOT_LEFT_DRIVE);

pros::Motor topRightDrive(TOP_RIGHT_DRIVE);
pros::Motor midRightDrive(MID_RIGHT_DRIVE);
pros::Motor botRightDrive(BOT_RIGHT_DRIVE);

pros::Motor intake(INTAKE, pros::E_MOTOR_GEAR_BLUE);

pros::Motor rightElevation(RIGHT_ELEVATION);
pros::Motor leftElevation(LEFT_ELEVATION);

pros::Motor flyl(FLYWHEELL);
pros::Motor flyr(FLYWHEELR);
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

void autonomous() {
	route_match_drew();
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


//Code for Elevation Button: X for up,  B for down
void elevate(){
	if (master.get_digital(DIGITAL_UP)){ //Comes out of storage
		rightElevation = 100;
		leftElevation = 100;
	} else if(master.get_digital(DIGITAL_DOWN)){ //Climbs
		rightElevation = -100;
		leftElevation = -100;
	} else{
		rightElevation = 0;
		leftElevation = 0;
	}
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


//Flippers Buttons: R2 to Deploy and Pull Back
bool flipperToggle = false;
void actiavteFlippers(){
	if(master.get_digital_new_press(DIGITAL_R2)){
		flipperToggle = !flipperToggle;
		flippers.set_value(flipperToggle);
		pros::delay(300);
	}
}

//Intake Activation Buttons: R1 to Deploy and Pull Back
bool intakeToggle = false;
void activateIntake(){
	if(master.get_digital_new_press(DIGITAL_R1)){
		intakeToggle = !intakeToggle;
		intakePneu.set_value(intakeToggle);
		pros::delay(300);
	}
}

//Intake Activation Buttons: Left to Deploy and Pull Back
bool eleToggle = false;
void activateElevation(){
	if(master.get_digital_new_press(DIGITAL_LEFT)){
		eleToggle = !eleToggle;
		eleLock.set_value(eleToggle);
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
		flyl = 127;
	}
	else {
		flyl = 0;
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

		//Run Flywheel Button: Y
		flywheelRun();

		//Elevation Activation Button: Left
		activateElevation();

		pros::delay(2);
	}
}