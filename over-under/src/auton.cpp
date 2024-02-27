#include "main.h"
#include <chrono>
#include "global_defs.h"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "auton.h"
#include "pros/rtos.h"
#include <cmath>
#include <string>
#include <math.h>
#include <list>
#include <sys/_stdint.h>

using namespace pros;

const int circum = wheel_radius * 2 * M_PI;

Imu imu(IMU);
pros::ADIDigitalOut eleLock(ELEVATION_LOCK);

pros::Motor lf(TOP_LEFT_DRIVE);
pros::Motor lm(MID_LEFT_DRIVE);
pros::Motor lb(BOT_LEFT_DRIVE);

pros::Motor flyl(FLYWHEEL_L);
pros::Motor flyr(FLYWHEEL_R);
pros::Motor intk(INTAKE, pros::E_MOTOR_GEAR_BLUE);
pros::Motor rele(RIGHT_ELEVATION);
pros::Motor lele(LEFT_ELEVATION);
pros::ADIDigitalOut intkPNEU(INTAKE_PNEU);
pros::Motor rf(TOP_RIGHT_DRIVE);
pros::Motor rm(MID_RIGHT_DRIVE);
pros::Motor rb(BOT_RIGHT_DRIVE);

//straight params
int tick_margin = 15;
int integral_max_error_s= 1000;
//PID values for when we are moving straight
float Kps = 0.225;
float Kis = 0.01;
float Kds = 0;

//turning params
int degree_margin = 1;
int integral_max_error_t = 14;
//PID values for when we are turning
float Kpt = 1.1;
float Kit = 0.00001;
float Kdt = 0;

double convert(double degrees){
    return degrees + 23;
}

void reset_motors(){
    lf.set_zero_position(0);
    lm.set_zero_position(0);
    lb.set_zero_position(0);
    rf.set_zero_position(0);
    rm.set_zero_position(0);
    rb.set_zero_position(0);
    return;
}

void set_all_voltage(int voltage){
    lf = voltage;
    lm = voltage;
    lb = voltage;
    rf = voltage;
    rm = voltage;
    rb = voltage;
    return;
}

void set_left_voltage(float voltage){
    lf = voltage;
    lm = voltage;
    lb = voltage;
    return;
}

void set_right_voltage(float voltage){
    rf = voltage;
    rm = voltage;
    rb = voltage;
    return;
}

void all_brake(){
    lf.brake();
    lm.brake();
    lb.brake();
    rf.brake();
    rm.brake();
    rb.brake();
}

int convert(int degrees){
	return degrees + 23;
}

void shoot(int num){
    intk = 127;
    flyl = -127;
    for(int i=0; i < num; i++){
        intkPNEU.set_value(1);
        pros::delay(600);
        intkPNEU.set_value(0);
        pros::delay(1000);
    }
    intkPNEU.set_value(0);
    pros::delay(500);
}

double calculateDrivetrainVoltage(double& encoder_units, double error, double PID_Integral, double PID_Derivative){
    double prevError = error;
    error = encoder_units - (lf.get_position() + lm.get_position() + rb.get_position()) / 3.0; 

    //if each iteration of the loop doesn't take a uniform amount of time, derivative and integral calculations need
    //to consider time.
    PID_Derivative = error - prevError;
    if(abs(error) > integral_max_error_s){
        PID_Integral = integral_max_error_s;
    }
    else {
        PID_Integral += Kis * error;
    }
    double voltage = Kps * error + Kis * PID_Integral + Kds * PID_Derivative; //error is the proportional factor of the PID calculation
    return voltage;
}

double constrainVoltage(double voltage, int i){
    //keeps voltage <= 127, and gives a continuous ramping effect
    //TODO: make the ramping scale factor a variable t rather than just a magic number
    if(voltage > 127){
        double x = 127.0 * (i+100)/25000;
        return x > 127 ? 127 : x;
    }
    return voltage;
}

