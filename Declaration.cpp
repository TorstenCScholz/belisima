#include "Declaration.h"
#include "Void.h"
#include "Environment.h"
#include "Frame.h"
#include "Symbol.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Declaration::Declaration(const std::string& var_name) : _var_name(var_name) {
        }

        Declaration::Declaration(const Declaration& that) : _var_name(that._var_name) {
        }

        Declaration::~Declaration() {
        }
        
        Declaration& Declaration::operator=(const Declaration& that) {
            _var_name = that._var_name;

            return *this;
        }

        std::string Declaration::symbolName() const {
            return _var_name;
        }

        Expression* Declaration::eval(Environment& env) {
            // Declare variable in env
            if (!env.top()->insert(_var_name, new Void()))
                throw bel::expr::Panic("DECLARATION", std::string("Cannot declare same variable '") + _var_name + "' in the same scope.");

            return new Void();
        }

        Type::Value Declaration::type() const {
            return Type::Operator;
        }

        std::string Declaration::toString() const {
            return "Op(Declaration)";
        }

        Expression* Declaration::clone() const {
            return new Declaration(*this);
        }
    }
}
