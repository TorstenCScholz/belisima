#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Multiplication : public Expression {
        public:
            Multiplication(Expression* first, Expression* second);
            Multiplication(const Multiplication& that);
            ~Multiplication();

            Multiplication& operator=(const Multiplication& that);

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
