void reset_positions();
void set_all_velocity(int voltage);
void set_left_velocity(int voltage);
void set_right_velocity(int voltage);
void all_brake();
void move_distance_proportional(int inches, float p);
void turn_absolute(int degrees, float p);
void turn_right_relative(int degrees, float p);
void turn_left_relative(int degrees, float p);
