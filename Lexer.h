#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "NonCopyable.h"

namespace bel {
    namespace interpreter {
        class Token;
        
        class Lexer : public NonCopyable {
        public:
            // Constructs a lexer from the given input
            // If the input is a filename (is_file == true), the
            // content is read from that file, else it is taken
            // as given
            Lexer(const std::string& input, bool is_file = true);
            ~Lexer();
            
            // Scans the current input
            // Returns all found tokens
            std::vector<Token> scan();

        private:
            std::vector<char> _input;
            
            // Scans the next token
            // If there is no token left to be scanned, the method
            // returns token of type Token::EOFTOK
            Token scanNext(char*& pos, char* end, int& cur_line);

            // Scans for an identifier
            Token scanIdentifier(char*& pos, char* end, int& cur_line);

            // Scans for a number
            Token scanNumber(char*& pos, char* end, int& cur_line);

            // Scans for a string
            Token scanString(char*& pos, char* end, int& cur_line);

            // Scans for only digits and returns a string of those digits
            std::string scanDigits(char*& pos, char* end, int& cur_line);

            // Returns true, if ch is considered to be an atomic newline character
            static bool isNewline(char ch);

            // Returns true, of pos is the start of a (multi-)comment
            bool isComment(char* pos, char* end);

            // Scans for an atomic newline character
            Token scanNewline(char*& pos, char* end, int& cur_line);

            // Ignores whitespace characters (excluding newlines) and comments (including newlines)
            size_t ignore(char*& pos, char* end, int& cur_line);

            // Ignores a (multi-)comment
            size_t ignoreComment(char*& pos, char* end, int& cur_line);
        };
    }
}

#endif
