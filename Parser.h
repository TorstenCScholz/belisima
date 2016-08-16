#ifndef PARSER_H
#define PARSER_H

#include "NonCopyable.h"
#include <vector>
#include "GuardedPosition.h"

namespace bel {
    namespace expr {
        class Expression;
    }

    namespace interpreter {
        class Token;

        class Parser : public NonCopyable {
        public:
            // Constructs a parser from the given input
            // If the input is a filename (is_file == true), the
            // content is read from that file, else it is taken
            // as given
            Parser(const std::vector<Token>& input);
            ~Parser();

            // Parses the input and returns a program ready to be evaluated (executed)
            bel::expr::Expression* parse();
            typedef GuardedPosition<Token> GuardedToken;

        private:
            std::vector<Token> _input;

            // Trys to parse an expression
            bel::expr::Expression* parseExpression(GuardedToken& pos, int& cur_line);

            // Parses a block
            bel::expr::Expression* parseBlock(GuardedToken& pos, int& cur_line);

            // Parses a sequence
            bel::expr::Expression* parseSequence(GuardedToken& pos, int& cur_line);

            // Parses a declaration / definition
            bel::expr::Expression* parseDecl(GuardedToken& pos, int& cur_line);

            // Parses a function expression
            bel::expr::Expression* parseFunction(GuardedToken& pos, int& cur_line);
            
            // Parses a procedure call
            bel::expr::Expression* parseProcCall(GuardedToken& pos, int& cur_line);

            // Parses a list
            std::vector<bel::expr::Expression*> parseList(GuardedToken& pos, int& cur_line);
            
            // Parses an if-call
            bel::expr::Expression* parseIf(GuardedToken& pos, int& cur_line);

            // Parses an comparison
            bel::expr::Expression* parseComparison(GuardedToken& pos, int& cur_line);

            // Parses an assignment
            bel::expr::Expression* parseAssignment(GuardedToken& pos, int& cur_line);
            
            // Parses a math expression
            bool parseMath(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);
            
            // Parses an atom (i. e. number, string, procedure call etc.)
            // TODO: Obsolete because of parseExpression?
            //bool parseAtom(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);

            // Parses a term
            bool parseTerm(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);

            // Parses a factor
            bool parseFactor(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);

            // Trys to parse a number
            bool parseNumber(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);
            
            // Trys to parse an identifier (symbol)
            bool parseIdentifier(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);
            
            // Trys to parse a string
            bool parseString(GuardedToken& pos, int& cur_line, bel::expr::Expression*& expr);

            // Ignores newlines and increases the line count
            void ignoreNewlines(GuardedToken& pos, int& cur_line);

            // Kills multiply consecutive newlines
            // TODO: Problem: line numbers in parser are wrong (because N consecutive lines count as one line, not as N)
            void optimize();

            // Parses any construct
            bel::expr::Expression* parseAny(GuardedToken& pos, int& cur_line);

            // Returns a keyword for a specific keyword-name
            std::string getKeywordFor(const std::string& value, int& cur_line);
        };
    }
}

#endif
