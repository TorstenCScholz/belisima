#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class IfStatement : public Expression {
        public:
            IfStatement(Expression* comparison, Expression* if_body, Expression* else_body = nullptr);
            IfStatement(const IfStatement& that);
            ~IfStatement();

            IfStatement& operator=(const IfStatement& that);

            bel::expr::Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

        private:
            Expression* _cmp;
            Expression* _if_body;
            Expression* _else_body;
        };
    }
}

#endif
