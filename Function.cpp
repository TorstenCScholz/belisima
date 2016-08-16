#include "Function.h"
#include "Frame.h"
#include "Environment.h"
#include "Panic.h"
#include <sstream>

// For Operators
#include <iostream>
#include "Void.h"
#include "Boolean.h"
#include "Symbol.h"
#include "Number.h"
#include "Array.h"
#include "String.h"
#include "Reference.h"

namespace bel {
    namespace expr {
        Function::Function(const std::string& name, const std::vector<std::string>& parameters, Expression* body) : _name(name), _params(parameters), _body(body) {
        }

        Function::Function(const Function& that) : _name(that._name), _params(that._params), _body(nullptr) {
            if (that._body != nullptr) {
                 _body = that._body->clone();
            }
        }

        Function::~Function() {
            delete _body;
        }

        Function& Function::operator=(const Function& that) {
            delete _body;

            _name = that._name;
            _params = that._params;
            _body = nullptr;
            if (that._body != nullptr) {
                _body = that._body->clone();
            }

            return *this;
        }
            
        int Function::formalsSize() const {
            return _params.size();
        }

        Expression* Function::apply(const std::vector<Expression*>& arguments, Environment& env) {
            // TODO: Use static env and append frame for the call
            //Frame* fr = new Frame();
            
            if (formalsSize() != arguments.size()) {
                std::stringstream ss;
                ss << "Cannot apply function with " << formalsSize() << " parameters to " << arguments.size() << " arguments.";
                throw bel::expr::Panic("FUNCTION", ss.str());
            }

            for (size_t i = 0; i < formalsSize(); ++i) {
                // TODO
                env.top()->insert(_params[i], arguments[i]->eval(env)); // Evaluate arguments in env
            }

            // TODO
            return _body->eval(env);
        }

        Expression* Function::eval(Environment& env) {
            return clone();
        }

        Type::Value Function::type() const {
            return Type::Function;
        }

        std::string Function::toString() const {
            return "Function:" + _name;
        }

        Expression* Function::clone() const {
            return new Function(*this);
        }

        // --- Operators ---

        class Op_println : public Function {
        public:
            Op_println() : Function("println", std::vector<std::string>(), nullptr) {}
            ~Op_println() {}

            int formalsSize() const {
                return -1; // VarArgs
            }

            Expression* apply(const std::vector<Expression*>& arguments, Environment& env) {
                if (arguments.size() > 0) {
                    Expression* arg = arguments[0]->eval(env);
                    std::cout << arg->toString();
                    delete arg;

                    for (size_t i = 1; i < arguments.size(); ++i) {
                        arg = arguments[i]->eval(env);
                        std::cout << " " << arg->toString();
                        delete arg;
                    }
                }

                std::cout << "\n";

                return new Void();
            }
        };

        class Op_defined : public Function {
        public:
            Op_defined() : Function("defined", std::vector<std::string>(), nullptr) {}
            ~Op_defined() {}

            int formalsSize() const {
                return 1;
            }

            Expression* apply(const std::vector<Expression*>& arguments, Environment& env) {
                if (formalsSize() != arguments.size()) {
                    std::stringstream ss;
                    ss << "Cannot apply 'defined' with " << formalsSize() << " parameters to " << arguments.size() << " arguments.";
                    throw bel::expr::Panic("OPERATOR", ss.str());
                }

                Symbol* sym = dynamic_cast<Symbol*>(arguments[0]);

                if (sym == nullptr) {
                    throw bel::expr::Panic("OPERATOR", "Operator 'defined' expects a symbol parameter.");
                }

                return new Boolean(env.top()->exists(sym->name()));
            }
        };
        
        class Op_type : public Function {
        public:
            Op_type() : Function("type", std::vector<std::string>(), nullptr) {}
            ~Op_type() {}

            int formalsSize() const {
                return 1;
            }

            Expression* apply(const std::vector<Expression*>& arguments, Environment& env) {
                if (formalsSize() != arguments.size()) {
                    std::stringstream ss;
                    ss << "Cannot apply 'type' with " << formalsSize() << " parameters to " << arguments.size() << " arguments.";
                    throw bel::expr::Panic("OPERATOR", ss.str());
                }

                Expression* expr = arguments[0]->eval(env);
                Symbol* s = new Symbol(typeToString(expr->type()));
                delete expr;

                return s;
            }
        };

        class Op_array : public Function {
        public:
            Op_array() : Function("array", std::vector<std::string>(), nullptr) {}
            ~Op_array() {}

            int formalsSize() const {
                return -1; // Varargs
            }

            Expression* apply(const std::vector<Expression*>& arguments, Environment& env) {
                if (arguments.size() == 0) {
                    throw bel::expr::Panic("OPERATOR", "Cannot apply 'array' to zero arguments.");
                }

                std::vector<size_t> dims;
                bool failure = false;

                for (auto it = arguments.begin(); it != arguments.end() && !failure; ++it) {
                    Expression* evaled = (*it)->eval(env);

                    Number* num = dynamic_cast<Number*>(evaled);

                    if (num == nullptr) {
                        failure = true;
                    }

                    dims.push_back(atoi(num->toString().c_str()));
                    delete num;
                }

                if (failure) {
                    throw bel::expr::Panic("OPERATOR", "At least one of the arguments to array is not a number.");
                }

                // TODO
                return new Reference(new Array(dims[0]));
            }
        };

        class Op_len : public Function {
        public:
            Op_len() : Function("len", std::vector<std::string>(), nullptr) {}
            ~Op_len() {}

            int formalsSize() const {
                return 1;
            }

            Expression* apply(const std::vector<Expression*>& arguments, Environment& env) {
                if (formalsSize() != arguments.size()) {
                    std::stringstream ss;
                    ss << "Cannot apply 'len' with " << formalsSize() << " parameters to " << arguments.size() << " arguments.";
                    throw bel::expr::Panic("OPERATOR", ss.str());
                }

                Expression* expr = arguments[0]->eval(env);

                if (expr->type() != Type::Reference) {
                    delete expr;
                    throw bel::expr::Panic("OPERATOR", std::string("Cannot call 'len' on ") + typeToString(expr->type()) + ".");
                }

                Expression* len;
                Type::Value type = ((Reference*)expr)->deref()->type();

                switch (type) {
                case Type::Array: {
                    Array* arr = (Array*)Reference::cast<Array>(expr);
                    len = new Number(arr->length());
                    break;
                }
                case Type::String: {
                    String* str = (String*)Reference::cast<String>(expr);;
                    len = new Number(str->length());
                    break;
                }
                default:
                    delete expr;
                    throw bel::expr::Panic("OPERATOR", std::string("Cannot call 'len' on ") + typeToString(type) + ".");
                    break;
                }

                delete expr;

                return len;
            }
        };
        
        Expression* Function::op_println = new Op_println();
        Expression* Function::op_defined = new Op_defined();
        Expression* Function::op_type = new Op_type();
        Expression* Function::op_array = new Op_array();
        Expression* Function::op_len = new Op_len();
    }
}
