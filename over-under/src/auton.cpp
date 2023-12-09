#include "main.h"
#include "global_defs.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <cmath>
#include <string>

using namespace pros;

const int circum = radius*2*M_PI;

Motor lf(left_front);
Motor lb(left_back);
Motor rf(right_front, -1);
Motor rb(right_back, -1);

void reset_positions(){
    std::cout << lf.set_zero_position(0);
    std::cout << lb.set_zero_position(0);
    std::cout << rf.set_zero_position(0);
    std::cout << rb.set_zero_position(0);
    return;
}

//motor_encoder_units_e_t: 2 for stank bot
//900 encoder_units == 1 revolution
void move_distance(float inches, int velocity){
    double revolutions = inches / circum;
    double encoder_units = revolutions * 900;
    lf.move_absolute(encoder_units, velocity);
    lb.move_absolute(encoder_units, velocity);
    rf.move_absolute(encoder_units, velocity);
    rb.move_absolute(encoder_units, velocity);
}