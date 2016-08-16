#ifndef LEXEREXCEPTION_H
#define LEXEREXCEPTION_H

#include "Exception.h"

#include <sstream>

namespace bel {
    namespace exception {
        // Is to be throw, when there is a problem with(in) the lexer
        class LexerException : public Exception {
        public:
            LexerException(const std::string& function_name, const std::string& msg, int line);
            LexerException(const LexerException& that);
            ~LexerException();

            LexerException& operator=(const LexerException& that);

        private:
            std::string _function_name;
            int _line;

            // Transforms the input arguments (constructor) into a formatted message
            void makeMessage();
        };
    }
}

#endif
