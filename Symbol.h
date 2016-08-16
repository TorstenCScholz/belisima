#ifndef SYMBOL_H
#define SYMBOL_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Symbol : public Expression {
        public:
            Symbol(const std::string& name);
            Symbol(const Symbol& that);
            ~Symbol();

            Symbol& operator=(const Symbol& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            std::string name() const;

            Expression* clone() const;

        private:
            std::string _name;
        };
    }
}

#endif
