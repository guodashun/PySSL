#include "visionmodule.h"
#include "Semaphore.h"
#include <iostream>
#include <thread>

Semaphore vision_receive_semaphore(0);

VisionModule::VisionModule(int port)
    : _port(port)
    , _udpReceiveThread(nullptr)
    , udpReceiveSocket() {
    udpReceiveSocket.bind(QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress);
	_udpReceiveThread = new std::thread([=] {udpReceive();});
    _udpReceiveThread->detach();
}

void VisionModule::udpReceive() {
    static QByteArray datagram;
    while(true) {
    	std::this_thread::sleep_for(std::chrono::milliseconds(2));
	    while (udpReceiveSocket.hasPendingDatagrams()) {
	        datagram.resize(udpReceiveSocket.pendingDatagramSize());
	        udpReceiveSocket.readDatagram(datagram.data(), datagram.size());
	        storeData(datagram.data(), datagram.size());
	    }
	}
}

void VisionModule::initVision() {
	detectionFrame.ball.init();
	for (int i = 0; i < ROBOT_MAX_NUM; i++) {
		detectionFrame.robots_blue[i].init();
		detectionFrame.robots_yellow[i].init();
	}
}

void VisionModule::storeData(void* data, int size) {
    static Vision_DetectionFrame detection;
    detection.ParseFromArray(data, size);
    vision_update_mutex.lock();
    initVision();
    if (detection.has_balls()) {
        auto ball = detection.balls();
        detectionFrame.ball.vel_x = ball.vel_x();
        detectionFrame.ball.vel_y = ball.vel_y();
        detectionFrame.ball.area = ball.area();
        detectionFrame.ball.x = ball.x();
        detectionFrame.ball.y = ball.y();
        detectionFrame.ball.height = ball.height();
        detectionFrame.ball.ball_state = ball.ball_state();
        detectionFrame.ball.last_touch = ball.last_touch();
        detectionFrame.ball.valid = ball.valid();
        detectionFrame.ball.raw_x = ball.raw_x();
        detectionFrame.ball.raw_y = ball.raw_y();
        detectionFrame.ball.chip_predict_x = ball.chip_predict_x();
        detectionFrame.ball.chip_predict_y = ball.chip_predict_y();
    } else {
        std::cout << "Ball not Found!" << std::endl;
    }
    int blueSize;
    if (detection.robots_blue_size() > ROBOT_MAX_NUM) {
        std::cout << "Blue robot num is invalid, remain top " << ROBOT_MAX_NUM << "robots imformation" << std::endl;
        blueSize = ROBOT_MAX_NUM;
    } else
        blueSize = detection.robots_blue_size();
    int yellowSize;
    if (detection.robots_yellow_size() > ROBOT_MAX_NUM) {
        std::cout << "Yellow robot num is invalid, remain top " << ROBOT_MAX_NUM << "robots imformation" << std::endl;
        yellowSize = ROBOT_MAX_NUM;
    } else
        yellowSize = detection.robots_yellow_size();
    for (int i = 0; i < blueSize; i++) {
        auto robot = detection.robots_blue(i);
        int index = robot.robot_id();
        detectionFrame.robots_blue[index].valid = robot.valid();
        detectionFrame.robots_blue[index].x = robot.x();
        detectionFrame.robots_blue[index].y = robot.y();
        detectionFrame.robots_blue[index].orientation = robot.orientation();
        detectionFrame.robots_blue[index].vel_x = robot.vel_x();
        detectionFrame.robots_blue[index].vel_y = robot.vel_y();
        detectionFrame.robots_blue[index].rotate_vel = robot.rotate_vel();
        detectionFrame.robots_blue[index].accelerate_x = robot.accelerate_x();
        detectionFrame.robots_blue[index].accelerate_y = robot.accelerate_y();
        detectionFrame.robots_blue[index].raw_x = robot.raw_x();
        detectionFrame.robots_blue[index].raw_y = robot.raw_y();
        detectionFrame.robots_blue[index].raw_orientation = robot.raw_orientation();
        detectionFrame.robots_blue[index].raw_vel_x = robot.raw_vel_x();
        detectionFrame.robots_blue[index].raw_vel_y = robot.raw_vel_y();
        detectionFrame.robots_blue[index].raw_rotate_vel = robot.raw_rotate_vel();
    }
    for (int i = 0; i < yellowSize; i++) {
        auto robot = detection.robots_yellow(i);
        int index = robot.robot_id();
        detectionFrame.robots_yellow[index].valid = robot.valid();
        detectionFrame.robots_yellow[index].x = robot.x();
        detectionFrame.robots_yellow[index].y = robot.y();
        detectionFrame.robots_yellow[index].orientation = robot.orientation();
        detectionFrame.robots_yellow[index].vel_x = robot.vel_x();
        detectionFrame.robots_yellow[index].vel_y = robot.vel_y();
        detectionFrame.robots_yellow[index].rotate_vel = robot.rotate_vel();
        detectionFrame.robots_yellow[index].accelerate_x = robot.accelerate_x();
        detectionFrame.robots_yellow[index].accelerate_y = robot.accelerate_y();
        detectionFrame.robots_yellow[index].raw_x = robot.raw_x();
        detectionFrame.robots_yellow[index].raw_y = robot.raw_y();
        detectionFrame.robots_yellow[index].raw_orientation = robot.raw_orientation();
        detectionFrame.robots_yellow[index].raw_vel_x = robot.raw_vel_x();
        detectionFrame.robots_yellow[index].raw_vel_y = robot.raw_vel_y();
        detectionFrame.robots_yellow[index].raw_rotate_vel = robot.raw_rotate_vel();
    }
    vision_update_mutex.unlock();
    vision_receive_semaphore.Signal();
}

ZVision_DetectionFrame VisionModule::getVision() {
	vision_receive_semaphore.Wait();
	vision_update_mutex.lock();
	detectionFrameSafe = detectionFrame;
	vision_update_mutex.unlock();
	return detectionFrameSafe;
}