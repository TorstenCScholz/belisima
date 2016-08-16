#include "Parser.h"
#include "Token.h"
#include "Sequence.h"
#include "Block.h"
#include "BacktrackingGuard.h"
#include "Number.h"
#include "Boolean.h"
#include "Symbol.h"
#include "String.h"
#include "Void.h"
#include "Function.h"
#include <iostream>
#include "ParserException.h"
#include "Declaration.h"
#include "Assignment.h"
#include "ProcCall.h"
#include "IfStatement.h"
#include "Comparison.h"

#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"

namespace bel {
    namespace interpreter {
        using namespace bel::expr;

        Parser::Parser(const std::vector<Token>& input) : _input(input) {
            optimize();
        }

        Parser::~Parser() {
        }

        Expression* Parser::parse() {
            Token* begin = &_input.front();
            Token* end = &_input.back() + 1;
            int cur_line = 1;

            GuardedToken pos(begin, end);

            Expression* ret = parseSequence(pos, cur_line);
            //ignoreNewlines(pos, cur_line);

            return ret;
        }

        void Parser::optimize() {
            std::vector<Token> optimized;

            for (auto it = _input.begin(); it != _input.end(); ++it) {
                optimized.push_back(*it);
                if (it->type() == Tok::SEPARATOR) {
                    // Keep track of number of ignored separators
                    SeparatorData* sd = new SeparatorData(0);
                    optimized.back().additionalData(sd);

                    while (it != _input.end() && it->type() == Tok::SEPARATOR) {
                        sd->count(sd->count() + 1);
                        it++;
                    }
                    it--; // Balance with ++it of for-loop
                }
            }

            _input = optimized;
        }

        Expression* Parser::parseBlock(GuardedToken& pos, int& cur_line) {

            ignoreNewlines(pos, cur_line);

            Block* block = nullptr;

            if (pos->type() == Tok::LEFT_BRACE) {
                pos++;

                ignoreNewlines(pos, cur_line);

                Expression* content = parseSequence(pos, cur_line);
                ignoreNewlines(pos, cur_line);

                if (pos->type() != Tok::RIGHT_BRACE) {
                    delete content;
                    throw bel::exception::ParserException("Parser::parseBlock", "Missing token '}'", cur_line);
                }
                pos++; // '}' read

                block = new Block(content);
            }

            return block;
        }

        Expression* Parser::parseAny(GuardedToken& pos, int& cur_line) {
            ignoreNewlines(pos, cur_line);

            // Try to parse a block
            Expression* expr = parseBlock(pos, cur_line);
            if (expr != nullptr) {
                return expr; // It's a block
            }
            
            expr = parseIf(pos, cur_line);
            if (expr != nullptr) {
                return expr;
            }

            expr = parseDecl(pos, cur_line);
            if (expr != nullptr) {
                return expr;
            }

            expr = parseAssignment(pos, cur_line);
            if (expr != nullptr) {
                return expr;
            }

            return parseExpression(pos, cur_line);
        }

        Expression* Parser::parseSequence(GuardedToken& pos, int& cur_line) {
            ignoreNewlines(pos, cur_line);

            Expression* expr = nullptr;

            if (!pos.isEnd() && pos->type() != Tok::EOFTOK) {
                expr = parseAny(pos, cur_line);

                // follow-up expression / statement?
                if (!pos.isEnd() && pos->type() == Tok::SEPARATOR) {
                    // We have a follow-up, so ignore seps and create a sequence
                    ignoreNewlines(pos, cur_line);

                    Sequence* seq = new Sequence();

                    seq->add(expr);
                    
                    // Only add sequence, if it is non-void (so we have no trailing empty sequence)
                    Expression* next_seq = parseSequence(pos, cur_line);
                    if (next_seq->type() == Type::Void) {
                        delete next_seq;
                    }
                    else {
                        seq->add(next_seq);
                    }

                    return seq;
                }
                
                // No followup statement
                return expr;
            }

            // No sequence
            return new Void();
        }

