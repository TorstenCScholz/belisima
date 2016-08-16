#include "Exception.h"

namespace bel {
    namespace exception {
        Exception::Exception(const std::string& msg) : _msg(msg) {
        }

        Exception::Exception(const Exception& that) : _msg(that._msg) {
        }

        Exception::~Exception() {
        }
        
        Exception& Exception::operator=(const Exception& that) {
            _msg = that._msg;

            return *this;
        }
    }
}
