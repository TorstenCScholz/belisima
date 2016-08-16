#ifndef PROCCALL_H
#define PROCCALL_H

#include "Expression.h"
#include <vector>

namespace bel {
    namespace expr {
        class ProcCall : public Expression {
        public:
            ProcCall(const std::string& proc_name, std::vector<Expression*> arguments);
            ProcCall(const ProcCall& that);
            ~ProcCall();

            ProcCall& operator=(const ProcCall& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

            std::string name() const;
            std::vector<Expression*> arguments() const;

        private:
            std::string _proc_name;
            std::vector<Expression*> _args;
        };
    }
}

#endif