        Expression* Parser::parseExpression(GuardedToken& pos, int& cur_line) {
            ignoreNewlines(pos, cur_line);

            BacktrackingGuard<Token> bt(pos.get());
            Expression* expr = nullptr;
            
            if ((expr = parseProcCall(pos, cur_line)) != nullptr) {
                bt.noBacktrack();
                return expr;
            }
            else if ((expr = parseFunction(pos, cur_line)) != nullptr) {
                bt.noBacktrack();
                return expr;
            }
            else if (parseMath(pos, cur_line, expr)) {
                bt.noBacktrack();
                return expr;
            }
            else if (parseIdentifier(pos, cur_line, expr)) {
                bt.noBacktrack();
                return expr;
            }
            else if (parseString(pos, cur_line, expr)) {
                bt.noBacktrack();
                return expr;
            }
            else {
                bt.noBacktrack();
                return new Void(); // Emtpy expression
            }

            return expr;
        }

        Expression* Parser::parseDecl(GuardedToken& pos, int& cur_line) {
            ignoreNewlines(pos, cur_line);

            if (pos->type() == Tok::IDENT && pos->value() == getKeywordFor("Variable", cur_line)) {
                pos++; // Ignore keyword
                Expression* variable = nullptr;
                
                // Parse identifier
                if (!parseIdentifier(pos, cur_line, variable)) {
                    throw bel::exception::ParserException("Parser::parseDecl", std::string("Expected identifiert, but got ") + pos->name() + " indead.", cur_line);
                }
                
                Symbol* s = static_cast<Symbol*>(variable);
                Declaration* decl = new Declaration(s->name());
                delete variable; // Symbol no longer needed

                // Definition?
                if (!pos.isEnd() && pos->type() == Tok::EQUAL) {
                    pos++;

                    // Parse the assignment expr
                    Expression* assign = parseExpression(pos, cur_line);

                    // Make a sequence: 1) declaration 2) assignment
                    Sequence* seq = new Sequence();
                    seq->add(decl);
                    seq->add(new Assignment(decl->symbolName(), assign));

                    return seq;
                }

                return decl;
            }

            return nullptr; // No decl
        }

        bel::expr::Expression* Parser::parseFunction(GuardedToken& pos, int& cur_line) {
            BacktrackingGuard<Token> bt(pos.get());

            // Check for function begin
            if (pos->type() != Tok::IDENT || getKeywordFor("Function", cur_line) != pos->value()) {
                return nullptr;
            }
            pos++;

            Expression* func_name;
            if (!parseIdentifier(pos, cur_line, func_name)) {
                throw bel::exception::ParserException("Parser::parseFunction", "Function expects an identifier.", cur_line);
            }

            if (pos->type() != Tok::COLON) {
                delete func_name;
                throw bel::exception::ParserException("Parser::parseFunction", "Function expects a colon after identifier.", cur_line);
            }
            pos++;

            std::vector<Expression*> parameter_exprs = parseList(pos, cur_line);
            std::vector<std::string> parameters;

            // Check, if parameters are symbols (and nothing else)
            bool failure = false;
            for (auto it = parameter_exprs.begin(); it != parameter_exprs.end() && !failure; ++it) {
                Symbol* s = dynamic_cast<Symbol*>(*it);

                if (s == nullptr) {
                    failure = true;
                    continue;
                }

                parameters.push_back(s->name());
            }
            
            // We don't need the expressions any more
            for (auto it = parameter_exprs.begin(); it != parameter_exprs.end(); ++it) {
                delete *it;
            }

            if (failure) {
                delete func_name;
                throw bel::exception::ParserException("Parser::parseFunction", "Parameter list has to contain only symbols.", cur_line);
            }

            Expression* body = parseBlock(pos, cur_line);
            if (body == nullptr) {
                delete func_name;
                throw bel::exception::ParserException("Parser::parseFunction", "Function expects a block after parameter list.", cur_line);
            }

            Expression* func = new Function(dynamic_cast<Symbol*>(func_name)->name(), parameters, body);
            delete func_name;

            bt.noBacktrack();
            return func;
            //func a: a, b, c {
            //}
        }

