#ifndef COMPARISON_H
#define COMPARISON_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Comparison : public Expression {
        public:
            Comparison(Expression* left, Expression* right);
            Comparison(bool val);
            Comparison(const Comparison& that);
            ~Comparison();

            Comparison& operator=(const Comparison& that);

            bool value() const;

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

        private:
            Expression* _left;
            Expression* _right;
        };
    }
}

#endif
