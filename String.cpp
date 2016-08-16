#include "String.h"

namespace bel {
    namespace expr {
        String::String(const std::string& content) : _cont(content) {
        }

        String::String(const String& that) : _cont(that._cont) {
        }

        String::~String() {}
        
        String& String::operator=(const String& that) {
            _cont = that._cont;

            return *this;
        }

        Expression* String::eval(Environment& env) {
            return new String(*this);
        }

        Type::Value String::type() const {
            return Type::String;
        }

        std::string String::toString() const {
            return _cont;
        }

        Expression* String::clone() const {
            return new String(*this);
        }

        size_t String::length() const {
            return _cont.length();
        }
    }
}
