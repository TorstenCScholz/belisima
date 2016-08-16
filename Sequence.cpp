#include "Sequence.h"
#include "Void.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Sequence::Sequence() {
        }

        Sequence::Sequence(const Sequence& that) {
            for (auto it = that._exprs.begin(); it != that._exprs.end(); ++it) {
                _exprs.push_back((*it)->clone());
            }
        }

        Sequence::~Sequence() {
            for (auto it = _exprs.begin(); it != _exprs.end(); ++it) {
                delete *it;
            }
        }

        Sequence& Sequence::operator=(const Sequence& that) {
            for (auto it = _exprs.begin(); it != _exprs.end(); ++it) {
                delete *it;
            }
            _exprs.clear();

            for (auto it = that._exprs.begin(); it != that._exprs.end(); ++it) {
                _exprs.push_back((*it)->clone());
            }

            return *this;
        }

        void Sequence::add(Expression* expr) {
            _exprs.push_back(expr);
        }
            
        Expression* Sequence::eval(Environment& env) {
            Expression* ret = nullptr;

            for (auto it = _exprs.begin(); it != _exprs.end(); ++it) {
                delete ret; // Delete prev.(!) expression
                ret = (*it)->eval(env);
            }

            if (_exprs.size() > 0) {
                return ret;
            }

            // No elements in sequence
            return new Void();
        }

        Type::Value Sequence::type() const {
            return Type::Operator;
        }

        std::string Sequence::toString() const {
            return "Op(Sequence)";
        }

        Expression* Sequence::clone() const {
            return new Sequence(*this);
        }
    }
}
