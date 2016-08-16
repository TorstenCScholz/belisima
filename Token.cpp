#include "Token.h"

namespace bel {
    namespace interpreter {
        Token::Token(Tok::Type type, int line, const std::string& value) : _type(type), _line(line), _value(value), _add(nullptr) {
            _name = nameForType(type);
        }

        Token::Token(const Token& that) : _type(that._type), _line(that._line), _value(that._value), _add(nullptr) {
            _name = that._name;

            if (that._add != nullptr) {
                _add = that._add->clone();
            }
        }

        Token::~Token() {
            delete _add;
        }

        Token& Token::operator=(Token& that) {
            delete _add;
            _add = nullptr;

            _type = that._type;
            _line = that._line;
            _value = that._value;
            _name = that._name;
            
            if (that._add != nullptr) {
                _add = that._add->clone();
            }

            return *this;
        }

        std::string Token::nameForType(Tok::Type type) {
            switch (type) {
            case Tok::EOFTOK:
                return "End of File";
            case Tok::NONE:
                return "None";
            case Tok::SEPARATOR:
                return "Separator";
            case Tok::NUMBER:
                return "Number";
            case Tok::STRING:
                return "String";
            case Tok::IDENT:
                return "Identifier";
            case Tok::LEFT_PAR:
                return "Left Parenthesis";
            case Tok::RIGHT_PAR:
                return "Right Parenthesis";
            case Tok::LEFT_BRACKET:
                return "Left Bracket";
            case Tok::RIGHT_BRACKET:
                return "Right Bracket";
            case Tok::LEFT_BRACE:
                return "Left Brace";
            case Tok::RIGHT_BRACE:
                return "Right Brace";
            case Tok::COMMA:
                return "Comma";
            case Tok::DOT:
                return "Dot";
            case Tok::COLON:
                return "Colon";
            case Tok::PLUS:
                return "Plus";
            case Tok::MINUS:
                return "Minus";
            case Tok::MULT:
                return "Multiply";
            case Tok::DIV:
                return "Divide";
            case Tok::EQUAL:
                return "Equal";
            case Tok::LESS:
                return "Less";
            case Tok::GREATER:
                return "Greater";
            case Tok::LEQUAL:
                return "Less or equal";
            case Tok::GEQUAL:
                return "Greater or equal";
            case Tok::ARROW:
                return "Arrow";
            default:
                return "Unknown";
            }
        }
    }
}
