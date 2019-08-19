import QtQuick 2.10
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import ZSS 1.0 as ZSS
ApplicationWindow{
    visible:true;
//    height:330;
    width:500;
    color: "#333"
    id:window;
    ZSS.Interaction { id : interaction; }

    Timer{
        id:timer;
        interval:16;//15 ms
        running:false;
        repeat:true;
        onTriggered: {
                crazyShow.updateCommand();
                interaction.sendCommands();
        }
    }
    Column{
        anchors.fill:parent;
        Grid{
            id:radioRectangle;
            width:parent.width;
            columns: 1;
            topPadding: 10;
            columnSpacing: 10;
            property int itemWidth : (width - (columns-1) * columnSpacing - 2*padding)/columns;
            ZGroupBox{
                title :qsTr("Sender Setting");
                Column{
                    width:parent.width;
                    Button{
                        x: parent.width / 2 - crazyConnect.width / 2;
                        id : crazyConnect;
                        text : qsTr("Connect");
                        onClicked: {
                            interaction.cmdInit();
                            crazyConnect.enabled = false;
                        }
                    }
                }
            }
            ZGroupBox{
                title : qsTr("Manual Control");
                Column{
                    width:parent.width;
                    Grid{
                        bottomPadding: 5;
                        id : crazyShow;
                        columns: 6;
                        columnSpacing: 10;
                        padding:0;
                        width:parent.width;
                        property int itemWidth : (width - (columns-1) * columnSpacing - 2*padding)/columns;
                        property int robotID : 1;//Robot
                        property int velX : 0;//Vx
                        property int velY : 0;//Vy
                        property int velR : 0;//Vr
                        property bool shoot : false;//Shoot
                        property bool dribble : false;//Dribb
                        property bool rush : false;//Rush

                        property int velXStep : 20;//VxStep
                        property int velYStep : 20;//VyStep
                        property int velRStep : 20;//VrStep
                        property bool mode : false;//KickMode
                        property int dribbleLevel : 3;//DribLevel
                        property int rushSpeed : 20;//RushSpeed

                        property int m_VEL : 150//MaxVel
                        property int velocityRMax : 1023;//MaxVelR
                        property int power : 20;//KickPower

                        property int m_VELR : 200
                        property int velocityMax : 511;
                        property int dribbleMaxLevel : 3;
                        property int kickPowerMax: 650;

                        ZText{ text:qsTr("Robot");}
                        //max num of robot: 12
                        ZSpinBox{ minimumValue:1; maximumValue:12; value:parent.robotID; width:parent.itemWidth
                            onEditingFinished:{parent.robotID = value}}
                        ZText{ text:"Stop" }
                        Button{ text:qsTr("[Space]"); width:parent.itemWidth}
                        ZText{ text:" "}
                        ZText{ text:" "}
                        ZText{ text:qsTr("Vx [W/S]") }
                        //Vx:(-m_VEL, m_VEL)
                        ZSpinBox{ minimumValue:-crazyShow.m_VEL; maximumValue:crazyShow.m_VEL; value:parent.velX;onEditingFinished:{parent.velX = value;}}
                        ZText{ text:qsTr("VxStep") }
                        //VxStep:(1, m_VEL)
                        ZSpinBox{ minimumValue:1; maximumValue:crazyShow.m_VEL; value:parent.velXStep;onEditingFinished:{parent.velXStep = value;}}
                        ZText{ text:qsTr("MaxVel") }
                        //MaxVel:(1, velocityMax)
                        ZSpinBox{ minimumValue:1; maximumValue:crazyShow.velocityMax; value:parent.m_VEL;width:parent.itemWidth
                            onEditingFinished:{parent.m_VEL = value;}}
                        ZText{ text:qsTr("Vy [A/D]")}
                        //Vy:(-m_VEL, m_VEL)
                        ZSpinBox{ minimumValue:-crazyShow.m_VEL; maximumValue:crazyShow.m_VEL; value:parent.velY;width:parent.itemWidth
                            onEditingFinished:{parent.velY = value;}}
                        ZText{ text:qsTr("VyStep") }
                        //VyStep:(1, m_VEL)
                        ZSpinBox{ minimumValue:1; maximumValue:crazyShow.m_VEL; value:parent.velYStep;width:parent.itemWidth
                            onEditingFinished:{parent.velYStep = value;}}
                        ZText{ text:" " }
                        ZText{ text:" " }
                        ZText{ text:qsTr("Vr [←/→]") }
                        //Vr:(-m_VEL, m_VEL)
                        ZSpinBox{ minimumValue:-crazyShow.m_VELR; maximumValue:crazyShow.m_VELR; value:parent.velR;width:parent.itemWidth
                            onEditingFinished:{parent.velR = value;}}
                        ZText{ text:qsTr("VrStep") }
                        //VrStep:(1, m_VELR)
                        ZSpinBox{ minimumValue:1; maximumValue:crazyShow.m_VELR; value:parent.velRStep;width:parent.itemWidth
                            onEditingFinished:{parent.velRStep = value;}}
                        ZText{ text:qsTr("MaxVelR") }
                        //MaxVelR:(1, velocityRMax)
                        ZSpinBox{ minimumValue:1; maximumValue:crazyShow.velocityRMax; value:parent.m_VELR;width:parent.itemWidth
                            onEditingFinished:{parent.m_VELR = value;}}
                        ZText{ text:qsTr("Shoot [E]")}
                        Button{ text:(parent.shoot? qsTr("true") : qsTr("false"));width:parent.itemWidth
                            onClicked: { parent.shoot = !parent.shoot;}
                        }
                        ZText{ text:qsTr("Mode [↑]") }
                        Button{ text:(parent.mode?qsTr("chip"):qsTr("flat"));width:parent.itemWidth
                            onClicked: { parent.mode = !parent.mode }
                        }
                        ZText{ text:qsTr("Power") }
                        //KickPower:(1, kickPowerMax)
                        ZSpinBox{ minimumValue:0; maximumValue:parent.kickPowerMax; value:parent.power;width:parent.itemWidth
                            onEditingFinished:{parent.power = value;}}
                        ZText{ text:qsTr("Dribb [Q]") }
                        Button{ text:(parent.dribble ? qsTr("true") : qsTr("false"));width:parent.itemWidth
                            onClicked: { parent.dribble = !parent.dribble;}
                        }
                        ZText{ text:qsTr("DribLevel") }
                        //DribLevel:(0, dribbleMaxLevel)
                        ZSpinBox{ minimumValue:0; maximumValue:crazyShow.dribbleMaxLevel; value:parent.dribbleLevel;width:parent.itemWidth
                            onEditingFinished:{parent.dribbleLevel = value;}}
                        ZText{ text:" " }
                        Rectangle{ width:parent.itemWidth; height:20; color:parent.shoot ? "red" : "lightgrey"; }
                        // keyboard response
                        Keys.onPressed:getFocus(event);
                        function getFocus(event){
                            switch(event.key){
                            case Qt.Key_Enter:
                            case Qt.Key_Return:
                            case Qt.Key_Escape:
                                crazyShow.focus = true;
                                break;
                            default:
                                event.accepted = false;
                                return false;
                            }
                            event.accepted = true;
                        }
                        function updateStop(){
                            crazyShow.velX = 0;
                            crazyShow.velY = 0;
                            crazyShow.velR = 0;
                            crazyShow.shoot = false;
                            crazyShow.dribble = false;
                            crazyShow.rush = false;
                        }
                        function updateRush(){
                            if(crazyShow.rush){
                                crazyShow.velX = crazyShow.rushSpeed;
                                crazyShow.velY = 0;
                                crazyShow.velR = 0;
                                crazyShow.shoot = true;
                                crazyShow.dribble = false;
                            }else{
                                crazyShow.updateStop();
                            }
                        }
                        function handleKeyboardEvent(e){
                            switch(e){
                            case 'U':crazyShow.mode = !crazyShow.mode;
                                break;
                            case 'a':crazyShow.velY = crazyShow.limitVel(crazyShow.velY-crazyShow.velYStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;
                            case 'd':crazyShow.velY = crazyShow.limitVel(crazyShow.velY+crazyShow.velYStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;
                            case 'w':crazyShow.velX = crazyShow.limitVel(crazyShow.velX+crazyShow.velXStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;
                            case 's':crazyShow.velX = crazyShow.limitVel(crazyShow.velX-crazyShow.velXStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;
                            case 'q':crazyShow.dribble = !crazyShow.dribble;
                                break;
                            case 'e':crazyShow.shoot = !crazyShow.shoot;
                                break;
                            case 'L':crazyShow.velR = crazyShow.limitVel(crazyShow.velR-crazyShow.velRStep,-crazyShow.m_VELR,crazyShow.m_VELR);
                                break;
                            case 'R':crazyShow.velR = crazyShow.limitVel(crazyShow.velR+crazyShow.velRStep,-crazyShow.m_VELR,crazyShow.m_VELR);
                                break;
                            case 'S':crazyShow.updateStop();
                                break;
                            case 'g':crazyShow.rush = !crazyShow.rush; updateRush();
                                break;
                            default:
                                return false;
                            }
                            updateCommand();
                        }
                        function updateCommand(){
                            interaction.updateCommands(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.dribbleLevel,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                        }
                        function updateFromGamepad(){
                            crazyShow.velX = -parseInt(gamepad.axisLeftY*10)/10.0*crazyShow.m_VEL;
                            crazyShow.velY = parseInt(gamepad.axisLeftX*10)/10.0*crazyShow.m_VEL;
                            crazyShow.velR = parseInt(gamepad.axisRightX*10)/10.0*crazyShow.m_VELR;
                            if(gamepad.buttonX > 0){
                                crazyShow.power = parseInt(gamepad.buttonL2*10)/10.0*crazyShow.kickPowerMax;
                                crazyShow.mode = true;
                                crazyShow.shoot = gamepad.buttonX;
                            }
                            else if(gamepad.buttonY > 0){
                                crazyShow.power = parseInt(gamepad.buttonL2*10)/10.0*crazyShow.kickPowerMax;
                                crazyShow.mode = false;
                                crazyShow.shoot = gamepad.buttonY;

                            }
                            else{
                                crazyShow.shoot = 0;
                            }

                            if(gamepad.buttonR2 > 0){
                                crazyShow.dribbleLevel =  parseInt(gamepad.buttonR2*10)/10.0*crazyShow.dribbleMaxLevel;
                                crazyShow.dribble = true ;
                            }
                            else{
                                crazyShow.dribble = false ;
                            }

                            console.log(velX,velY);
                        }
                        function limitVel(vel,minValue,maxValue){
                            if(vel>maxValue) return maxValue;
                            if(vel<minValue) return minValue;
                            return vel;
                        }
                        Shortcut{
                            sequence:"G";
                            onActivated:crazyShow.handleKeyboardEvent('g');
                        }
                        Shortcut{
                            sequence:"A";
                            onActivated:crazyShow.handleKeyboardEvent('a');
                        }
                        Shortcut{
                            sequence:"Up";
                            onActivated:crazyShow.handleKeyboardEvent('U');
                        }
                        Shortcut{
                            sequence:"D"
                            onActivated:crazyShow.handleKeyboardEvent('d');
                        }
                        Shortcut{
                            sequence:"W"
                            onActivated:crazyShow.handleKeyboardEvent('w');
                        }
                        Shortcut{
                            sequence:"S"
                            onActivated:crazyShow.handleKeyboardEvent('s');
                        }
                        Shortcut{
                            sequence:"Q"
                            onActivated:crazyShow.handleKeyboardEvent('q');
                        }
                        Shortcut{
                            sequence:"E"
                            onActivated:crazyShow.handleKeyboardEvent('e');
                        }
                        Shortcut{
                            sequence:"Left"
                            onActivated:crazyShow.handleKeyboardEvent('L');
                        }
                        Shortcut{
                            sequence:"Right"
                            onActivated:crazyShow.handleKeyboardEvent('R');
                        }
                        Shortcut{
                            sequence:"Space"
                            onActivated:crazyShow.handleKeyboardEvent('S');
                        }
                    }
                    Button{
                        anchors.right: parent.right;
                        id:crazyStart;
                        text:qsTr("Start");
                        property bool ifStarted:false;
                        enabled : true;
                        onClicked:{
                            handleClickEvent();
                        }

                        function handleClickEvent(){
                            if(ifStarted){
                                timer.stop();
                            }else{
                                timer.start();
                            }
                            ifStarted = !ifStarted;
                            text = (ifStarted ? qsTr("Stop") : qsTr("Start"));
                        }
                    }
                }
            }
        }
    }
}


