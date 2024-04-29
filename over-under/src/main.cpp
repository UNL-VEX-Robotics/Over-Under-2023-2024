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

PID leftpid = PID(.08,0.11,0,15,25);
PID rightpid = PID(.08,0.11,0,15,25);
PID turnpid = PID(0.446,0.07,0,1,17); // <- was 0.43, 0.07, lim: 15 when we showed up
int start_voltage=50;
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
  //skills_routes.push_back(std::make_tuple(match_tanner, "match_tanner"));
  //skills_routes.push_back(
  //    std::make_tuple(match_drew_MONEY, "match_drew_MONEY"));
  //skills_routes.push_back(std::make_tuple(test_route, "test_route"));
  skills_routes.push_back(
      std::make_tuple(tanner_touch_elevation, "tanner_touch_elevation"));
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
  leftFly.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightFly.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  match_tanner(start_voltage, leftpid, rightpid, turnpid);
}

//runs in its own task
void opcontrol() {
  master.clear();
  double flywheel_percent = 0.82;
  leftFly.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightFly.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  flippers.set_value(0);
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

    //Activiate Flaps
    activateFlippers();

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

    wheelsBrake();

    pros::delay(2);
  }}