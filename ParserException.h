#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include "Exception.h"

#include <sstream>

namespace bel {
    namespace exception {
        // Is to be throw, when there is a problem with(in) the parser
        class ParserException : public Exception {
        public:
            ParserException(const std::string& function_name, const std::string& msg, int line);
            ParserException(const ParserException& that);
            ~ParserException();

            ParserException& operator=(const ParserException& that);

        private:
            std::string _function_name;
            int _line;

            // Transforms the input arguments (constructor) into a formatted message
            void makeMessage();
        };
    }
}

#endif
