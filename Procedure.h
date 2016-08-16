#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Expression.h"
#include <vector>

namespace bel {
    namespace expr {
        class Procedure : public Expression {
        public:
            Procedure() {}
            virtual ~Procedure() = 0 {}
            
            virtual int formalsSize() const = 0;
            virtual Expression* apply(const std::vector<Expression*>& arguments, Environment& env) = 0;
        };
    }
}

#endif
