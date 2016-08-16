#ifndef BLOCK_H
#define BLOCK_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class Block : public Expression {
        public:
            Block(Expression* content);
            Block(const Block& that);
            ~Block();

            Block& operator=(const Block& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;

        private:
            Expression* _cont;
        };
    }
}

#endif
