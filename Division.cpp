#include "Division.h"
#include <cassert>
#include "Number.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Division::Division(Expression* first, Expression* second) : _first(first), _second(second) {
            assert(first != nullptr);
            assert(second != nullptr);
        }

        Division::Division(const Division& that) : _first(that._first->clone()), _second(that._second->clone()) {
        }

        Division::~Division() {
            delete _first;
            delete _second;
        }

        Division& Division::operator=(const Division& that) {
            delete _first;
            delete _second;

            _first = that._first->clone();
            _second = that._second->clone();

            return *this;
        }

        Expression* Division::eval(Environment& env) {
            Number* left = dynamic_cast<Number*>(_first->eval(env));
            Number* right = dynamic_cast<Number*>(_second->eval(env));

            if (left == nullptr || right == nullptr) {
                throw bel::expr::Panic("DIVISION", "Cannot apply multiplication to operands, because one of them is not a number.");
            }

            Number* ret = new Number(*left / *right);
            delete left;
            delete right;

            return ret;
        }

        Type::Value Division::type() const {
            return Type::Operator;
        }

        std::string Division::toString() const {
            return "Op(Division)";
        }

        Expression* Division::clone() const {
            return new Division(*this);
        }
    }
}
