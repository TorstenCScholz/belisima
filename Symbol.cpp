#include "Symbol.h"
#include "Environment.h"
#include "Frame.h"

namespace bel {
    namespace expr {
        Symbol::Symbol(const std::string& name) : _name(name) {
        }

        Symbol::Symbol(const Symbol& that) : _name(that._name) {
        }

        Symbol::~Symbol() {
        }

        Symbol& Symbol::operator=(const Symbol& that) {
            _name = that._name;

            return *this;
        }

        Expression* Symbol::eval(Environment& env) {
            return env.retrieve(_name)->clone();
        }

        Type::Value Symbol::type() const {
            return Type::Symbol;
        }

        std::string Symbol::toString() const {
            return name();
        }

        std::string Symbol::name() const {
            return _name;
        }

        Expression* Symbol::clone() const {
            return new Symbol(*this);
        }
    }
}
