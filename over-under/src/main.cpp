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
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <sys/select.h>
#include <tuple>

bool skillsToggle = true;
std::list<std::tuple<std::function<void(PID a, PID b, PID c)>, std::string>>
    skills_routes;
std::list<std::tuple<std::function<void(PID a, PID b, PID c)>, std::string>>::
    iterator skills_iter = skills_routes.begin();
std::list<std::tuple<std::function<void()>, std::string>> match_routes;
std::list<std::tuple<std::function<void()>, std::string>>::iterator match_iter =
    match_routes.begin();

PID leftpid = PID(.08,0.01,0,15,1000);
PID rightpid = PID(.08,0.01,0,15,1000);
PID turnpid = PID(0.55,0,0.005,0.5,100);

PID *selected_pid = &leftpid;
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
  master.clear_line(0);
  pros::delay(50);
  std::cout << route_name.c_str();
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
  master.clear_line(0);
  pros::delay(50);
  std::cout << route_name.c_str();
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
  if(master.get_digital_new_press(DIGITAL_R1)){
    route_counter_up();
  }
  if(master.get_digital_new_press(DIGITAL_R2)){
    route_counter_down();
  }
}

void display_p_value(PID *pid){
  double p = pid->P_weight;
  master.print(2,0,"P %.5f", p);
}
void display_i_value(PID *pid){
  double i = pid->I_weight;
  master.print(2,0,"I %.5f", i);
}
void display_d_value(PID *pid){
  double d = pid->D_weight;
  master.print(2,0,"D %.5f", d);
}
void display_all_values(PID *pid){
  display_p_value(pid);
  pros::delay(50);
  display_i_value(pid);
  pros::delay(50);
  display_d_value(pid);
  pros::delay(50);
}

void scroll_pid_selection(){
  if(master.get_digital_new_press(DIGITAL_RIGHT)){
    lrt_iter++ ;
    lrt_iter %= 2;
    switch (lrt_iter){
      case 0:
        master.print(1,0,"Straight");
        selected_pid = &leftpid;
        break;
      case 1:
        master.print(1,0,"Turn");
        selected_pid = &turnpid;
        break;
    }
    switch (pid_iter){
      case 0:
        pros::delay(50);
        display_p_value(selected_pid);
        break;
      case 1:
        pros::delay(50);
        display_i_value(selected_pid);
        break;
      case 2:
        pros::delay(50);
        display_d_value(selected_pid);
        break;
    }
  }
  if(master.get_digital_new_press(DIGITAL_LEFT)){
    pid_iter ++;
    pid_iter %= 3;
	switch (pid_iter){
		case 0:
			pros::delay(50);
			display_p_value(selected_pid);
			break;
		case 1:
			pros::delay(50);
			display_i_value(selected_pid);
			break;
		case 2:
			pros::delay(50);
			display_d_value(selected_pid);
			break;
		}
  }
}




void incr_decr_pid_vals() {
  double p_incr = 0.01;
  double i_incr = 0.01;
  double d_incr = 0.1;

  if(master.get_digital_new_press(DIGITAL_UP)){
    switch(pid_iter){
      case 0:
	  	if(selected_pid==&leftpid){
			rightpid.P_weight += p_incr;
		}
        selected_pid->P_weight += p_incr;
        display_p_value(selected_pid);
        break;
      case 1:
	  	if(selected_pid==&leftpid){
			rightpid.I_weight += i_incr;
		}
        selected_pid->I_weight += i_incr;
        display_i_value(selected_pid);
        break;
      case 2:
	  	if(selected_pid==&leftpid){
			rightpid.D_weight += d_incr;
		}
        selected_pid->D_weight += d_incr;
        display_d_value(selected_pid);
        break;
    }
  }
  if(master.get_digital_new_press(DIGITAL_DOWN)){
    switch(pid_iter){
      case 0:
	  	if(selected_pid==&leftpid){
			rightpid.P_weight -= p_incr;
		}
        selected_pid->P_weight -= p_incr;
        display_p_value(selected_pid);
        break;
      case 1:
	  	if(selected_pid==&leftpid){
			rightpid.I_weight -= i_incr;
		}
        selected_pid->I_weight -= i_incr;
        display_i_value(selected_pid);
        break;
      case 2:
	  	if(selected_pid==&leftpid){
			rightpid.D_weight -= d_incr;
		}
        selected_pid->D_weight -= d_incr;
        display_d_value(selected_pid);
        break;
    }}}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void dummy(PID a, PID b, PID c) { return; }
void initialize() {
  master.clear();
  pros::delay(50);
  skills_routes.push_back(std::make_tuple(dummy, "DUMMY"));
  skills_routes.push_back(std::make_tuple(test_route, "test_route"));
  skills_routes.push_back(
      std::make_tuple(match_drew_MONEY, "match_drew_MONEY"));
  //DONT DELETE OR MOVE THIS COMMENT SRSLY
  ++skills_iter;
  ++skills_iter;
  ++match_iter;
  ++match_iter;
  pros::lcd::initialize();
  pros::lcd::set_text(1, "its over...");
  pros::lcd::register_btn1_cb(route_counter_down);
  pros::lcd::register_btn2_cb(route_counter_up);
  std::string route_name = (skillsToggle) ? std::get<1>(*skills_iter) : std::get<1>(*match_iter);
  pros::lcd::set_text(3, route_name);
  master.print(0, 0, route_name.c_str());
}

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
 */
void autonomous() {
	std::get<0>(*skills_iter)(leftpid, rightpid, turnpid);
}

//runs in its own task
void opcontrol() {
  master.clear();
  display_d_value(selected_pid);
  rightElevation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  leftElevation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  double flywheel_percent = 0.80;

  while (true) {
    
    scroll_routes();
    scroll_pid_selection();
    incr_decr_pid_vals();

    moveDrive();

    // Elevation Button: X for up,  B for down
    elevate();

    // Intake Button: L1 for in, L2 for out
    intake_func();

    // Flippers Button: R2
    activateFlippers();

    activateIntake180();

    // Elevation Lock on Button: Left
    activateElevation();

    // Wheel Braking set to Button: Right
    wheelsBrake();

    pros::delay(2);
  }
}