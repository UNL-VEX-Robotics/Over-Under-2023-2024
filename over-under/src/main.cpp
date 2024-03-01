#include "main.h"
#include "auton.h"
#include "driver.h"
#include "global_defs.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "routes.h"
#include <iostream>
#include <list>
#include <string>
#include <tuple>

int autonSelector = 0;
bool skillsToggle = true;
std::list<std::tuple<std::function<void(PID a, PID b, PID c)>, std::string>>
    skills_routes;
std::list<std::tuple<std::function<void(PID a, PID b, PID c)>, std::string>>::
    iterator skills_iter = skills_routes.begin();
std::list<std::tuple<std::function<void()>, std::string>> match_routes;
std::list<std::tuple<std::function<void()>, std::string>>::iterator match_iter =
    match_routes.begin();
std::list<std::tuple<PID, PID, PID, std::string>> pids;
std::list<std::tuple<PID, PID, PID, std::string>>::iterator pid_iter =
    pids.begin();

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
  pros::lcd::set_text(3, (skillsToggle) ? std::get<1>(*skills_iter)
                                        : std::get<1>(*match_iter));
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
  pros::lcd::set_text(3, (skillsToggle) ? std::get<1>(*skills_iter)
                                        : std::get<1>(*match_iter));
}

void pid_counter_up() {
  if (++pid_iter == pids.end()) {
    pid_iter = pids.begin();
  }
  std::string ba = std::get<3>(*pid_iter);
  pros::lcd::set_text(2, "PID: " + ba);
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
  std::tuple<PID, PID, PID, std::string> classicPID = std::make_tuple(
      PID(0.225, 0.01, 0, 15, 1000), PID(0.225, 0.01, 0, 15, 1000),
      PID(1.1, 0.00001, 0, 1, 14), "classic");
  std::tuple<PID, PID, PID, std::string> highStraightPID =
      std::make_tuple(PID(0.3, 0.01, 0, 15, 1000), PID(0.3, 0.01, 0, 15, 1000),
                      PID(1.1, 0.001, 0, 1, 14), "high Straight P");
  std::tuple<PID, PID, PID, std::string> highTurnPID = std::make_tuple(
      PID(0.225, 0.01, 0, 15, 1000), PID(0.225, 0.01, 0, 15, 1000),
      PID(1.3, 0.001, 0, 1, 14), "high Turn P");
  std::tuple<PID, PID, PID, std::string> precisionPID = std::make_tuple(
      PID(0.225, 0.01, 0, 5, 1000), PID(0.225, 0.01, 0, 5, 1000),
      PID(1.1, 0.00001, 0, 1, 14), "precision");
  std::tuple<PID, PID, PID, std::string> zeroPID = std::make_tuple(
      PID(0, 0, 0, 5, 1000), PID(0, 0, 0, 5, 1000),
      PID(0, 0, 0, 1, 14), "zero");
  pids.push_back(classicPID);
  pids.push_back(highStraightPID);
  pids.push_back(highTurnPID);
  pids.push_back(precisionPID);
  pids.push_back(zeroPID);
  skills_routes.push_back(std::make_tuple(dummy, "DUMMY"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part1, "full_skills_part_1"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part2, "full_skills_part_2"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part3, "full_skills_part_3"));
  skills_routes.push_back(
      std::make_tuple(full_skills_route_part4, "full_skills_part_4"));
  ++skills_iter;
  ++match_iter;
  pros::lcd::initialize();
  pros::lcd::set_text(1, "WE RESPECT WOMEN");
  pros::lcd::register_btn0_cb(pid_counter_up);
  pros::lcd::register_btn1_cb(route_counter_down);
  pros::lcd::register_btn2_cb(route_counter_up);
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
  PID left = std::get<0>(*pid_iter);
  PID right = std::get<1>(*pid_iter);
  PID turn = std::get<2>(*pid_iter);
  std::get<0> (*skills_iter)(left, right, turn);
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
    // Tank Drive Code Sticks
    moveDrive();

    // Elevation Button: X for up,  B for down
    elevate();

    // Intake Button: L1 for in, L2 for out
    intake_func();

    // Flippers Button: R2
    activteFlippers();

    // Intake Activation Button: R1
    activateIntake();

    // Flywheel On by default
    rightFlyun();

    // Elevation Lock on Button: Left
    activateElevation();

    // Wheel Braking set to Button: Right
    wheelsBrake();

    pros::delay(2);
  }
}