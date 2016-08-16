#ifndef ADDITION_H
#define ADDITION_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Addition : public Expression {
        public:
            Addition(Expression* first, Expression* second);
            Addition(const Addition& that);
            ~Addition();

            Addition& operator=(const Addition& that);

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
