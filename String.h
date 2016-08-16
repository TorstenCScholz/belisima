#ifndef STRING_H
#define STRING_H

#include "Expression.h"

namespace bel {
    namespace expr {
        class String : public Expression {
        public:
            String(const std::string& content);
            String(const String& that);
            ~String();

            String& operator=(const String& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;
            
            size_t length() const;

        private:
            std::string _cont;
        };
    }
}

#endif
