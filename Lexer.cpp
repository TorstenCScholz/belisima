#include "Lexer.h"

#include <fstream>
#include <iterator>
#include <exception>
#include "Token.h"
#include "LexerException.h"

namespace bel {
    namespace interpreter {
        Lexer::Lexer(const std::string& input, bool is_file) {
            if (is_file) {
                // Try to open file and read contents
    		    std::ifstream in(input.c_str());

                if (!in.is_open()) {
                    throw bel::exception::LexerException("Lexer::Lexer", std::string("Cannot open file named '") + input + "'.", 0);
                }

                // Copy content into _input
		        std::copy(std::istreambuf_iterator<char>(in.rdbuf()),
		            std::istreambuf_iterator<char>(),
		            std::back_inserter(_input));

                in.close();
            }
            else { // No file input => string content
                std::copy(input.begin(),
                    input.end(),
		            std::back_inserter(_input));
            }
        }

        Lexer::~Lexer() {
        }

        std::vector<Token> Lexer::scan() {
            std::vector<Token> tokens;

            char* pos = &_input.front();
            char* end = &_input.back() + 1;
            int cur_line = 1;

            while (pos < end) {
                tokens.push_back(scanNext(pos, end, cur_line));
            }

            return tokens;
        }

        Token Lexer::scanNext(char*& pos, char* end, int& cur_line) {
            ignore(pos, end, cur_line);

            if (pos == end)
                return Token(Tok::EOFTOK, cur_line);

            Token tok(Tok::NONE, 0);

            if (isalpha(*pos)) {
                tok = scanIdentifier(pos, end, cur_line);
            }
            else if (isdigit(*pos)) {
                tok = scanNumber(pos, end, cur_line);
            }
            else if (*pos == '\"') {
                tok = scanString(pos, end, cur_line);
            }
            else if (isNewline(*pos)) {
                tok = scanNewline(pos, end, cur_line);
            }
            else {
                switch (*pos) {
                case '(':
                    tok = Token(Tok::LEFT_PAR, cur_line, "(");
                    break;
                case ')':
                    tok = Token(Tok::RIGHT_PAR, cur_line, ")");
                    break;
                case '[':
                    tok = Token(Tok::LEFT_BRACKET, cur_line, "[");
                    break;
                case ']':
                    tok = Token(Tok::RIGHT_BRACKET, cur_line, "]");
                    break;
                case '{':
                    tok = Token(Tok::LEFT_BRACE, cur_line, "{");
                    break;
                case '}':
                    tok = Token(Tok::RIGHT_BRACE, cur_line, "}");
                    break;
                case ',':
                    tok = Token(Tok::COMMA, cur_line, ",");
                    break;
                case '.':
                    tok = Token(Tok::DOT, cur_line, ".");
                    break;
                case ':':
                    tok = Token(Tok::COLON, cur_line, ":");
                    break;
                case '+':
                    tok = Token(Tok::PLUS, cur_line, "+");
                    break;
                case '-':
                    pos++; // Look ahead
                    if (pos != end && *pos == '>') {
                        tok = tok = Token(Tok::ARROW, cur_line, "->");
                    }
                    else {
                        pos--; // Look back
                        tok = Token(Tok::MINUS, cur_line, "-");
                    }
                    break;
                case '*':
                    tok = Token(Tok::MULT, cur_line, "*");
                    break;
                case '/':
                    tok = Token(Tok::DIV, cur_line, "/");
                    break;
                case '=':
                    tok = Token(Tok::EQUAL, cur_line, "=");
                    break;
                case '<':
                    pos++; // Look ahead
                    if (pos != end && *pos == '=') {
                        tok = Token(Tok::LEQUAL, cur_line, "<=");
                    }
                    else {
                        pos--; // Look back
                        tok = Token(Tok::LESS, cur_line, "<");
                    }
                    break;
                case '>':
                    pos++; // Look ahead
                    if (pos != end && *pos == '=') {
                        tok = Token(Tok::GEQUAL, cur_line, ">=");
                    }
                    else {
                        pos--; // Look back
                        tok = Token(Tok::GREATER, cur_line, ">");
                    }
                }

                pos++;
            }


            return tok;
        }

        Token Lexer::scanIdentifier(char*& pos, char* end, int& cur_line) {
            std::string ident = "";

            while (pos != end && (isalnum(*pos) || *pos == '_')) {
                ident += *pos;
                pos++;
            }

            return Token(Tok::IDENT, cur_line, ident);
        }