        Expression* Parser::parseProcCall(GuardedToken& pos, int& cur_line) {
            BacktrackingGuard<Token> bt(pos.get());

            if (pos->type() != Tok::IDENT) {
                return nullptr;
                //throw bel::exception::ParserException("Parser::parseProcCall", std::string("A procedure call must begin with an identifier, but got '") + Token::nameForType(pos->type()) + "'.", cur_line);
            }

            Expression* proc_head;
            parseIdentifier(pos, cur_line, proc_head);
            Symbol* s = dynamic_cast<Symbol*>(proc_head);

            if (pos->type() != Tok::LEFT_PAR) {
                delete proc_head;
                return nullptr;
                //throw bel::exception::ParserException("Parser::parseProcCall", std::string("Expected '(', but got '") + Token::nameForType(pos->type()) + "'.", cur_line);
            }
            pos++;

            std::vector<Expression*> list_items = parseList(pos, cur_line);

            if (pos->type() != Tok::RIGHT_PAR) {
                delete proc_head;
                throw bel::exception::ParserException("Parser::parseProcCall", std::string("Expected ')', but got '") + Token::nameForType(pos->type()) + "' (" + pos->value() + ").", cur_line);
            }
            pos++;

            std::string proc_name = s->name();
            delete proc_head;

            bt.noBacktrack();
            return new ProcCall(proc_name, list_items);
        }

        std::vector<Expression*> Parser::parseList(GuardedToken& pos, int& cur_line) {
            // Test, if there is an expr. with result != Void
            Expression* expr = parseExpression(pos, cur_line);
            
            if (expr->type() == Type::Void) { // Empty list
                delete expr;
                return std::vector<Expression*>();
            }

            std::vector<Expression*> list_items;
            list_items.push_back(expr); // There was an expression

            while (true) {
                if (pos->type() != Tok::COMMA) {
                    break;
                    //throw bel::exception::ParserException("Parser::parseList", "Lists must be separated by a comma.", cur_line);
                }
                pos++; // Ignore ,
                
                expr = parseExpression(pos, cur_line);
                list_items.push_back(expr);
            }

            return list_items;
        }

        
        Expression* Parser::parseIf(GuardedToken& pos, int& cur_line) {
            // do we have an if-statement?
            if (pos->type() != Tok::IDENT || getKeywordFor("If", cur_line) != pos->value()) {
                return nullptr;
            }
            pos++;

            Expression* comp = parseComparison(pos, cur_line);
            if (comp == nullptr) {
                throw bel::exception::ParserException("Parser::parseIf", "Expected a comparison as argument to if.", cur_line);
            }

            // then-keyword?
            if (pos->type() != Tok::IDENT || getKeywordFor("Then", cur_line) != pos->value()) {
                delete comp;
                throw bel::exception::ParserException("Parser::parseIf", "Expected keyword for then after comparsion.", cur_line);
            }
            pos++;

            Expression* body = parseBlock(pos, cur_line);
            if (body == nullptr) {
                delete comp;
                throw bel::exception::ParserException("Parser::parseIf", "Expected a block after keyword for then.", cur_line);
            }

            // Else-part?
            BacktrackingGuard<Token> bt(pos.get());
            ignoreNewlines(pos, cur_line);
            if (!pos.isEnd() && pos->type() == Tok::IDENT && getKeywordFor("Else", cur_line) == pos->value()) {
                pos++;

                Expression* else_body = parseBlock(pos, cur_line);

                if (else_body == nullptr) {
                    delete comp;
                    delete body;
                    throw bel::exception::ParserException("Parser::parseIf", "Expected a block after keyword for else.", cur_line);
                }

                bt.noBacktrack();
                return new IfStatement(comp, body, else_body);
            }

            return new IfStatement(comp, body);
        }
        
