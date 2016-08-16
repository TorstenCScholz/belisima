#ifndef PANIC_H
#define PANIC_H

#include <string>
#include "Expression.h"

namespace bel {
    namespace expr {
        class Panic : public Expression {
        public:
            Panic(const std::string& name, const std::string& message) : _name(name), _message(message) {
            }
            Panic(const Panic& that) : _name(that._name), _message(that._message) {
            }
            ~Panic() {}

            Panic& operator=(const Panic& that) {
                _name = that._name;
                _message = that._message;

                return *this;
            }

            std::string name() const { return _name; }
            std::string message() const { return _message; }

            Expression* eval(Environment& env) { return clone(); }
            Type::Value type() const { return Type::Panic; }
            std::string toString() const { return std::string("Panic: ") + message(); }

            Expression* clone() const { return new Panic(*this); }

        private:
            std::string _name;
            std::string _message;
        };
    }
}

#endif
