#ifndef DECLARATION_H
#define DECLARATION_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Declaration : public Expression {
        public:
            Declaration(const std::string& var_name);
            Declaration(const Declaration& that);
            ~Declaration();

            Declaration& operator=(const Declaration& that);

            std::string symbolName() const;

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;
            
            Expression* clone() const;

        private:
            std::string _var_name;
        };
    }
}

#endif
