#include "Subtraction.h"
#include <cassert>
#include "Number.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Subtraction::Subtraction(Expression* first, Expression* second) : _first(first), _second(second) {
            assert(first != nullptr);
            assert(second != nullptr);
        }

        Subtraction::Subtraction(const Subtraction& that) : _first(that._first->clone()), _second(that._second->clone()) {
        }

        Subtraction::~Subtraction() {
            delete _first;
            delete _second;
        }

        Subtraction& Subtraction::operator=(const Subtraction& that) {
            delete _first;
            delete _second;

            _first = that._first->clone();
            _second = that._second->clone();

            return *this;
        }

        Expression* Subtraction::eval(Environment& env) {
            Number* left = dynamic_cast<Number*>(_first->eval(env));
            Number* right = dynamic_cast<Number*>(_second->eval(env));

            if (left == nullptr || right == nullptr) {
                throw bel::expr::Panic("SUBTRACTION", "Cannot apply subtraction to operands, because one of them is not a number.");
            }

            Number* ret = new Number(*left - *right);
            delete left;
            delete right;

            return ret;
        }

        Type::Value Subtraction::type() const {
            return Type::Operator;
        }

        std::string Subtraction::toString() const {
            return "Op(Subtraction)";
        }

        Expression* Subtraction::clone() const {
            return new Subtraction(*this);
        }
    }
}
