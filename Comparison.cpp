#include "Comparison.h"
#include <cassert>
#include "Number.h"
#include "Boolean.h"
#include "String.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Comparison::Comparison(Expression* left, Expression* right) : _left(left), _right(right) {
            assert(_left != nullptr);
            assert(_right != nullptr);
        }

        Comparison::Comparison(const Comparison& that) : _left(that._left->clone()), _right(that._right->clone()) {
        }

        Comparison::~Comparison() {
            delete _left;
            delete _right;
        }

        Comparison& Comparison::operator=(const Comparison& that) {
            delete _left;
            delete _right;

            _left = that._left->clone();
            _right = that._right->clone();

            return *this;
        }

        bel::expr::Expression* Comparison::eval(Environment& env) {
            Number* n1 = dynamic_cast<Number*>(_left->eval(env));
            Number* n2 = dynamic_cast<Number*>(_right->eval(env));

            if (n1 != nullptr && n2 != nullptr) {
                Expression* ret = new Boolean(*n1 == *n2);

                delete n1;
                delete n2;

                return ret;
            }
            else {
                delete n1;
                delete n2;
                
                Boolean* b1 = dynamic_cast<Boolean*>(_left->eval(env));
                Boolean* b2 = dynamic_cast<Boolean*>(_right->eval(env));

                if (b1 != nullptr && b2 != nullptr) {
                    Expression* ret = new Boolean(b1->value() == b2->value());

                    delete b1;
                    delete b2;

                    return ret;
                }
                else {
                    delete b1;
                    delete b2;
                    
                    String* s1 = dynamic_cast<String*>(_left->eval(env));
                    String* s2 = dynamic_cast<String*>(_right->eval(env));

                    if (s1 != nullptr && s2 != nullptr) {
                        Expression* ret = new Boolean(s1->toString() == s2->toString());

                        delete s1;
                        delete s2;

                        return ret;
                    }
                    else {
                        delete s1;
                        delete s2;

                        throw bel::expr::Panic("COMPARISON", "Cannot compare expressions, because they do not type match.");
                    }
                }
            }
        }

        Type::Value Comparison::type() const {
            return Type::Operator;
        }

        std::string Comparison::toString() const {
            return "Op(Comparison)";
        }

        Expression* Comparison::clone() const {
            return new Comparison(*this);
        }
    }
}
