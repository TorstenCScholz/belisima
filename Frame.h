#ifndef FRAME_H
#define FRAME_H

#include <string>
#include "NonCopyable.h"
#include <unordered_map>

namespace bel {
    namespace expr {
        class Symbol;
        class Expression;

        class Frame : public NonCopyable {
        public:
            Frame();
            ~Frame();

            bool insert(const std::string& symbol_name, Expression* expr);
            bool remove(const std::string& symbol_name);

            bool exists(const std::string& symbol_name) const;

            Expression* retrieve(const std::string& symbol_name);

            std::string toString() const;

        private:
            std::unordered_map<std::string, Expression*> _symTable;

            std::string toStringWithoutBraces() const;
        };
    }
}

#endif