void push(double inches){
    double start = pros::millis();
    double now = pros::millis(); 
    double revolutions = inches / circum;
    double encoder_units = 10.0/6.0 * revolutions * blue_ticks_per_rev;
    reset_motors();
    int i = 0;
    double left_error = encoder_units;
    double right_error = encoder_units;
    double left_integral = 0;
    double right_integral = 0;
    double left_derivative = 0;
    double right_derivative = 0;
    double left_voltage = 0;
    double right_voltage = 0;
    while(( abs(left_error) > tick_margin ) && (abs(right_error) > tick_margin )){ 
        left_voltage = calculateDrivetrainVoltage(encoder_units, left_error, left_integral, left_derivative);
        right_voltage = calculateDrivetrainVoltage(encoder_units, right_error, right_integral, right_derivative);
        set_left_voltage(constrainVoltage(left_voltage, i));
        set_right_voltage(constrainVoltage(right_voltage, i));

        if(i % 5000 == 0){ 
            //TODO: will this act as intended if each iteration of the while loop takes longer than 1 millisecond? If it is shorter, it will take 10000 iterations to to the timeout
            now = pros::millis();
            if(now - start > 5*1000){  //after 5 seconds
                left_error = 0;
                right_error = 0;
                set_all_voltage(0);
            }
            std::cout << "\n   L: ";
            std::cout << left_error;
            std::cout << "   R: ";
            std::cout << right_error;
            std::cout <<"\n";
        }
        i++;

    }
}

void go(double inches){
    double revolutions = inches / circum;
    double encoder_units = 10.0/6.0 * revolutions * blue_ticks_per_rev;
    reset_motors();
    int i = 0;
    double left_error = encoder_units;
    double right_error = encoder_units;
    double left_integral = 0;
    double right_integral = 0;
    double left_derivative = 0;
    double right_derivative = 0;

    double left_voltage = 0;
    double right_voltage = 0;
    while(( abs(left_error) > tick_margin ) || (abs(right_error) > tick_margin )){ 
        left_voltage = calculateDrivetrainVoltage(encoder_units, left_error, left_integral, left_derivative);
        right_voltage = calculateDrivetrainVoltage(encoder_units, right_error, right_integral, right_derivative);
        set_left_voltage(constrainVoltage(left_voltage, i));
        set_right_voltage(constrainVoltage(right_voltage, i));
        if(i % 5000 == 0){
            std::cout << "\n   L: ";
            std::cout << left_error;
            std::cout << "   R: ";
            std::cout << right_error;
            std::cout <<"\n";
        }
        i++;
    }
}


//go but with loop control. Used to push into goal
//we know we're in a "loop", i.e. bot is against the bars of the goal 
//when standard deviation of previous x(10?) iterations' error drops below a certain threshold

