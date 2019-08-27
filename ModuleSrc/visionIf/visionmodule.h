#include <QtNetwork/QUdpSocket>
#include <QMutex>
#include "vision_detection.pb.h"
#include <thread>

#define ROBOT_MAX_NUM 8

struct ZVision_DetectionBall {
    float  vel_x;
    float  vel_y;
    int    area;
    float  x;
    float  y;
    float  height;
    int    ball_state;
    int    last_touch;
    bool   valid;
    float  raw_x;
    float  raw_y;
    float  chip_predict_x;
    float  chip_predict_y;
    void init() {
        this->vel_x = this->vel_y = this->height = 0;
        this->area = this->ball_state = this->last_touch = 0;
        this->x = this->y = this->raw_x = this->raw_y = this->chip_predict_y = this->chip_predict_x = 9999;
        this->valid = false;
    }
};

struct ZVision_DetectionRobot {
    bool   valid;
    float  x;
    float  y;
    float  orientation;
    float  vel_x;
    float  vel_y;
    float  rotate_vel;
    float  accelerate_x;
    float  accelerate_y;
    float  raw_x;
    float  raw_y;
    float  raw_orientation;
    float  raw_vel_x;
    float  raw_vel_y;
    float  raw_rotate_vel;
    void init() {
        this->vel_x = this->vel_y = this->rotate_vel = this->accelerate_x = this->accelerate_y 
                    = this->orientation = this->raw_orientation = this->raw_rotate_vel 
                    = this->raw_vel_x = this->raw_vel_y = 0;
        this->x = this->y = this->raw_x = this->raw_y = 9999;
        this->valid = false;
    }
};

struct ZVision_DetectionFrame {
    ZVision_DetectionBall  ball;   
    ZVision_DetectionRobot robots_yellow[ROBOT_MAX_NUM];
    ZVision_DetectionRobot robots_blue[ROBOT_MAX_NUM];
    ZVision_DetectionRobot get_robots_yellow(int index){
        if (index >= 0 && index < ROBOT_MAX_NUM) {
            return this->robots_yellow[index];
        } else
            return this->robots_yellow[0];
    }
    ZVision_DetectionRobot get_robots_blue(int index){
        if (index >= 0 && index < ROBOT_MAX_NUM) {
            return this->robots_blue[index];
        } else
            return this->robots_blue[0];
    }
};

class VisionModule{
public:
    VisionModule(int port = 23333);
	VisionModule(const VisionModule& p) {}
    ZVision_DetectionFrame getVision();
private:
    ZVision_DetectionFrame detectionFrame, detectionFrameSafe;
    int _port;
    QMutex vision_update_mutex;
    std::thread* _udpReceiveThread;
    QUdpSocket udpReceiveSocket;
	void initVision();
	void udpReceive();
    void storeData(void *data, int size);
};
