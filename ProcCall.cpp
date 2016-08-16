#include "ProcCall.h"
#include "Environment.h"
#include "Frame.h"
#include "Panic.h"
#include "Procedure.h"
#include "Array.h"
#include "Number.h"
#include "Reference.h"
#include <sstream>

namespace bel {
    namespace expr {
        ProcCall::ProcCall(const std::string& proc_name, std::vector<Expression*> arguments) : _proc_name(proc_name), _args(arguments) {
        }

        ProcCall::ProcCall(const ProcCall& that) : _proc_name(that._proc_name) {
            for (auto it = that._args.begin(); it != that._args.end(); ++it) {
                _args.push_back((*it)->clone());
            }
        }

        ProcCall::~ProcCall() {
            for (auto it = _args.begin(); it != _args.end(); ++it) {
                delete *it;
            }
        }

        ProcCall& ProcCall::operator=(const ProcCall& that) {
            _proc_name = that._proc_name;

            for (auto it = that._args.begin(); it != that._args.end(); ++it) {
                _args.push_back((*it)->clone());
            }

            return *this;
        }
        
        Expression* ProcCall::eval(Environment& env) {
            Expression* expr = env.retrieve(_proc_name);

            if (expr->type() == Type::Array) {
                Array* arr = Reference::cast<Array>(expr);
                
                std::vector<size_t> args;
                for (auto it = _args.begin(); it != _args.end(); ++it) {
                    Expression* possible_num = (*it)->eval(env);
                    Number* num = dynamic_cast<Number*>(possible_num);
                        
                    if (num == nullptr) {
                        delete possible_num;
                        throw bel::expr::Panic("PROCEDURE", "At least one argument of the array does not contain a number.");
                    }
                        
                    args.push_back(atoi(num->toString().c_str()));
                    delete possible_num;
                }

                return arr->get(args)->clone();
            }
            else {
                Procedure* proc = dynamic_cast<Procedure*>(env.retrieve(_proc_name));

                if (proc == nullptr) {
                    throw bel::expr::Panic("PROCCALL", std::string("Symbol '") + _proc_name + "' is not an array or procedure.");
                }

                // Test for artity is handled by proc->apply itself, thus missing here
                return proc->apply(_args, env);
            }
        }

        Type::Value ProcCall::type() const {
            return Type::Operator;
        }

        std::string ProcCall::toString() const {
            return "Op(ProcCall)";
        }

        Expression* ProcCall::clone() const {
            return new ProcCall(*this);
        }

        
        std::string ProcCall::name() const {
            return _proc_name;
        }

        std::vector<Expression*> ProcCall::arguments() const {
            return _args;
        }
    }
}
