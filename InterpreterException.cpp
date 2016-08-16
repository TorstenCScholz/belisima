#include "InterpreterException.h"

#include <sstream>

namespace bel {
    namespace exception {
        InterpreterException::InterpreterException(const std::string& function_name, const std::string& msg) :
            Exception(msg), _function_name(function_name) {
            makeMessage();
        }

        InterpreterException::InterpreterException(const InterpreterException& that) :
            Exception(that), _function_name(that._function_name) {
            makeMessage();
        }

        InterpreterException::~InterpreterException() {
        }

        InterpreterException& InterpreterException::operator=(const InterpreterException& that) {
            _msg = that._msg;
            _function_name = that._function_name;

            return *this;
        }

        void InterpreterException::makeMessage() {
            std::stringstream ss;

            ss << _function_name << ": " << Exception::what();

            _msg = ss.str();
        }
    }
}
