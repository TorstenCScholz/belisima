#include "Environment.h"

#include "Frame.h"
#include "Panic.h"

namespace bel {
    namespace expr {

        Environment::Environment() : _env(new _Environment()) {
        }

        Environment::Environment(const Environment& that) : _env(that._env) {
        }
        
        Environment::~Environment() {
        }

        Environment& Environment::operator=(const Environment& that) {
            _env = that._env;

            return *this;
        }
        
        Expression* Environment::eval(Environment& env) {
            return new Environment(*this);
        }

        Type::Value Environment::type() const {
            return _env->type();
        }

        std::string Environment::toString() const {
            return _env->toString();
        }

        Frame* Environment::top() const {
            return _env->top();
        }
        
        void Environment::push(Frame* frame) {
            _env->push(frame);
        }

        void Environment::pop() {
            _env->pop();
        }

        Expression* Environment::retrieve(const std::string& name) {
            return _env->retrieve(name);
        }
        
        void Environment::update(const std::string& name, Expression* expr) {
            _env->update(name, expr);
        }

        Environment::_Environment::_Environment() {
            _frames.push_back(new Frame());
        }

        Environment::_Environment::~_Environment() {
            for (auto it = _frames.begin(); it != _frames.end(); ++it)
                delete *it;
        }

        Type::Value Environment::_Environment::type() const {
            return Type::Environment;
        }

        std::string Environment::_Environment::toString() const {
            return "(" + top()->toString() + ")";
        }

        std::string Environment::_Environment::toStringWithoutBraces() const {
            if (_frames.empty()) { return ""; }

            std::string str;
            if (_frames.size() > 0) {
                str = (*_frames.begin())->toString(); // Erstes Frame
                
                // Erstes Element überspringen (weil schon vorhanden)
                auto it = _frames.begin();
                for (++it; it != _frames.end(); ++it)
                    str += ", " + (*it)->toString();
            }

            return str;
        }

        Frame* Environment::_Environment::top() const {
            return *_frames.begin();
        }

        void Environment::_Environment::push(Frame* frame) {
            _frames.insert(_frames.begin(), frame);
        }

        void Environment::_Environment::pop() {
            delete *_frames.begin();
            _frames.erase(_frames.begin());
        }
        
        Expression* Environment::_Environment::retrieve(const std::string& name) {
            for (auto it = _frames.begin(); it != _frames.end(); ++it) {
                if ((*it)->exists(name)) {
                    return (*it)->retrieve(name);
                }
            }

            throw bel::expr::Panic("ENVIRONMENT", "No such name: " + name);
        }

        void Environment::_Environment::update(const std::string& name, Expression* expr) {
            for (auto it = _frames.begin(); it != _frames.end(); ++it) {
                if ((*it)->exists(name)) {
                    (*it)->insert(name, expr);
                    return;
                }
            }

            throw bel::expr::Panic("ENVIRONMENT", "No such name: " + name);
        }
    }
}
