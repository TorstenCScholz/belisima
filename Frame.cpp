#include "Frame.h"

#include "Symbol.h"
#include "Expression.h"
#include "Panic.h"

namespace bel {
    namespace expr {
        Frame::Frame() {}

        Frame::~Frame() {
            for (auto it = _symTable.begin(); it != _symTable.end(); ++it) {
                delete it->second;
            }
        }

        bool Frame::insert(const std::string& symbol_name, Expression* expr) {
            bool newly_inserted = _symTable.find(symbol_name) == _symTable.end();

            _symTable[symbol_name] = expr;

            return newly_inserted;
        }

        bool Frame::remove(const std::string& symbol_name) {
            if (_symTable.find(symbol_name) != _symTable.end()) {
                _symTable.erase(symbol_name);
                return true;
            }

            return false;
        }

        bool Frame::exists(const std::string& symbol_name) const {
            return _symTable.find(symbol_name) != _symTable.end();
        }

        Expression* Frame::retrieve(const std::string& symbol_name) {
            if (_symTable.find(symbol_name) == _symTable.end()) {
                throw Panic("FRAME", "Cannot find symbol: " + symbol_name);
            }

            return _symTable[symbol_name];
        }

        std::string Frame::toString() const {
            return "{" + toStringWithoutBraces() + "}";
        }

        std::string Frame::toStringWithoutBraces() const {
            if (_symTable.empty()) { return ""; }

            std::string str;
            if (_symTable.size() > 0) {
                str = _symTable.begin()->first;
                
                // Erstes Element überspringen (weil schon vorhanden)
                auto it = _symTable.begin();
                for (++it; it != _symTable.end(); ++it)
                    str += ", " + it->first;
            }

            return str;
        }
    }
}
