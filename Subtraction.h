#ifndef SUBTRACTION_H
#define SUBTRACTION_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Subtraction : public Expression {
        public:
            Subtraction(Expression* first, Expression* second);
            Subtraction(const Subtraction& that);
            ~Subtraction();

            Subtraction& operator=(const Subtraction& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

        private:
            Expression* _first;
            Expression* _second;
        };
    }
}

#endif
