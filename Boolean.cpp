#include "Boolean.h"

namespace bel {
    namespace expr {
        Boolean::Boolean() : _val(false) {
        }

        Boolean::Boolean(bool val) : _val(val) {
        }

        Boolean::Boolean(const Boolean& that) : _val(that._val) {
        }

        Boolean::~Boolean() {}
        
        Boolean& Boolean::operator=(const Boolean& that) {
            _val = that._val;

            return *this;
        }

        bool Boolean::value() const {
            return _val;
        }

        Expression* Boolean::eval(Environment& env) {
            return clone();
        }

        Type::Value Boolean::type() const {
            return Type::Boolean;
        }

        std::string Boolean::toString() const {
            return _val ? "true" : "false";
        }

        Expression* Boolean::clone() const {
            return new Boolean(*this);
        }
    }
}
