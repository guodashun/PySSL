#include "interaction.h"
#include "visionmodule.h"
#include "globaldata.h"
#include "maintain.h"
#include "actionmodule.h"
#include "globalsettings.h"
#include "simulator.h"
#include "parammanager.h"
#include "simmodule.h"
#include "refereethread.h"
#include "rec_recorder.h"
#include "networkinterfaces.h"
#include <QProcess>
namespace {
QProcess *medusaProcess = nullptr;
QProcess *medusaProcess2 = nullptr;
QProcess *simProcess = nullptr;
QProcess *crazyProcess = nullptr;
QProcess *monitorProcess = nullptr;
}
Interaction::Interaction(QObject *parent) : QObject(parent) {
}
Interaction::~Interaction() {
    if (simProcess != nullptr) {
        if (simProcess->isOpen()) {
            simProcess->close();
        }
        delete simProcess;
        simProcess = nullptr;
        QTextStream(stdout) << "\n------------------------------------\nexit Sim\n------------------------------------\n";
    }
    if (medusaProcess != nullptr) {
        if (medusaProcess->isOpen()) {
            medusaProcess->close();
        }
        delete medusaProcess;
        medusaProcess = nullptr;
        QTextStream(stdout) << "\n------------------------------------\nexit Medusa\n------------------------------------\n";
    }
    if (medusaProcess2 != nullptr) {
        if (medusaProcess2->isOpen()) {
            medusaProcess2->close();
        }
        delete medusaProcess2;
        medusaProcess2 = nullptr;
        QTextStream(stdout) << "\n------------------------------------\nexit 2 Medusa\n------------------------------------\n";
    }
    if (crazyProcess != nullptr) {
        if (crazyProcess->isOpen()) {
            crazyProcess->close();
        }
        delete crazyProcess;
        crazyProcess = nullptr;
        QTextStream(stdout) << "\n------------------------------------\nexit Crazy\n------------------------------------\n";
    }
    if (monitorProcess != nullptr) {
        if (monitorProcess->isOpen()) {
            monitorProcess->close();
        }
        delete monitorProcess;
        monitorProcess = nullptr;
        QTextStream(stdout) << "\n------------------------------------\nexit Monitor\n------------------------------------\n";
    }
}
void Interaction::updateInterfaces(){
    ZNetworkInterfaces::instance()->updateInterfaces();
}
QStringList Interaction::getInterfaces(){
    return ZNetworkInterfaces::instance()->getInterfaces();
}
void Interaction::changeVisionInterface(int index){
//    if(portNum < ports.size() && portNum >= 0){
//        serial.setPortName(ports[portNum]);
//        return true;
//    }
    VisionModule::instance()->setInterfaceIndex(index);
//    qDebug() << "vision interface : " << index;
}
void Interaction::changeRadioInterface(bool ifBlue,bool ifSender,int index){
//    qDebug() << "radio  interface : " << ifBlue << ifSender << index;
}
void Interaction::setIfEdgeTest(bool ifEdgeTest) {
    VisionModule::instance()->setIfEdgeTest(ifEdgeTest);
}
void Interaction::showIfEdgeTest() {
    std::cout << VisionModule::instance()->showIfEdgeTest() << std::endl;
}
void Interaction::setVision(bool needStart, bool real) {
    ZSS::ZActionModule::instance()->setSimulation(!real);
    if (needStart) {
        ZSS::ZParamManager::instance()->changeParam("Alert/IsSimulation", !real);
        VisionModule::instance()->udpSocketConnect(real);
        if (real) RefereeThread::instance()->start();
        //Maintain::instance()->udpSocketConnect();
    } else {
        ZSS::ZParamManager::instance()->changeParam("Alert/IsSimulation", !real);
        VisionModule::instance()->udpSocketDisconnect();
        RefereeThread::instance()->disconnectTCP();
        //Maintain::instance()->udpSocketDisconnect();
    }
}
void Interaction::controlCamera(int index, bool state) {
    GlobalData::instance()->cameraControl[index] = state;
}
void Interaction::controlProcess(int index, bool state) {
    GlobalData::instance()->processControl[index] = state;
}
bool Interaction::connectRadio(bool sw, int id, int frq) {
    if(sw) {
        ZCommunicator::instance()->disconnectMedusa(id);
        ZCommunicator::instance()->connectMedusa(id);
        ZSS::ZActionModule::instance()->disconnectRadio(id);
        return ZSS::ZActionModule::instance()->connectRadio(id, frq);
    } else {
//        return ZSS::ZActionModule::instance()->disconnectRadio(id);
    }
}
bool Interaction::connectSim(bool sw, int id, bool color) {
    if(sw) {
        ZCommunicator::instance()->disconnectMedusa(id);
        ZCommunicator::instance()->connectMedusa(id);
        ZSS::ZSimModule::instance()->disconnectSim(color);
        return ZSS::ZSimModule::instance()->connectSim(color);
    } else {
//        return ZSS::ZSimModule::instance()->disconnectSim(color); //fix a bug for Medusa out of Athena
    }
}
bool Interaction::controlMonitor(bool control) {
#ifdef WIN32
    if(control == false) {
        if (monitorProcess != nullptr) {
            if (monitorProcess->isOpen()) {
                monitorProcess->close();
            }
            delete monitorProcess;
            monitorProcess = nullptr;
            QTextStream(stdout) << "\n------------------------------------\nexit Monitor\n------------------------------------\n";
        }
    } else {
        monitorProcess = new QProcess();
        QString name = "./ProcessAlive.exe";
        monitorProcess->start(name);
        QTextStream(stdout) << "\n------------------------------------\n" << "running " << name << "\n------------------------------------\n";
    }
    return true;
#else
    if(control == false) {
        if (monitorProcess != nullptr) {
            if (monitorProcess->isOpen()) {
                monitorProcess->close();
            }
            delete monitorProcess;
            monitorProcess = nullptr;
            QTextStream(stdout) << "\n------------------------------------\nexit Monitor\n------------------------------------\n";
        }
    } else {
        monitorProcess = new QProcess();
        QString name = "sh ProcessAlive.sh";
        monitorProcess->start(name);
        QTextStream(stdout) << "\n------------------------------------\n" << "start : " << name << "\n------------------------------------\n";
    }
    return true;
#endif
}
bool Interaction::controlMedusa(bool control) {
    if(control == false) {
        if (medusaProcess != nullptr) {
            if (medusaProcess->isOpen()) {
                medusaProcess->close();
            }
            delete medusaProcess;
            medusaProcess = nullptr;
            QTextStream(stdout) << "\n------------------------------------\nexit Medusa\n------------------------------------\n";
        }
        emit GlobalSettings::instance()->clearOutput();
    } else {
        medusaProcess = new QProcess();
        QString name = "./Medusa";
        connect(medusaProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(medusaPrint()));
        medusaProcess->start(name);
        QTextStream(stdout) << "\n------------------------------------\n" << "running " << name << "\n------------------------------------\n";
    }
    return true;
}
bool Interaction::controlMedusa2(bool control) {
    if(control == false) {
        if (medusaProcess2 != nullptr) {
            if (medusaProcess2->isOpen()) {
                medusaProcess2->close();
            }
            delete medusaProcess2;
            medusaProcess2 = nullptr;
            QTextStream(stdout) << "\n------------------------------------\nexit 2 Medusa\n------------------------------------\n";
        }
    } else {
        medusaProcess2 = new QProcess();
        QString name = "./Medusa";
        medusaProcess2->start(name);
        QTextStream(stdout) << "\n------------------------------------\n" << "running 2 " << name << "\n------------------------------------\n";
    }
    return true;
}
bool Interaction::controlSim(bool control, bool show) {
    if(control == false) {
        if (simProcess != nullptr) {
            if (simProcess->isOpen()) {
                simProcess->close();
            }
            delete simProcess;
            simProcess = nullptr;
            QTextStream(stdout) << "\n------------------------------------\nexit Sim\n------------------------------------\n";
        }
    } else {
        simProcess = new QProcess();
        QString name = "./grsim";
        QStringList args;
        if(!show) args << "-H";
        simProcess->start(name, args);
        QTextStream(stdout) << "\n------------------------------------\n" << "running " << name << "\n------------------------------------\n";
    }
    return true;
}
bool Interaction::controlCrazy(bool control) {
    if(control == false) {
        if (crazyProcess != nullptr) {
            if (crazyProcess->isOpen()) {
                crazyProcess->close();
            }
            delete crazyProcess;
            crazyProcess = nullptr;
            QTextStream(stdout) << "\n------------------------------------\nexit Crazy\n------------------------------------\n";
        }
    } else {
        crazyProcess = new QProcess();
        QString name = "./Crazy.exe";
        crazyProcess->start(name);
        QTextStream(stdout) << "\n------------------------------------\n" << "running " << name << "\n------------------------------------\n";
    }
    return true;
}
bool Interaction::changeMedusaSettings(bool color, bool side) {
    ZSS::ZParamManager::instance()->changeParam("ZAlert/IsYellow", color);
    ZSS::ZParamManager::instance()->changeParam("ZAlert/IsRight", side);
    return true;
}
void Interaction::medusaPrint() {
    emit GlobalSettings::instance()->addOutput(medusaProcess->readAllStandardOutput());
}
void Interaction::robotControl(int id, int team) {
    Simulator::instance()->controlRobot(id, team == 1);
}

void Interaction::kill() {
    ZRecRecorder::instance()->stop();
    QProcess killTask;
#ifdef WIN32
    RefereeThread::instance()->disconnectTCP();
    QString athena = "taskkill -im Athena.exe -f";
    QString medusa = "taskkill -im Medusa.exe -f";
    QString grSim = "taskkill -im grSim.exe -f";
#else
    QString athena = "pkill Client";
    QString medusa = "pkill Medusa";
    QString grSim = "pkill grsim";
#endif
    if (monitorProcess != nullptr) {
        if (monitorProcess->isOpen()) {
            monitorProcess->close();
        }
        delete monitorProcess;
        monitorProcess = nullptr;
    }
    killTask.execute(medusa);
    killTask.execute(grSim);
    killTask.execute(athena);
    killTask.close();
}