        Expression* Parser::parseComparison(GuardedToken& pos, int& cur_line) {
            Expression* left;
            Expression* right;

            left = parseExpression(pos, cur_line);
            if (left->type() == Type::Void) {
                delete left;
                return nullptr;
            }

            switch (pos->type()) {
            case Tok::EQUAL:
                pos++; // Skip equal

                right = parseExpression(pos, cur_line);
                if (right->type() == Type::Void) {
                    delete left;
                    delete right;
                    throw bel::exception::ParserException("Parser::parseComparison", "Expected a second operand for comparison.", cur_line);
                }
                return new Comparison(left, right);
                break;
            default:
                // Maybe just one identifier
                return new Comparison(left, new Boolean(true));
                break;
            }

            delete left;
            throw bel::exception::ParserException("Parser::parseComparison", "Expected a comparison operator for comparison.", cur_line);
        }

        bel::expr::Expression* Parser::parseAssignment(GuardedToken& pos, int& cur_line) {
            BacktrackingGuard<Token> bt(pos.get());
            Expression* variable = nullptr;

            // Check for assignment to array element
            Expression* expr = parseProcCall(pos, cur_line);
            if (expr != nullptr) {
                ProcCall* proc = dynamic_cast<ProcCall*>(expr);

                if (pos->type() != Tok::EQUAL) {
                    delete proc;
                    return nullptr;
                }
                pos++;

                // Parse the assignment expr
                Expression* assign = parseExpression(pos, cur_line);

                if (assign->type() == Type::Void) {
                    delete proc;
                    delete assign;
                    throw bel::exception::ParserException("Parser::parseAssignment", "Expected an expression after equal sign.", cur_line);
                }

                std::vector<Expression*> args;
                std::vector<Expression*> old_args = proc->arguments();
                for (auto it = old_args.begin(); it != old_args.end(); ++it) {
                    args.push_back((*it)->clone());
                }

                Expression* expr = new Assignment(proc->name(), args, assign);
                delete proc;

                bt.noBacktrack();
                return expr;
            }
            else {
                // Parse identifier
                if (!parseIdentifier(pos, cur_line, variable)) {
                    return nullptr;
                }

                // Just an assignment to normal symbol
                Symbol* s = static_cast<Symbol*>(variable);

                if (pos->type() != Tok::EQUAL) {
                    delete s;
                    return nullptr;
                }
                pos++;

                // Parse the assignment expr
                Expression* assign = parseExpression(pos, cur_line);

                if (assign->type() == Type::Void) {
                    delete s;
                    delete assign;
                    throw bel::exception::ParserException("Parser::parseAssignment", "Expected an expression after equal sign.", cur_line);
                }

                Expression* expr = new Assignment(s->name(), assign);
                delete s;

                bt.noBacktrack();
                return expr;
            }
        }

        bool Parser::parseMath(GuardedToken& pos, int& cur_line, Expression*& expr) {
            BacktrackingGuard<Token> bt(pos.get());
            Expression* first;
            Expression* second;

            // left part
            if (!parseTerm(pos, cur_line, first)) {
                return false;
            }

            // Plus or minus sign
            bool minus = !pos.isEnd() && pos->type() == Tok::MINUS;
            if (pos.isEnd() || (pos->type() != Tok::PLUS && pos->type() != Tok::MINUS)) {
                // Math expression ends here
                expr = first;
                bt.noBacktrack();
                return true;
            }
            pos++;

            // right part
            if (!parseTerm(pos, cur_line, second)) {
                delete first;
                pos--; // To print out operator
                throw bel::exception::ParserException("Parser::parseMath", std::string("Expected a term after ") + pos->value() + ", but got " + (++pos)->value() + ".", cur_line);
            }

            // All correct
            bt.noBacktrack();
            if (minus) {
                expr = new Subtraction(first, second);
            }
            else {
                expr = new Addition(first, second);
            }

            return true;
        }

