#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Boolean : public Expression {
        public:
            Boolean();
            Boolean(bool val);
            Boolean(const Boolean& that);
            ~Boolean();

            Boolean& operator=(const Boolean& that);

            bool value() const;

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

        private:
            bool _val;
        };
    }
}

#endif
