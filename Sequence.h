#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Expression.h"
#include <vector>

namespace bel {
    namespace expr {
        class Sequence : public Expression {
        public:
            Sequence();
            Sequence(const Sequence& that);
            ~Sequence();

            Sequence& operator=(const Sequence& that);

            void add(Expression* expr);
            
            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

        private:
            std::vector<Expression*> _exprs;
        };
    }
}

#endif
