#include "Number.h"
#include "Expression.h"
#include "String.h"
#include "Void.h"
#include "Environment.h"
#include "Symbol.h"
#include "Frame.h"
#include "Panic.h"
#include "Procedure.h"
#include "Function.h"

#include "Lexer.h"
#include "Token.h"

#include "Parser.h"

#include "LexerException.h"
#include "ParserException.h"

#include <iostream>

using namespace bel;
using namespace bel::interpreter;

void printExpr(expr::Expression& e) {
    expr::Environment env;
    expr::Expression* evaled = e.eval(env);

    std::cout << "e is of type: " << expr::typeToString(evaled->type()) << std::endl;
    std::cout << "e has value : <" << evaled->toString() << ">" << std::endl;

    delete evaled;
}

int main(int argc, char** argv) {
    expr::Number num1("100", "3");
    expr::Number num2(3);

    printExpr(num1);
    
    std::cout << num1.toString() << " + " << num2.toString() << " = " << (num1 + num2).toString() << std::endl;
    std::cout << num1.toString() << " - " << num2.toString() << " = " << (num1 - num2).toString() << std::endl;
    std::cout << num1.toString() << " * " << num2.toString() << " = " << (num1 * num2).toString() << std::endl;
    std::cout << num1.toString() << " / " << num2.toString() << " = " << (num1 / num2).toString() << std::endl;
    
    std::cout << num1.toString() << " <  " << num2.toString() << "? " << ((num1 < num2) ? "true" : "false") << std::endl;
    std::cout << num1.toString() << " >  " << num2.toString() << "? " << ((num1 > num2) ? "true" : "false") << std::endl;
    std::cout << num1.toString() << " <= " << num2.toString() << "? " << ((num1 <= num2) ? "true" : "false") << std::endl;
    std::cout << num1.toString() << " >= " << num2.toString() << "? " << ((num1 >= num2) ? "true" : "false") << std::endl;
    std::cout << num1.toString() << " != " << num2.toString() << "? " << ((num1 != num2) ? "true" : "false") << std::endl;
    
    std::cout << expr::Number::One.toString() << " >  " << expr::Number::One.toString() << "? " << ((expr::Number::One > expr::Number::One) ? "true" : "false") << std::endl;
    std::cout << expr::Number::One.toString() << " >= " << expr::Number::One.toString() << "? " << ((expr::Number::One >= expr::Number::One) ? "true" : "false") << std::endl;
    std::cout << expr::Number::One.toString() << " <  " << expr::Number::One.toString() << "? " << ((expr::Number::One < expr::Number::One) ? "true" : "false") << std::endl;
    std::cout << expr::Number::One.toString() << " <= " << expr::Number::One.toString() << "? " << ((expr::Number::One <= expr::Number::One) ? "true" : "false") << std::endl;
    
    expr::String str1("Hallo Welt!");

    printExpr(str1);

    expr::Void void1;

    printExpr(void1);

    expr::Environment env;
    expr::Symbol* sym = new expr::Symbol("num1");
    env.top()->insert(sym->name(), new expr::Number(num1));

    printExpr(env);

    try {
        std::cout << "Value of " << sym->toString() << " = " << env.top()->retrieve(sym->name())->toString() << std::endl;
        env.top()->retrieve("test");
    }
    catch (bel::expr::Panic& p) {
        std::cout << "Error: " << p.message() << std::endl;
    }

    try {
        //Lexer lx("if a = b {\nc = 3/2, d = 3.1415\n, str = \"Hallo \\\"Welt\\\".\"}", false);
        Lexer lx("ArrayTest.txt");
        std::vector<Token> tokens = lx.scan();

        /*std::cout << "Tokens:" << std::endl;
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            std::cout << "Type: " << it->name() << ", Value: " << it->value() << ", Line: " << it->line() << std::endl;
        }*/

        try {
            bel::expr::Environment global_env;
            global_env.top()->insert("println", bel::expr::Function::op_println);
            global_env.top()->insert("defined", bel::expr::Function::op_defined);
            global_env.top()->insert("type", bel::expr::Function::op_type);
            global_env.top()->insert("array", bel::expr::Function::op_array);
            global_env.top()->insert("len", bel::expr::Function::op_len);

            Parser ps(tokens);
            bel::expr::Expression* prog_value = ps.parse()->eval(global_env);
            std::cout << "Program:" << std::endl;
            std::cout << "Value = " << prog_value->toString() << std::endl;
            std::cout << "Type  = " << bel::expr::typeToString(prog_value->type()) << std::endl;
            delete prog_value;
        }
        catch (bel::exception::ParserException& ex) {
            std::cout << "Parsing Error: " << ex.what() << std::endl;
        }
        catch (bel::expr::Panic& ex) {
            std::cout << ex.name() << ": " << ex.message() << std::endl;
        }
    }
    catch (bel::exception::Exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
    }

    system("pause");

    return 0;
}
