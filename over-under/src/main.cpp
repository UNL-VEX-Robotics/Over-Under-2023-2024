#include "main.h"
#include "auton.h"
#include "routes.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "global_defs.h"
#include "driver.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
int autonSelector = 0;
bool skillsAuton = false;


void on_center_button() { //Count Up
	autonSelector --; 
	if(autonSelector == -1){
		autonSelector = 3;
	}
	std::string ba = "" + std::to_string(autonSelector);
	pros::lcd::set_text(3, ba);
}

void on_right_button() {
	autonSelector ++; 
	if(autonSelector == 4){
		autonSelector = 0;
	}
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
	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn2_cb(on_right_button);
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
	match_drew();
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
		activateFlippers();

    activateIntake180();

		//Run Flywheel Button: Y
		flywheelRun();

		//Elevation Activation Button: Left
		activateElevation();

		//Wheel set to brake mode hold and allowing it to change with Button: RIght
		wheelsBrake();

		pros::delay(2);
	}
}