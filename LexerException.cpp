#include "LexerException.h"

#include <sstream>

namespace bel {
    namespace exception {
        LexerException::LexerException(const std::string& function_name, const std::string& msg, int line) :
            Exception(msg), _function_name(function_name), _line(line) {
            makeMessage();
        }

        LexerException::LexerException(const LexerException& that) :
            Exception(that), _function_name(that._function_name), _line(that._line) {
            makeMessage();
        }

        LexerException::~LexerException() {
        }

        LexerException& LexerException::operator=(const LexerException& that) {
            _msg = that._msg;
            _function_name = that._function_name;
            _line = that._line;

            return *this;
        }

        void LexerException::makeMessage() {
            std::stringstream ss;

            ss << "Line " << _line << " (" << _function_name << "): " << Exception::what();

            _msg = ss.str();
        }
    }
}
