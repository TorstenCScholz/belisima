#include "IfStatement.h"
#include <cassert>
#include "Boolean.h"
#include "Void.h"

namespace bel {
    namespace expr {
        IfStatement::IfStatement(Expression* comparison, Expression* if_body, Expression* else_body) : _cmp(comparison), _if_body(if_body), _else_body(else_body) {
            assert(_cmp != nullptr);
            assert(_if_body != nullptr);
        }

        IfStatement::IfStatement(const IfStatement& that) : _cmp(that._cmp->clone()), _if_body(that._if_body->clone()) {
            _else_body = nullptr;

            if (that._else_body != nullptr) {
                _else_body = that._else_body->clone();
            }
        }

        IfStatement::~IfStatement() {
            delete _cmp;
            delete _if_body;
            delete _else_body;
        }

        IfStatement& IfStatement::operator=(const IfStatement& that) {
            delete _cmp;
            delete _if_body;
            delete _else_body;

            _cmp = that._cmp->clone();
            _if_body = that._if_body->clone();

            if (that._else_body != nullptr) {
                _else_body = that._else_body->clone();
            }

            return *this;
        }

        bel::expr::Expression* IfStatement::eval(Environment& env) {
            Boolean* comp = dynamic_cast<Boolean*>(_cmp->eval(env));

            if (comp->value()) {
                delete comp;
                return _if_body->eval(env);
            }
            else if (_else_body != nullptr) {
                delete comp;
                return _else_body->eval(env);
            }

            return new Void();
        }

        Type::Value IfStatement::type() const {
            return Type::Operator;
        }

        std::string IfStatement::toString() const {
            return "Op(If)";
        }

        Expression* IfStatement::clone() const {
            return new IfStatement(*this);
        }
    }
}
