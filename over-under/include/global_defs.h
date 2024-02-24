#define TOP_LEFT_DRIVE 8
#define MID_LEFT_DRIVE -20
#define BOT_LEFT_DRIVE -7

#define TOP_RIGHT_DRIVE -19
#define MID_RIGHT_DRIVE 3
#define BOT_RIGHT_DRIVE 17

#define INTAKE 11

#define RIGHT_ELEVATION -10
#define LEFT_ELEVATION 4

#define FLYWHEEL_L 6
#define FLYWHEEL_R -1

#define INTAKE_PNEU 'B'
#define ELEVATION_LOCK 'C'
#define FLIPPERS 'F'

#define IMU 13

const float wheel_radius = 1.625;
const float encoder_wheel_radius = 2.75;

const int blue_ticks_per_rev = 300;
const int green_ticks_per_rev = 900;
const int red_ticks_per_rev = 1800;