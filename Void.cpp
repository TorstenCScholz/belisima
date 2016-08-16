#include "Void.h"

namespace bel {
    namespace expr {
        Void::Void() {
        }

        Void::Void(const Void& that) {
        }

        Void::~Void() {}
        
        Void& Void::operator=(const Void& that) {
            return *this;
        }

        Expression* Void::eval(Environment& env) {
            return new Void();
        }

        Type::Value Void::type() const {
            return Type::Void;
        }

        std::string Void::toString() const {
            return "";
        }

        Expression* Void::clone() const {
            return new Void(*this);
        }
    }
}