void turn_right_relative_debug(double degrees){
    double end_heading = degrees + imu.get_heading();
    double error = degrees;
    double prev_error = 0;
    double integral = 0;
    double derivative = 0;
    double voltage = 0;
    int i = 0;
    if(end_heading >= 360){
        while(imu.get_heading() > 15){
            prev_error = error;
            error = end_heading - imu.get_heading();
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative;
            if(voltage > 127){
                set_left_voltage(127);
                set_right_voltage(-127);
            } else {
                set_left_voltage(voltage);
                set_right_voltage(-voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        std::cout << "\nflipped to 0\n";
        end_heading-=360;
        error = end_heading - imu.get_heading();
        int i = 0;
        while(abs(error) > degree_margin){
            prev_error = error;
            error = end_heading - imu.get_heading();
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative;
            if(voltage > 127){
                set_left_voltage(127);
                set_right_voltage(-127);
            } else {
                set_left_voltage(voltage);
                set_right_voltage(-voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    }
    while(abs(error) > degree_margin){
        prev_error = error;
        error = end_heading - imu.get_heading();
        if(abs(error) > integral_max_error_t){
            integral = 0;
        } else {
            integral = integral + error;
        }
        derivative = error - prev_error;
        voltage = Kpt * error + Kit * integral + Kdt * derivative;
        if(voltage > 127){
            set_left_voltage(127);
            set_right_voltage(-127);
        } else {
            set_left_voltage(voltage);
            set_right_voltage(-voltage);
        }
        if(i%5000 == 0){
            std::cout << error;
            std::cout << "  ";
        }
        i++;
    }
    return;
}

void turn_left_relative_debug(double degrees){
    double end_heading = imu.get_heading() - degrees;
    double prev_error = 0;
    double error = degrees;
    double integral = 0;
    double derivative = 0;
    double voltage = 0;
    int i = 0;
    if(end_heading < 0){
         while(imu.get_heading() < 345){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative; 
            if(voltage > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-voltage);
                set_right_voltage(voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        std::cout << "\nflipped to 0\n";
        end_heading+=360;
        error = imu.get_heading() - end_heading;
        while(abs(error) > degree_margin){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative; 
            if(voltage > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-voltage);
                set_right_voltage(voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    } else {
        while(abs(error) > degree_margin){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative; 
            if(voltage > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-voltage);
                set_right_voltage(voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
    return;
    }
}

//Turns the robot to the given heading
void turn(double degrees){
    int start_heading = imu.get_heading(); 
    //right range = start +1, start + 180 % 360
    //left range = start -1, start - 180 % 360
    int low_bound_right_range;
    int high_bound_right_range;
    if(start_heading < 180){
        low_bound_right_range = start_heading;
        high_bound_right_range = start_heading + 180;
        if(degrees > low_bound_right_range && degrees < high_bound_right_range) {
            int x = ((degrees - start_heading) > 0 ) ? (degrees - start_heading) : (degrees - start_heading + 360);
            std::cout << "\nright: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative_debug( x);
        } else {
            int x = ((start_heading-degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative_debug(x);
        }
    } else {
        low_bound_right_range = 360;
        high_bound_right_range = start_heading - 180;
        if(degrees > low_bound_right_range || degrees < high_bound_right_range) {
            int x = ((degrees - start_heading) > 0 ) ? (degrees - start_heading) : (degrees - start_heading + 360);
            std::cout << "\nright: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative_debug( x);
        } else {
            int x = ((start_heading -degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative_debug(x);
        }
    }    
    return;
}

void route_skills(double start_heading){
    int i = 0;
    shoot(5);
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(start_heading));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(-2);
    turn(convert(0));
    go(7*12);
    //deployFlaps();
    turn(convert(45));
    go(2.25*12);
    turn(convert(90));
    push(1.75*12);
    //unDeployFlaps();
    //reset??
    go(-1*12);
    turn(convert(180));
    go(4*12);
    //deployFlaps();
    turn(convert(90));
    go(3*12);
    turn(convert(0));
    push(3.5*12);
}


void ez_skills_start_on_left(){
    shoot(15);
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(45));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(-1);
    turn(convert(315));
    go(1.5*12);
    turn(convert(0));
    rele.set_zero_position(0);
    double num_revolutions_to_elevate = 1.5;
    while(rele.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rele = 127;
        lele = 127;
    }
    rele = 0;
    lele = 0;
    push(4*12+3);
    while(rele.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rele = -127;
        lele = -127;
    }
    eleLock.set_value(1);
}

void ez_skills_start_on_right(){
    shoot(15);
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(315));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(-1);
    turn(convert(45));
    go(1.5*12);
    turn(convert(0));
    rele.set_zero_position(0);
    double num_revolutions_to_elevate = 1.5;
    while(rele.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rele = 127;
        lele = 127;
    }
    rele = 0;
    lele = 0;
    push(4*12+3);
    while(rele.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rele = -127;
        lele = -127;
    }
    eleLock.set_value(1);
}



void route_match_tanner(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(0));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(50);
    turn(convert(45));
    go(12);
    intkPNEU.set_value(1);
    go(-6);
    turn(convert(270));
    push(30);
    go(-12);
    turn(convert(135));
    go(24);
    turn(convert(180));
    go(18);
}

void route_match_drew(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    intk = 127;
    pros::delay(500);
    intkPNEU.set_value(1);
    pros::delay(500);
    pros::delay(500);
    intkPNEU.set_value(0);
    intk = 0;
    pros::delay(500);
    go(-1);
    turn(convert(260));
    flyl=-100;
    intk=-100;
    pros::delay(1000);
    flyl=0;
    intk=0;
    turn(convert(85));
    push(-20);
    go(12);
    turn(convert(45));
    go(15);
    turn(convert(180));
    rele.set_zero_position(0);
    lele.set_zero_position(0);
    while((lele.get_position() < (double)red_ticks_per_rev*0.1)){
        rele = 127;
        lele = 127;
    } 
    rele = 0;
    lele = 0;
    intkPNEU.set_value(0);
    pros::delay(1000);
    push(-28);
}