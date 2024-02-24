#define TOP_LEFT_DRIVE 16 
#define MID_LEFT_DRIVE 8 
#define BOT_LEFT_DRIVE 7 

#define TOP_RIGHT_DRIVE 3
#define MID_RIGHT_DRIVE 4
#define BOT_RIGHT_DRIVE 5

#define INTAKE 19

#define RIGHT_EVEVATION 2
#define LEFT_ELEVATION 6

#define FLYWHEEL 10

#define INTAKE_PNEU 'A'
#define FLIPPERS 'B'
#define ELEVATION_LOCK 'C'

#define IMU 20

const float wheel_radius = 1.625;
const float encoder_wheel_radius = 2.75;

const int blue_ticks_per_rev = 300;
const int green_ticks_per_rev = 900;
const int red_ticks_per_rev = 1800;