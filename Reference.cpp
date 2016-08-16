#include "Reference.h"
#include <iostream>

namespace bel {
    namespace expr {
        Reference::Reference(Expression* expr) : _count(new size_t(1)), _expr(expr) {
        }

        Reference::Reference(const Reference& that) : _count(that._count), _expr(that._expr) {
            (*_count)++;
        }

        Reference::~Reference() {
            (*_count)--;

            if (*_count == 0) {
                delete _count;
                delete _expr;
            }
        }

        Reference& Reference::operator=(const Reference& that) {
            (*_count)--;

            if (*_count == 0) {
                delete _count;
                delete _expr;
            }

            _count = that._count;
            _expr = that._expr;

            (*_count)++;

            return *this;
        }

        Expression* Reference::eval(Environment& env) {
            return clone();
        }

        Type::Value Reference::type() const {
            return Type::Reference;
        }

        std::string Reference::toString() const {
            return "REFERENCE"; // TODO
        }

        Expression* Reference::clone() const {
            return new Reference(*this);
        }

        Expression* Reference::deref() const {
            return _expr;
        }
    }
}
