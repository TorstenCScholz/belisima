#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace bel{
    namespace interpreter {
        namespace Tok {
            enum Type {
                EOFTOK = 0,
                NONE,

                SEPARATOR,

                NUMBER,
                STRING,
                IDENT,

                LEFT_PAR,
                RIGHT_PAR,
                LEFT_BRACKET,
                RIGHT_BRACKET,
                LEFT_BRACE,
                RIGHT_BRACE,

                COMMA,
                DOT,
                COLON,

                PLUS,
                MINUS,
                MULT,
                DIV,

                EQUAL,
                LESS,
                GREATER,
                LEQUAL,
                GEQUAL,
                ARROW,
            };
        }

        class AdditionalData {
        public:
            virtual ~AdditionalData() = 0 {};
            virtual AdditionalData* clone() const = 0;
        };

        class SeparatorData : public AdditionalData {
        public:
            SeparatorData(size_t count) : _count(count) {}
            SeparatorData(const SeparatorData& that) : _count(that._count) {}
            ~SeparatorData() {}

            SeparatorData& operator=(const SeparatorData& that) {
                _count = that._count;

                return *this;
            }

            AdditionalData* clone() const { return new SeparatorData(*this); }

            size_t count() const { return _count; }
            void count(size_t count) { _count = count; }

        private:
            size_t _count;
        };

        class Token {
        public:
            Token(Tok::Type type, int line, const std::string& value = "");
            Token(const Token& that);
            ~Token();

            Token& operator=(Token& that);

            Tok::Type type() const { return _type; }
            int line() const { return _line; }
            std::string value() const { return _value; }
            std::string name() const { return _name; }

            static std::string nameForType(Tok::Type type);

            AdditionalData* additionalData() const { return _add; }
            void additionalData(AdditionalData* data) { _add = data; }

        private:
            Tok::Type _type;
            int _line;
            std::string _name;
            std::string _value;

            AdditionalData* _add;
        };
    }
}

#endif
