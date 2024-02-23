#define TOP_LEFT_DRIVE 16 
#define MID_LEFT_DRIVE 8 
#define BOT_LEFT_DRIVE 7 

#define TOP_RIGHT_DRIVE 3
#define MID_RIGHT_DRIVE 4
#define BOT_RIGHT_DRIVE 5

#define INTAKE 100 

#define RIGHT_EVEVATION 100 
#define LEFT_ELEVATION 100 

#define FLYWHEEL 10

#define INTAKE_PNEU_RIGHT 'A'
#define INTAKE_PNEU_LEFT 'B'
#define FLIPPERS_RIGHT 'G'
#define FLIPPERS_LEFT 'H'

#define IMU 20

const float wheel_radius = 1.625;
const float encoder_wheel_radius = 2.75;

const int blue_ticks_per_rev = 300;
const int green_ticks_per_rev = 900;
const int red_ticks_per_rev = 1800;