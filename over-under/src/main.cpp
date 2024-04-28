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

PID leftpid = PID(.09,0.03,0,15,50);
PID rightpid = PID(.075,0.03,0,15,50);
PID turnpid = PID(0.35,0.1,10,1,10);

PID fastleftpid = PID(.01,0.06,0,15,50);
PID fastrightpid = PID(.0833333,0.06,0,15,50);
PID fastturnpid = PID(0.40,0.1,10,1,20);

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
  intakeActuation.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  match_drew_MONEY(leftpid, rightpid, turnpid);
}

//runs in its own task
void opcontrol() {
  master.clear();
  double flywheel_percent = 0.80;

  flippers.set_value(0);
  arm.set_value(0);
  while (true) {
    moveDrive();
      
    // Intake Button: L1 for in, L2 for out
    intake_func();

    // Flippers Button: R2
    activateFlippers();
    flywheelRun(flywheel_percent);
    activateArms();
    activateIntake180();

    pros::delay(2);
  }
}