#include <boost/python.hpp>
#include "visionmodule.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(visionIf) {
    class_<ZVision_DetectionBall>("Vision_DetectionBall")
    	.def_readonly("vel_x", &ZVision_DetectionBall::vel_x)
    	.def_readonly("vel_y", &ZVision_DetectionBall::vel_y)
    	.def_readonly("area", &ZVision_DetectionBall::area)
    	.def_readonly("x", &ZVision_DetectionBall::x)
    	.def_readonly("y", &ZVision_DetectionBall::y)
    	.def_readonly("height", &ZVision_DetectionBall::height)
        .def_readonly("ball_state", &ZVision_DetectionBall::ball_state)
        .def_readonly("last_touch", &ZVision_DetectionBall::last_touch)
        .def_readonly("valid", &ZVision_DetectionBall::valid)
        .def_readonly("raw_x", &ZVision_DetectionBall::raw_x)
        .def_readonly("raw_y", &ZVision_DetectionBall::raw_y)
        .def_readonly("chip_predict_x", &ZVision_DetectionBall::chip_predict_x)
        .def_readonly("chip_predict_y", &ZVision_DetectionBall::chip_predict_y);
    class_<ZVision_DetectionRobot>("Vision_DetectionRobot")
    	.def_readonly("valid", &ZVision_DetectionRobot::valid)
    	.def_readonly("x", &ZVision_DetectionRobot::x)
    	.def_readonly("y", &ZVision_DetectionRobot::y)
    	.def_readonly("orientation", &ZVision_DetectionRobot::orientation)
    	.def_readonly("vel_x", &ZVision_DetectionRobot::vel_x)
    	.def_readonly("vel_y", &ZVision_DetectionRobot::vel_y)
    	.def_readonly("rotate_vel", &ZVision_DetectionRobot::rotate_vel)
        .def_readonly("accelerate_x", &ZVision_DetectionRobot::accelerate_x)
        .def_readonly("accelerate_y", &ZVision_DetectionRobot::accelerate_y)
    	.def_readonly("raw_x", &ZVision_DetectionRobot::raw_x)
    	.def_readonly("raw_y", &ZVision_DetectionRobot::raw_y)
    	.def_readonly("raw_orientation", &ZVision_DetectionRobot::raw_orientation)
    	.def_readonly("raw_vel_x", &ZVision_DetectionRobot::raw_vel_x)
    	.def_readonly("raw_vel_y", &ZVision_DetectionRobot::raw_vel_y)
    	.def_readonly("raw_rotate_vel", &ZVision_DetectionRobot::raw_rotate_vel);
    class_<ZVision_DetectionFrame>("Vision_DetectionFrame")
        .def_readonly("ball", &ZVision_DetectionFrame::ball)
        .def("robots_yellow", &ZVision_DetectionFrame::get_robots_yellow)
        .def("robots_blue", &ZVision_DetectionFrame::get_robots_blue);
    class_<VisionModule>("vm", init<int>())
    	.def("getVision", &VisionModule::getVision);
}
