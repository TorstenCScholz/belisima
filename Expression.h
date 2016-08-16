#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "InterpreterException.h"

namespace bel {
    namespace expr {
        
        class Array;
        class Class;
        class Environment;
        class Function;
        class Number;
        class String;
        class Symbol;
        class Void;
        class Panic;

        namespace Type {
            enum Value {
                None = 0,
                Array,
                Boolean,
                Class,
                Environment,
                Function,
                Operator,
                Reference,
                Number,
                String,
                Symbol,
                Void,
                Panic,
            };
        }

        std::string typeToString(Type::Value type);

        class Expression {
        public:
            Expression() {}
            virtual ~Expression() = 0 {}
            
            virtual Expression* eval(Environment& env) = 0;
            virtual Type::Value type() const = 0;
            virtual std::string toString() const = 0;

            virtual Expression* clone() const {
                throw bel::exception::InterpreterException("Expression::clone", "Cannot clone expression of type '" + typeToString(type()) + "'.");
            }
        };
    }
}

#endif
