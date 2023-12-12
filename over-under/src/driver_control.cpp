#include "main.h"
#include "global_defs.h"

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
	int drive = master.get_analog(ANALOG_LEFT_Y);
	int turn = master.get_analog(ANALOG_LEFT_X);

	tr = drive - turn;
	mr = drive - turn;
	br = drive - turn;

	tl = drive + turn;
	ml = drive + turn;
	bl = drive + turn;	
}

//Function For Moving the Catapult: Button: L1

void moveCat(){
	
	if (master.get_digital(DIGITAL_L1)){
		leftCat = -100;
		rightCat = -100;
	} else if(limit.get_value()){
		leftCat = -100;
		rightCat = -100;
	} else {
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
	} else {
		elevation = 0;
	}
}

//Intake Button: R1 for in, R2 for out
void intake(){
	if(master.get_digital(DIGITAL_R1)){
		leftIntake = 100;
		rightIntake = 100;
		
	} else if (master.get_digital(DIGITAL_R2)){
		leftIntake = -100;
		rightIntake = -100;
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

void driver_control(){
  //Sets the brake type of the evevation motor
		//elevation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

		//Tank Drive Code Sticks
		moveDrive(topLeftDrive, midLeftDrive, botLeftDrive, topRightDrive, midRightDrive, botRightDrive);

		//Catapult Code Button: L1
		moveCat();

		//Elevation Button: X for up,  B for down
		elevate();

		//Intake Button: R1 for in, R2 for out
		intake();

		//Flippers Button: A
		actiavteFlippers();

		//Intake Activation Button: Y
		activateIntake();
		
		pros::delay(2);
}