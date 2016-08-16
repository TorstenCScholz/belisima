#ifndef VOID_H
#define VOID_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Void : public Expression {
        public:
            Void();
            Void(const Void& that);
            ~Void();

            Void& operator=(const Void& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;
        };

        static Void VOID;
    }
}

#endif
