#include <boost/python.hpp>
#include "CommandInterface.h"

#define BOOST_PYTHON_STATIC_LIB

using namespace boost::python;

BOOST_PYTHON_MODULE(cmdIf) {
    def("init", cmdInit);
    def("setSpeed", setSpeed);
    def("setKick", setKick);
    def("sendCommands", sendCommands);
}
