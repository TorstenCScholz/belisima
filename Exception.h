#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace bel {
    namespace exception {
        class Exception : public std::exception {
        public:
            explicit Exception(const std::string& msg);
            Exception(const Exception& that);
            virtual ~Exception() = 0;

            virtual Exception& operator=(const Exception& that);

            virtual const char* what() const { return _msg.c_str(); }

        protected:
            std::string _msg;
        };
    }
}

#endif
