#ifndef REFERENCE_H
#define REFERENCE_H

#include "Expression.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        class Reference : public Expression {
        public:
            Reference(Expression* expr);
            Reference(const Reference& that);
            ~Reference();

            Reference& operator=(const Reference& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

            Expression* deref() const;

            template<class T>
            static T* cast(Expression* expr) {
                Reference* ref = dynamic_cast<Reference*>(expr);

                if (ref == nullptr) {
                    throw bel::expr::Panic("REFERENCE", "Expression is not a reference.");
                }

                return dynamic_cast<T*>(ref->_expr);
            }

        private:
            size_t* _count;
            Expression* _expr;
        };
    }
}

#endif
