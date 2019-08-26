struct RobotCommand{
    double velocity_x;
    double velocity_y;
    double velocity_r;
    double flat_kick;
    double chip_kick;
    double dribble_spin;
    RobotCommand():velocity_x(0),velocity_y(0),velocity_r(0),flat_kick(0),chip_kick(0),dribble_spin(0) {}
};

void cmdInit();
void setSpeed(int num, double dribble, double vx, double vy, double vr);
void setKick(int num, double kp, double cp);
void sendCommands();

