#include "main.h"
#include "auton.h"
#include "driver.h"
#include "global_defs.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "routes.h"
#include <iostream>
#include <list>
#include <string>
#include <tuple>

bool skillsToggle = true;
std::list<std::tuple<std::function<void(PID a, PID b, PID c)>, std::string>>
    skills_routes;
std::list<std::tuple<std::function<void(PID a, PID b, PID c)>, std::string>>::
    iterator skills_iter = skills_routes.begin();
std::list<std::tuple<std::function<void()>, std::string>> match_routes;
std::list<std::tuple<std::function<void()>, std::string>>::iterator match_iter =
    match_routes.begin();

PID leftpid = PID(0,0,0,15,1000);
PID rightpid = PID(0,0,0,15,1000);
PID turnpid = PID(0,0,0,0.5,100);
int pid_iter = 0;
int lrt_iter = 0;

void route_counter_up() {
  if (skillsToggle) {
    if (++skills_iter == skills_routes.end()) {
      skills_iter = skills_routes.begin();
      ++skills_iter;
    }
  } else {
    if (++match_iter == match_routes.end()) {
      match_iter = match_routes.begin();
      ++match_iter;
    }
  }
  std::string route_name = (skillsToggle) ? std::get<1>(*skills_iter) : std::get<1>(*match_iter);
  pros::lcd::set_text(3, route_name);
  master.print(0,0,route_name.c_str());
}

void route_counter_down() {
  if (skillsToggle) {
    if (--skills_iter == skills_routes.begin()) {
      skills_iter = skills_routes.end();
      skills_iter--;
    }
  } else {
    if (--match_iter == match_routes.begin()) {
      match_iter = match_routes.end();
    }
  }
  std::string route_name = (skillsToggle) ? std::get<1>(*skills_iter) : std::get<1>(*match_iter);
  pros::lcd::set_text(3, route_name);
  master.print(0,0,route_name.c_str());
}


void skills_toggle() {
  skillsToggle = !skillsToggle;
  skills_iter = skills_routes.begin();
  ++skills_iter;
  match_iter = match_routes.begin();
  ++match_iter;
  std::string ba = "";
  if (skillsToggle) {
    ba = "SKILLS AUTON ON";
  } else {
    ba = "MATCH AUTON ON";
  }
  pros::lcd::set_text(2, ba);
}

void scroll_routes(){
  if(master.get_digital_new_press(DIGITAL_X)){
    route_counter_up();
  }
  if(master.get_digital_new_press(DIGITAL_B)){
    route_counter_down();
  }
}

void scroll_pid_selection(){
  if(master.get_digital_new_press(DIGITAL_RIGHT)){
    lrt_iter++ ;
    lrt_iter %= 3;
    switch (lrt_iter){
      case 0:
        master.print(1,0,"Left");
      case 1:
        master.print(1,0,"Rite");
      case 2:
        master.print(1,0,"Turn");
    }
  }
  if(master.get_digital_new_press(DIGITAL_LEFT)){
    pid_iter++;
    pid_iter %= 3;
    switch (pid_iter){
      case 0:
        master.print(1,5,"P");
      case 1:
        master.print(1,5,"I");
      case 2:
        master.print(1,5,"D");
    }
  }
}


void incr_decr_pid_vals() {
  double p_incr = 0.01;
  double i_incr = 0.01;
  double d_incr = 0.1;

  PID pid_to_change = PID(0,0,0,0,0);
  double *value_to_change; 
  double incr = 0;
  switch(lrt_iter){
    case 0:
      pid_to_change = leftpid; 
    case 1:
      pid_to_change = rightpid; 
    case 2:
      pid_to_change = turnpid; 
  }
  switch(pid_iter){
    case 0:
      value_to_change = &pid_to_change.P_weight;
      incr = p_incr;
    case 1:
      value_to_change = &pid_to_change.I_weight;
      incr = i_incr;
    case 2:
      value_to_change = &pid_to_change.D_weight;
      incr = d_incr;
  }
  if(master.get_digital_new_press(DIGITAL_UP)){
    *value_to_change += incr;
  }
  if(master.get_digital(DIGITAL_DOWN)){
    *value_to_change -= incr;
  }
}

void display_pid_values(){
  
}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void dummy(PID a, PID b, PID c) { return; }
void initialize() {
  skills_routes.push_back(std::make_tuple(dummy, "DUMMY"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part1, "full_skills_1"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part2, "full_skills_2"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part3, "full_skills_3"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part4, "full_skills_4"));
  match_routes.push_back(
      std::make_tuple(match_drew, "match_drew"));
  match_routes.push_back(
      std::make_tuple(match_drew, "match_drew2_lol"));
//DONT DELETE OR MOVE THIS COMMENT SRSLY
  ++skills_iter;
  ++skills_iter;
  ++match_iter;
  pros::lcd::initialize();
  pros::lcd::set_text(1, "lists populated");
  pros::lcd::register_btn1_cb(route_counter_down);
  pros::lcd::register_btn2_cb(route_counter_up);
  pros::lcd::set_text(3, (skillsToggle) ? std::get<1>(*skills_iter) : std::get<1>(*match_iter));
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
  std::get<0>(*skills_iter)(leftpid, rightpid, turnpid);
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
  double flywheel_percent = 0.80;
//	std::string x = std::to_string(std::get<0>(*pid_iter).P_weight);
  while (true) {
    
    scroll_routes();
    scroll_pid_selection();

    // Tank Drive Code Sticks
    moveDrive();

    // Elevation Button: X for up,  B for down
    elevate();

    // Intake Button: L1 for in, L2 for out
    intake_func();

    // Flippers Button: R2
    activateFlippers();

	// intake up or down: A
    activateIntake90();
	// intake up and down at once: B
    activateIntake180();

    // Flywheel On by default
    rightFlyun(flywheel_percent);

    // Elevation Lock on Button: Left
    activateElevation();

    // Wheel Braking set to Button: Right
    wheelsBrake();

    pros::delay(2);
  }
}