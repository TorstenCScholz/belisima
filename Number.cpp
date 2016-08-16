#include "Number.h"

#include <sstream>
#include "Panic.h"

namespace bel {
    namespace expr {
        const Number Number::Zero(0);
        const Number Number::One(1);
        const Number Number::MinusOne(-1);

        Number::Number(long num, long den) {
            mpz_init(_num);
            mpz_init(_den);

            mpz_set_si(_num, num);
            mpz_set_si(_den, den);

            reduce();
        }

        Number::Number(const mpz_t& num, const mpz_t& den) {
            mpz_init(_num);
            mpz_init(_den);
            
            mpz_set(_num, num);
            mpz_set(_den, den);

            reduce();
        }
        
        Number::Number(const std::string& num, const std::string& den) {
            mpz_init_set_str(_num, num.c_str(), 10);
            mpz_init_set_str(_den, den.c_str(), 10);
            
            reduce();
        }

        Number::Number(const std::string& number) {
            if (number.find('.') != std::string::npos && number.find('/') != std::string::npos) {
                throw Panic("NUMBER", "Decimal numbers with rational part are not supported.");
            }

            // Decimal number
            if (number.find('.') != std::string::npos) {
                size_t pos = number.find('.');

                // Get numerator without the dot
                std::string num;
                for (size_t i = 0; i < number.length(); ++i) {
                    if (number[i] != '.') {
                        num += number[i];
                    }
                }

                // Denominator
                std::string den = "1";
                for (size_t i = 0; i < number.length() - pos - 1; ++i) {
                    den += "0";
                }

                *this = Number(num, den);
            }
            else {
                // Rational number
                size_t divPos = 0;
                if ((divPos = number.find('/')) != std::string::npos) {
                    *this = Number(number.substr(0, divPos), number.substr(divPos + 1));
                }
                else {
                    *this = Number(number, "1");
                }
            }
        }

        Number::Number(const Number& that) {
            mpz_init(_num);
            mpz_init(_den);

            mpz_set(_num, that._num);
            mpz_set(_den, that._den);
        }

        Number::~Number() {
            mpz_clear(_num);
            mpz_clear(_den);
        }
        
        Number& Number::operator=(const Number& that) {
            mpz_init(_num);
            mpz_init(_den);

            mpz_set(_num, that._num);
            mpz_set(_den, that._den);

            return *this;
        }
        
        Number Number::numerator() const {
            return Number(_num, One._num);
        }

        Number Number::denomerator() const {
            return Number(_den, One._num);
        }

        Number Number::operator+(const Number& that) const {
            mpz_t num1;
            mpz_t num2;
            mpz_t den;
            
            mpz_init(num1);
            mpz_init(num2);
            mpz_init(den);
    
            mpz_mul(den, _den, that._den);
    
            mpz_mul(num1, _num, that._den);
            mpz_mul(num2, that._num, _den);
    
            mpz_add(num1, num1, num2);
            
            Number n(num1, den);
            
            mpz_clear(num1);
            mpz_clear(num2);
            mpz_clear(den);

            return n.reduce();
        }

        Number Number::operator-(const Number& that) const {
            mpz_t num1;
            mpz_t num2;
            mpz_t den;
            
            mpz_init(num1);
            mpz_init(num2);
            mpz_init(den);
    
            mpz_mul(den, _den, that._den);
    
            mpz_mul(num1, _num, that._den);
            mpz_mul(num2, that._num, _den);
    
            mpz_sub(num1, num1, num2);
            
            Number n(num1, den);
            
            mpz_clear(num1);
            mpz_clear(num2);
            mpz_clear(den);

            return n.reduce();
        }

        Number Number::operator*(const Number& that) const {
            mpz_t num;
            mpz_t den;
            
            mpz_init(num);
            mpz_init(den);
            
            mpz_mul(num, _num, that._num);
            mpz_mul(den, _den, that._den);

            Number n(num, den);
            
            mpz_clear(num);
            mpz_clear(den);

            return n.reduce();
        }

        Number Number::operator/(const Number& that) const {
            if (that == Zero)
                throw Panic("DIVISION", "Division by 0.");

            mpz_t num;
            mpz_t den;
            
            mpz_init(num);
            mpz_init(den);
            
            mpz_mul(num, _num, that._den);
            mpz_mul(den, _den, that._num);
            
            Number n(num, den);
            
            mpz_clear(num);
            mpz_clear(den);

            return n.reduce();
        }

        bool Number::operator<(const Number& that) const {
            mpz_t num1;
            mpz_t num2;
            
            mpz_init(num1);
            mpz_init(num2);
            
            mpz_mul(num1, _num, that._den);
            mpz_mul(num2, that._num, _den);

            bool smaller = (mpz_cmp(num1, num2) < 0);
            
            mpz_clear(num1);
            mpz_clear(num2);

            return smaller;
        }

        bool Number::operator<=(const Number& that) const {
            return (*this < that) || (*this == that);
        }

        bool Number::operator>(const Number& that) const {
            return !(*this < that) && *this != that;
        }

        bool Number::operator>=(const Number& that) const {
            return !(*this < that);
        }
        
        bool Number::operator==(const Number& that) const {
            return  (mpz_cmp(_num, that._num) == 0) &&
                    (mpz_cmp(_den, that._den) == 0);
        }

        bool Number::operator!=(const Number& that) const {
            return !(*this == that);
        }

        Expression* Number::eval(Environment& env) {
            return new Number(*this);
        }

        Type::Value Number::type() const {
            return Type::Number;
        }

        std::string Number::toString() const {
            std::stringstream str;

            if (*this == Zero) {
                return std::string("0");
            }
            if (mpz_cmp(_den, One._num) == 0) {
                char* buf = new char[mpz_sizeinbase(_num, 10) + 2];

                mpz_get_str(buf, 10, _num);
                str << buf;

                delete[] buf;
            }
            else {
                char* buf1 = new char[mpz_sizeinbase(_num, 10) + 2];
                char* buf2 = new char[mpz_sizeinbase(_den, 10) + 2];
            
                mpz_get_str(buf1, 10, _num);
                mpz_get_str(buf2, 10, _den);

                str << buf1 << "/" << buf2;

                delete[] buf1;
                delete[] buf2;
            }

            return str.str();
        }

        
        Expression* Number::clone() const {
            return new Number(*this);
        }

        Number& Number::reduce() {
            mpz_t gcd;
            mpz_init(gcd);

            mpz_gcd(gcd, _num, _den);

            if (mpz_cmp(gcd, One._num) != 0) {
                mpz_div(_num, _num, gcd);
                mpz_div(_den, _den, gcd);
            }

            mpz_clear(gcd);

            return *this;
        }
    }
}
