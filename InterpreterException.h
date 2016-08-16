#ifndef INTERPRETEREXCEPTION_H
#define INTERPRETEREXCEPTION_H

#include "Exception.h"

namespace bel {
    namespace exception {
        // Is to be throw, when there is a problem with(in) the lexer
        class InterpreterException : public Exception {
        public:
            InterpreterException(const std::string& function_name, const std::string& msg);
            InterpreterException(const InterpreterException& that);
            ~InterpreterException();

            InterpreterException& operator=(const InterpreterException& that);

        private:
            std::string _function_name;

            // Transforms the input arguments (constructor) into a formatted message
            void makeMessage();
        };
    }
}

#endif
