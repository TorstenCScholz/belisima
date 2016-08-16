#include "Block.h"
#include "Panic.h"
#include "Environment.h"
#include "Frame.h"

namespace bel {
    namespace expr {
        Block::Block(Expression* content) : _cont(content) {
        }

        Block::Block(const Block& that) {
            _cont = that._cont->clone();
        }

        Block::~Block() {
            delete _cont;
        }

        Block& Block::operator=(const Block& that) {
            delete _cont;

            _cont = that._cont->clone();

            return *this;
        }

        Expression* Block::eval(Environment& env) {
            env.push(new Frame());
            Expression* ret = _cont->eval(env);
            env.pop();

            return ret;
        }

        Type::Value Block::type() const {
            return Type::Operator;
        }

        std::string Block::toString() const {
            return "Op(Block)";
        }

        Expression* Block::clone() const {
            return new Block(*this);
        }
    }
}
