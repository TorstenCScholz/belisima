#include "Assignment.h"
#include "Void.h"
#include "Environment.h"
#include "Frame.h"
#include "Panic.h"
#include "Array.h"
#include "Number.h"
#include "Reference.h"
#include <iostream>

namespace bel {
    namespace expr {
        Assignment::Assignment(const std::string& var_name, Expression* assignment) : _var_name(var_name), _assignment(assignment) {
        }

        Assignment::Assignment(const std::string& var_name, const std::vector<Expression*>& args, Expression* assignment) : _var_name(var_name), _args(args), _assignment(assignment) {
        }

        Assignment::Assignment(const Assignment& that) : _var_name(that._var_name), _assignment(that._assignment->clone()) {
            for (auto it = that._args.begin(); it != that._args.end(); ++it) {
                _args.push_back((*it)->clone());
            }
        }

        Assignment::~Assignment() {
            delete _assignment;

            for (auto it = _args.begin(); it != _args.end(); ++it) {
                delete *it;
            }
        }
        
        Assignment& Assignment::operator=(const Assignment& that) {
            delete _assignment;
            for (auto it = _args.begin(); it != _args.end(); ++it) {
                delete *it;
            }
            _args.clear();

            _var_name = that._var_name;
            _assignment = that._assignment->clone();
            for (auto it = that._args.begin(); it != that._args.end(); ++it) {
                _args.push_back((*it)->clone());
            }

            return *this;
        }

        Expression* Assignment::eval(Environment& env) {
            Expression* expr = env.retrieve(_var_name);
            Expression* evaled = nullptr;

            if (expr != nullptr) {
                // Array assignment
                if (_args.size() > 0) {
                    if (expr->type() != Type::Reference) {
                        throw bel::expr::Panic("ASSIGNMENT", "Cannot assign value to an array call not containing an array.");
                    }

                    std::vector<size_t> args;
                    for (auto it = _args.begin(); it != _args.end(); ++it) {
                        Expression* possible_num = (*it)->eval(env);
                        Number* num = dynamic_cast<Number*>(possible_num);
                        
                        if (num == nullptr) {
                            delete possible_num;
                            throw bel::expr::Panic("ASSIGNMENT", "At least one argument of the array does not contain a number.");
                        }
                        
                        args.push_back(atoi(num->toString().c_str()));
                        delete possible_num;
                    }

                    evaled = _assignment->eval(env);
                    Array* arr = Reference::cast<Array>(expr);
                    arr->set(args, evaled);
                }
                else {
                    // Normal symbol assignment
                    evaled = _assignment->eval(env);
                    delete expr; // Delete old value
                    env.update(_var_name, evaled);
                }
            }
            else {
                throw bel::expr::Panic("ASSIGNMENT", std::string("Cannot assign value to a non-declared variable '") + _var_name + "'.");
            }

            return evaled->clone();
        }

        Type::Value Assignment::type() const {
            return Type::Operator;
        }

        std::string Assignment::toString() const {
            return "Op(Assignment)";
        }

        Expression* Assignment::clone() const {
            return new Assignment(*this);
        }
    }
}
