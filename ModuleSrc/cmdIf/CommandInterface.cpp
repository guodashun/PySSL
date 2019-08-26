#include "CommandInterface.h"
#include <iostream>
#include "grSim_Packet.pb.h"
#include "zss_cmd.pb.h"
#include "staticparams.h"
#include "parammanager.h"
#include "param.h"
#include <cmath>
#include <QtNetwork/QUdpSocket>
namespace {
int SIM_PORT = 0;
int SELF_PORT = 0;
int CHIP_ANGLE = 1;
int TEAM;
ZSS::Protocol::Robots_Command robots_command;

//data
RobotCommand commands[Param::Field::MAX_PLAYER_NUM];
QUdpSocket *command_socket;
}

void cmdInit() {
    bool isYellow = false;
    ZSS::ZParamManager::instance()->loadParam(SIM_PORT, "AlertPorts/SimPort", 20011);
    ZSS::ZParamManager::instance()->loadParam(SELF_PORT, "Ports/SimSelfPort", 30015);
    ZSS::ZParamManager::instance()->loadParam(CHIP_ANGLE, "Simulator/ChipAngle", 45);
    ZSS::ZParamManager::instance()->loadParam(isYellow, "ZAlert/IsYellow", false);
    TEAM = isYellow ? PARAM::YELLOW : PARAM::BLUE;
    command_socket = new QUdpSocket();
}

void setSpeed(int num, double dribble, double vx, double vy, double vr) {
    int number = num - 1;//找到+1处后可以把此处去掉，车号变回0-11
    if (number < 0 || number > Param::Field::MAX_PLAYER - 1) {
    
        return;
    }
    commands[number].dribble_spin = dribble;
    commands[number].velocity_x = vx;
    commands[number].velocity_y = vy;
    commands[number].velocity_r = vr / 360 * 3.14;

}
void setKick(int num, double kp, double cp) {
    int number = num - 1;//找到+1处后可以把此处去掉，车号变回0-11
    if (number < 0 || number > Param::Field::MAX_PLAYER - 1) {
    
        return;
    }
    commands[number].flat_kick = kp;
    commands[number].chip_kick = cp;
}

void sendCommands() {
    for (int i = 0; i < Param::Field::MAX_PLAYER; i++) {
//        if(!VisionModule::Instance()->OurPlayer(i+1).Valid()){
//            continue;
//        }
        auto robot_command = robots_command.add_command();
        robot_command->set_robot_id(i);
        robot_command->set_velocity_x(commands[i].velocity_x);
        robot_command->set_velocity_y(commands[i].velocity_y);
        robot_command->set_velocity_r(commands[i].velocity_r);
        robot_command->set_dribbler_spin(commands[i].dribble_spin);
//        if(commands[i].dribble_spin >=1){
//            GDebugEngine::Instance()->gui_debug_arc(VisionModule::Instance()->OurPlayer(i+1).RawPos(),5,0,360,COLOR_BLACK);
//        }
        if(commands[i].chip_kick < 0.001) { //flat kick
            robot_command->set_kick(false);
            robot_command->set_power(commands[i].flat_kick);
        } else {
            robot_command->set_kick(true);
            robot_command->set_power(commands[i].chip_kick);
        }
    }
    int size = ::robots_command.ByteSize();
    QByteArray data(size, 0);
    ::robots_command.SerializeToArray(data.data(), size);
    command_socket->writeDatagram(data.data(), size, QHostAddress(ZSS::LOCAL_ADDRESS), ZSS::Athena::CONTROL_SEND[TEAM]);
    ::robots_command.Clear();
    memset(commands,0,sizeof(RobotCommand)*Param::Field::MAX_PLAYER_NUM);
}