        bool Parser::parseTerm(GuardedToken& pos, int& cur_line, Expression*& expr) {
            BacktrackingGuard<Token> bt(pos.get());
            Expression* first;
            Expression* second;

            // left part
            if (!parseFactor(pos, cur_line, first)) {
                return false;
            }

            // Plus or minus sign
            bool multiply = true;
            if (pos.isEnd() || (pos->type() != Tok::MULT && pos->type() != Tok::DIV)) {
                // Math expression ends here
                expr = first;
                bt.noBacktrack();
                return true;
            }
            if (pos->type() == Tok::DIV) {
                multiply = false;
            }
            pos++;

            // right part
            if (!parseFactor(pos, cur_line, second)) {
                delete first;
                pos--; // To print out operator
                throw bel::exception::ParserException("Parser::parseTerm", std::string("Expected a factor after ") + pos->value() + ", but got " + (++pos)->value() + ".", cur_line);
            }

            // All correct
            bt.noBacktrack();
            if (multiply) {
                expr = new Multiplication(first, second);
            }
            else {
                expr = new Division(first, second);
            }

            return true;
        }

        bool Parser::parseFactor(GuardedToken& pos, int& cur_line, Expression*& expr) {
            BacktrackingGuard<Token> bt(pos.get());

            if (parseNumber(pos, cur_line, expr)) {
                bt.noBacktrack();
                return true;
            }
            else if ((expr = parseProcCall(pos, cur_line)) != nullptr) {
                bt.noBacktrack();
                return true;
            }
            else if (parseIdentifier(pos, cur_line, expr)) {
                bt.noBacktrack();
                return true;
            }
            else if (parseString(pos, cur_line, expr)) {
                bt.noBacktrack();
                return true;
            }
            else { // Nothing more, yet. TODO: [+-](...)
                if (pos->type() != Tok::LEFT_PAR) {
                    return false;
                }
                pos++;

                if (!parseMath(pos, cur_line, expr)) {
                    throw bel::exception::ParserException("Parser::parseFactor", "Expected a mathematical term.", cur_line);
                }

                if (pos->type() != Tok::RIGHT_PAR) {
                    delete expr;
                }
                pos++;

                bt.noBacktrack();
                return true;
            }
        }

        bool Parser::parseNumber(GuardedToken& pos, int& cur_line, Expression*& expr) {
            if (pos->type() == Tok::NUMBER) {
                expr = new Number(pos->value());
                pos++;
                return true;
            }

            return false;
        }

        bool Parser::parseIdentifier(GuardedToken& pos, int& cur_line, Expression*& expr) {
            if (pos->type() == Tok::IDENT) {
                expr = new Symbol(pos->value());
                pos++;
                return true;
            }

            return false;
        }

        bool Parser::parseString(GuardedToken& pos, int& cur_line, Expression*& expr) {
            if (pos->type() == Tok::STRING) {
                expr = new String(pos->value());
                pos++;
                return true;
            }

            return false;
        }

        void Parser::ignoreNewlines(GuardedToken& pos, int& cur_line) {
            // Ignore leading "empty" lines
            while (!pos.isEnd() && pos->type() == Tok::SEPARATOR) {
                cur_line += dynamic_cast<SeparatorData*>(pos->additionalData())->count();
                pos++;
            }
        }

        std::string Parser::getKeywordFor(const std::string& value, int& cur_line) {
            if (value == "Variable") {
                return "var";
            }
            else if (value == "If") {
                return "if";
            }
            else if (value == "Then") {
                return "then";
            }
            else if (value == "Else") {
                return "else";
            }
            else if (value == "Function") {
                return "func";
            }
            else {
                throw bel::exception::ParserException("Parser::getKeywordFor", std::string("Cannot resolve keyword for request '") + value + "'.", cur_line);
            }
        }
    }
}