        Token Lexer::scanNumber(char*& pos, char* end, int& cur_line) {
            std::string num = "";
            int divide_signs = 0;
            int dot_signs = 0;

            if (pos != end)  {
                num += scanDigits(pos, end, cur_line);

                if (pos != end) {
                    if (*pos == '/' || *pos == '.') {
                        num += *pos;
                        pos++;
                        num += scanDigits(pos, end, cur_line);
                    }
                }
            }

            return Token(Tok::NUMBER, cur_line, num);
        }

        Token Lexer::scanString(char*& pos, char* end, int& cur_line) {
            std::string value = "";

            // Does the string start with "?
            if (*pos != '"') {
                throw bel::exception::LexerException("Lexer::scanString", "String does not start with \".", cur_line);
            }

            pos++;

            while (pos != end) {
                if (*pos == '\"') {
                    pos++;
                    break; // We are done here
                }

                // Escaped character?
                if (*pos == '\\') {
                    pos++;
                    
                    if (*pos != *end) {
                        switch (*pos) {
                        case 't':
                            value += "\t";
                            break;
                        case 'n':
                            value += "\n";
                            break;
                        case 'r':
                            value += "\r";
                            break;
                        case '\\':
                            value += "\\";
                            break;
                        case '"':
                            value += "\"";
                            break;
                        default: // Ignore escaped character (TODO: warn user about this)
                            break;
                        }
                    }
                }
                else {
                    if (isNewline(*pos)) {
                        throw bel::exception::LexerException("Lexer::scanString", "Read a newline character while scanning a string.", cur_line);
                    }
                    value += *pos;
                }

                pos++;
            }

            return Token(Tok::STRING, cur_line, value);
        }

        std::string Lexer::scanDigits(char*& pos, char* end, int& cur_line) {
            std::string num = "";

            while (pos != end && isdigit(*pos))  {
                num += *pos;
                pos++;
            }

            return num;
        }

        bool Lexer::isNewline(char pos) {
            return pos == '\n' || pos == '\r';
        }

        bool Lexer::isComment(char* pos, char* end) {
            return (pos != end && *pos == '-' && (pos + 1) != end && *(pos + 1) == '-');
        }
        
        Token Lexer::scanNewline(char*& pos, char* end, int& cur_line) {
            if (!isNewline(*pos)) {
                throw bel::exception::LexerException("Lexer::scanNewline", std::string("Cannot scan for a newline, because '") + *pos + "' is not a newline character.", cur_line);
            }

            if (*pos == '\r' && (pos + 1) != end && *(pos + 1) == '\n') {
                pos++; // Newline with two chars => skip one
            }

            pos++; // Skip newline

            return Token(Tok::SEPARATOR, cur_line++, "\n");
        }

        size_t Lexer::ignore(char*& pos, char* end, int& cur_line) {
            char* beg = pos;

            while (pos != end) {
                // Ignore whitespace (excluding newlines)
                if (!isNewline(*pos) && isspace(*pos)) {
                    pos++; // Skip current character
                }
                else if (isComment(pos, end)) { // Ignore comments
                    ignoreComment(pos, end, cur_line);
                }
                else { // Nothing to ignore
                    break;
                }
            }

            return pos - beg;
        }

        size_t Lexer::ignoreComment(char*& pos, char* end, int& cur_line) {
            char* beg = pos;
            char* comment_pos = pos;

            if (pos != end && *pos == '-' && (pos + 1) != end && *(pos + 1) == '-') {
                // Possibly comment (single)
                comment_pos += 2; // Ignore both '-'

                if (comment_pos != end && *comment_pos == '[' && (comment_pos + 1) != end && *(comment_pos + 1) == '[') {
                    // Multiline-comment
                    comment_pos += 2; // ignore both [[

                    // Ignore everything up to ]]
                    //while (!(comment_pos != end && *comment_pos == ']' && (comment_pos + 1) != end && *(comment_pos + 1) == ']')) {
                        //if (comment_pos == end || (comment_pos + 1) == end)
                        //    break;
                    
                    while (true) {
                        if (comment_pos == end || (comment_pos + 1) == end) {
                            throw bel::exception::LexerException("Lexer::ignoreComment", "Multiline comment was not properly closed", cur_line);
                        }

                        if (!(*comment_pos == ']' && *(comment_pos + 1) == ']')) {
                            if (isNewline(*comment_pos)) {
                                scanNewline(comment_pos, end, cur_line); // To count the line number
                            }
                            else {
                                comment_pos++; // Ignore character in comment
                            }
                        }
                        else {
                            comment_pos += 2; // Ignore both ]]
                            break;
                        }
                    }
                }
                else {
                    // Singleline-comment
                    while (comment_pos != end && !isNewline(*comment_pos)) {
                        comment_pos++;
                    }

                    scanNewline(comment_pos, end, cur_line); // Because we reached the newline
                }

                pos = comment_pos; // Set position to end of comment
            }

            return pos - beg;
        }
    }
}
