#include "Addition.h"
#include <cassert>
#include "Number.h"
#include "Panic.h"
#include "String.h"
#include "Boolean.h"
#include <sstream>

namespace bel {
    namespace expr {
        Addition::Addition(Expression* first, Expression* second) : _first(first), _second(second) {
            assert(first != nullptr);
            assert(second != nullptr);
        }

        Addition::Addition(const Addition& that) : _first(that._first->clone()), _second(that._second->clone()) {
        }

        Addition::~Addition() {
            delete _first;
            delete _second;
        }

        Addition& Addition::operator=(const Addition& that) {
            delete _first;
            delete _second;

            _first = that._first->clone();
            _second = that._second->clone();

            return *this;
        }

        Expression* Addition::eval(Environment& env) {
            Expression* left = _first->eval(env);
            Expression* right = _second->eval(env);
            
            if (left->type() != Type::Number && left->type() != Type::String) {
                throw bel::expr::Panic("ADDITION", "Left operand does not support addition.");
            }

            if (right->type() != Type::Number && right->type() != Type::String) {
                throw bel::expr::Panic("ADDITION", "Right operand does not support addition.");
            }

            if (left->type() == Type::Number && right->type() == Type::Number) {
                Number* ret = new Number(*dynamic_cast<Number*>(left) + *dynamic_cast<Number*>(right));
                delete left;
                delete right;
                
                return ret;
            }
            
            // One operand has to be of type string
            std::stringstream ss;
            ss << left->toString() << right->toString();

            String* ret = new String(ss.str());
            delete left;
            delete right;

            return ret;
        }

        Type::Value Addition::type() const {
            return Type::Operator;
        }

        std::string Addition::toString() const {
            return "Op(Addition)";
        }

        Expression* Addition::clone() const {
            return new Addition(*this);
        }
    }
}
