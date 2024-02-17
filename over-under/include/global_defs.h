#define TOP_LEFT_DRIVE 17 
#define MID_LEFT_DRIVE 5 
#define BOT_LEFT_DRIVE 2

#define TOP_RIGHT_DRIVE 19
#define MID_RIGHT_DRIVE 15
#define BOT_RIGHT_DRIVE 20

#define LEFT_INTAKE 7
#define RIGHT_INTAKE 10 

#define EVEVATION 8 

#define LEFT_CAT 3 
#define RIGHT_CAT 14

#define LIMIT_SWITCH 'A'
#define INTAKE 'G'
#define FLIPPERS 'H'

#define IMU 13

const float wheel_radius = 1.625;
const float encoder_wheel_radius = 2.75;

const int blue_ticks_per_rev = 300;
const int green_ticks_per_rev = 900;
const int red_ticks_per_rev = 1800;