#include "ParserException.h"

#include <sstream>

namespace bel {
    namespace exception {
        ParserException::ParserException(const std::string& function_name, const std::string& msg, int line) :
            Exception(msg), _function_name(function_name), _line(line) {
            makeMessage();
        }

        ParserException::ParserException(const ParserException& that) :
            Exception(that), _function_name(that._function_name), _line(that._line) {
            makeMessage();
        }

        ParserException::~ParserException() {
        }

        ParserException& ParserException::operator=(const ParserException& that) {
            _msg = that._msg;
            _function_name = that._function_name;
            _line = that._line;

            return *this;
        }

        void ParserException::makeMessage() {
            std::stringstream ss;
            
            ss << "Line " << _line << " (" << _function_name << "): " << Exception::what();

            _msg = ss.str();
        }
    }
}
