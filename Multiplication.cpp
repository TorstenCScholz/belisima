#include "Multiplication.h"
#include <cassert>
#include "Number.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Multiplication::Multiplication(Expression* first, Expression* second) : _first(first), _second(second) {
            assert(first != nullptr);
            assert(second != nullptr);
        }

        Multiplication::Multiplication(const Multiplication& that) : _first(that._first->clone()), _second(that._second->clone()) {
        }

        Multiplication::~Multiplication() {
            delete _first;
            delete _second;
        }

        Multiplication& Multiplication::operator=(const Multiplication& that) {
            delete _first;
            delete _second;

            _first = that._first->clone();
            _second = that._second->clone();

            return *this;
        }

        Expression* Multiplication::eval(Environment& env) {
            Number* left = dynamic_cast<Number*>(_first->eval(env));
            Number* right = dynamic_cast<Number*>(_second->eval(env));

            if (left == nullptr || right == nullptr) {
                throw bel::expr::Panic("MULTIPLICATION", "Cannot apply multiplication to operands, because one of them is not a number.");
            }

            Number* ret = new Number(*left * *right);
            delete left;
            delete right;

            return ret;
        }

        Type::Value Multiplication::type() const {
            return Type::Operator;
        }

        std::string Multiplication::toString() const {
            return "Op(Multiplication)";
        }

        Expression* Multiplication::clone() const {
            return new Multiplication(*this);
        }
    }
}
