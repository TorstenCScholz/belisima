#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Expression.h"
#include <vector>
#include <memory>
#include "NonCopyable.h"

namespace bel {
    namespace expr {
        class Frame;

        class Environment : public Expression {
        public:
            Environment();
            Environment(const Environment& that);
            ~Environment();

            Environment& operator=(const Environment& that);

            Expression* eval(Environment& env);
            Type::Value type() const;
            std::string toString() const;

            Frame* top() const;

            void push(Frame* frame);
            void pop();

            Expression* retrieve(const std::string& name);

            void update(const std::string& name, Expression* expr);

        private:

            class _Environment;
            std::shared_ptr<_Environment> _env;

            class _Environment : public NonCopyable {
            public:
                _Environment();
                ~_Environment();

                Expression* eval(Environment& env);
                Type::Value type() const;
                std::string toString() const;

                Frame* top() const;

                void push(Frame* frame);
                void pop();

                Expression* retrieve(const std::string& name);

                void update(const std::string& name, Expression* expr);

            private:
                std::vector<Frame*> _frames;

                std::string toStringWithoutBraces() const;
            };
        };
    }
}

#endif
