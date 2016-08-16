#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "Expression.h"
#include <vector>

namespace bel {
    namespace expr {
        class Assignment : public Expression {
        public:
            Assignment(const std::string& var_name, Expression* assignment);
            Assignment(const std::string& var_name, const std::vector<Expression*>& arguments, Expression* assignment);
            Assignment(const Assignment& that);
            ~Assignment();

            Assignment& operator=(const Assignment& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;
            
            Expression* clone() const;

        private:
            std::string _var_name;
            std::vector<Expression*> _args;
            Expression* _assignment;
        };
    }
}

#endif
