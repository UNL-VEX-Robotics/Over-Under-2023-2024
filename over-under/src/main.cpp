#include "main.h"
#include "auton.h"
#include "api.h"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/misc.h"
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
	while(true){
		//pros::delay(4000);
		//move_distance_individual_sides(36.0, .16);
		pros::Imu imu(IMU);
		while(imu.is_calibrating()){
			pros::delay(20);
		}
		std::cout << "\nSO IT BEGINS\n";
		turn_right_relative(180, 1.1);
		std::cout << "end reached\n";
		turn_left_relative(180,1.1);
		std::cout << "end reached\n";
		turn_right_relative(90,1.1);
		std::cout << "end reached\n";
		turn_left_relative(90,1.1);
		pros::delay(1000000);
	}
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


//Creating the Motors and Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor topLeftDrive(TOP_LEFT_DRIVE, pros::E_MOTOR_GEAR_BLUE, true);
pros::Motor midLeftDrive(MID_LEFT_DRIVE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor botLeftDrive(BOT_LEFT_DRIVE, pros::E_MOTOR_GEAR_BLUE, true);

pros::Motor topRightDrive(TOP_RIGHT_DRIVE);
pros::Motor midRightDrive(MID_RIGHT_DRIVE, true);
pros::Motor botRightDrive(BOT_RIGHT_DRIVE);

pros::Motor leftIntake(LEFT_INTAKE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor rightIntake(RIGHT_INTAKE, pros::E_MOTOR_GEAR_BLUE, true);

pros::Motor elevation(EVEVATION, pros::E_MOTOR_GEAR_RED, true);

pros::Motor leftCat(LEFT_CAT, pros::E_MOTOR_GEAR_RED, true);
pros::Motor rightCat(RIGHT_CAT, pros::E_MOTOR_GEAR_RED);

pros::ADIAnalogIn limit(LIMIT_SWITCH);
pros::ADIDigitalOut triBallIntake(INTAKE);
pros::ADIDigitalOut flippers(FLIPPERS);

//Function For Drive Code: Sticks
void moveDrive(pros::Motor tl, pros::Motor ml, pros::Motor bl, pros::Motor tr, pros::Motor mr, pros::Motor br){
	
	//Tank Drive
	int leftDrive = master.get_analog(ANALOG_LEFT_Y);
	int rightDrive = master.get_analog(ANALOG_RIGHT_Y);

	tr = rightDrive;
	mr = rightDrive;
	br = rightDrive;

	tl = leftDrive;
	ml = leftDrive;
	bl = leftDrive;
	
}

//Function For Moving the Catapult: Button: L1

void moveCat(){
	
	if (master.get_digital(DIGITAL_L1)){
		leftCat = -100;
		rightCat = -100;
	}else if(limit.get_value()){
		leftCat = -100;
		rightCat = -100;
	} else{
		leftCat = 0;
		rightCat = 0;
	}
}

//Code for Elevation Button: X for up,  B for down
void elevate(){
	if (master.get_digital(DIGITAL_X)){ //Comes out of storage
		elevation = 100;
	} else if(master.get_digital(DIGITAL_B)){ //Climbs
		elevation = -100;
	} else{
		elevation = 0;
	}
}

//Intake Button: R1 for in, R2 for out
void intake(){
	if(master.get_digital(DIGITAL_R1)){
		leftIntake = 100;
		rightIntake = 100;
		
	}else if(master.get_digital(DIGITAL_R2)){
		leftIntake = -100;
		rightIntake = -100;
	}else{
		leftIntake = 0;
		rightIntake = 0;
	}
}

//Flippers Buttons: Y to Deploy and Pull Back
bool flipperToggle = false;
void actiavteFlippers(){
	if(master.get_digital_new_press(DIGITAL_A)){
		flipperToggle = !flipperToggle;
		flippers.set_value(flipperToggle);
		pros::delay(300);
	}
}

//Intake Activation Buttons: A to Deploy and Pull Back
bool intakeToggle = false;
void activateIntake(){
	if(master.get_digital_new_press(DIGITAL_Y)){
		intakeToggle = !intakeToggle;
		triBallIntake.set_value(intakeToggle);
		pros::delay(300);
	}
}

void opcontrol() {

	master.clear();
	while (true) {
		//Sets the brake type of the evevation motor
		//elevation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

		//Tank Drive Code Sticks
		moveDrive(topLeftDrive, midLeftDrive, botLeftDrive, topRightDrive, midRightDrive, botRightDrive);

		//Catapult Code Button: L1
		//moveCat();

		//Elevation Button: X for up,  B for down
		//elevate();

		//Intake Button: R1 for in, R2 for out
		//intake();

		//Flippers Button: A
		//actiavteFlippers();

		//Intake Activation Button: Y
		//activateIntake();
		
		pros::delay(2);
	}
}