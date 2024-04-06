#include "main.h"
#include "auton.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "routes.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <string>
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
	pros::lcd::set_text(1, "CAMEL CASE IS BETTER");
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);
	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::set_text(2, "MATCH AUTON ON");
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
	full_skills_route_part1();
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
	double flywheel_percent = 0.82;
	master.clear();
	while (true) {
		if (master.get_digital_new_press(DIGITAL_UP)){
			master.print(0, 0, "Volt:%8.2f",  pros::battery::get_capacity());
			pros::delay(50);
			master.print(1, 0, "RF:%8.2f",  flywheel_percent);
			pros::delay(50);
			master.print(2, 0, "LF:%8.2f", flywheel_percent);
			flywheel_percent+=0.01;
		}
		if (master.get_digital_new_press(DIGITAL_DOWN)){
			master.print(0, 0, "Volt:%8.2f",  pros::battery::get_capacity());
			pros::delay(50);
			master.print(1, 0, "RF:%8.2f",  flywheel_percent);
			pros::delay(50);
			master.print(2, 0, "LF:%8.2f", flywheel_percent);
			flywheel_percent-=0.01;
		}
		//Tank Drive Code Sticks
		moveDrive();

		//Intake Button: L1 for in, L2 for out
		intake_func();

		//90 is A
		activateIntake90();

		//180 is X
		activateIntake180();

		//45 is Y
		activateIntake45();

		//Flywheel On by default
		rightFlyun(flywheel_percent);

		//Elevation Activate Button: Right
		elevationActive();

		i++;
		pros::delay(2);
	}
}