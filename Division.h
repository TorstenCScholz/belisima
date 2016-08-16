#ifndef DIVISION_H
#define DIVISION_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Division : public Expression {
        public:
            Division(Expression* first, Expression* second);
            Division(const Division& that);
            ~Division();

            Division& operator=(const Division& that);

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
