#ifndef FUNCTION_H
#define FUNCTION_H

#include "Procedure.h"
#include <vector>

namespace bel {
    namespace expr {
        class Function : public Procedure {
        public:
            Function(const std::string& name, const std::vector<std::string>& parameters, Expression* body);
            Function(const Function& that);
            ~Function();

            Function& operator=(const Function& that);
            
            virtual int formalsSize() const;
            virtual Expression* apply(const std::vector<Expression*>& arguments, Environment& env);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;
            
            static Expression* op_println;
            static Expression* op_defined;
            static Expression* op_type;
            static Expression* op_array;
            static Expression* op_len;

        private:
            std::string _name;
            std::vector<std::string> _params;
            Expression* _body;
            //Environment* _static_env;
        };
    }
}

#endif
