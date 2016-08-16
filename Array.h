#ifndef ARRAY_H
#define ARRAY_H

#include "Expression.h"
#include <vector>

namespace bel {
    namespace expr {
        class Array : public Expression {
        public:
            Array(size_t len);
            Array(const Array& that);
            ~Array();

            Array& operator=(const Array& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

            Expression* get(const std::vector<size_t>& pos) const;
            void set(const std::vector<size_t>& pos, Expression* data);

            size_t length() const;

        private:
            std::vector<Expression*> _content;
        };
    }
}

#endif
