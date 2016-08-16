#ifndef NUMBER_H
#define NUMBER_H

#include <gmp.h>
#include "Expression.h"

namespace bel {
    namespace expr {
        class Number : public Expression {
        public:
            Number(long num, long den = 1);
            Number(const mpz_t& num, const mpz_t& den);
            Number(const std::string& num, const std::string& den);
            Number(const std::string& number);
            Number(const Number& that);
            ~Number();

            Number& operator=(const Number& that);

            Number numerator() const;
            Number denomerator() const;
            
            Number operator+(const Number& that) const;
            Number operator-(const Number& that) const;
            Number operator*(const Number& that) const;
            Number operator/(const Number& that) const;
            
            bool operator<(const Number& that) const;
            bool operator<=(const Number& that) const;
            bool operator>(const Number& that) const;
            bool operator>=(const Number& that) const;
            bool operator==(const Number& that) const;
            bool operator!=(const Number& that) const;
            
            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Expression* clone() const;
            
            static const Number Zero;
            static const Number One;
            static const Number MinusOne;

        private:
            mpz_t _num;
            mpz_t _den;

            Number& reduce();
        };
    }
}

#endif
