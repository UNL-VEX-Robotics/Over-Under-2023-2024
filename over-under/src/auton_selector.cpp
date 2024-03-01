#include "main.h"
#include "auton.h"
#include "routes.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "global_defs.h"
#include "driver.h"
#include "auton_selector.h"

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

void run_auton(){
	if(autonSelector == 0){
		full_skills_route_part1();
	}
	else if(autonSelector == 1){
		full_skills_route_part2();
	}
	else if(autonSelector == 2){
		full_skills_route_part3();
	}
	else if(autonSelector == 3){
		full_skills_route_part4();
	}
}