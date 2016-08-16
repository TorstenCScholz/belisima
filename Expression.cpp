#include "Expression.h"

namespace bel {
    namespace expr {
        std::string typeToString(Type::Value type) {
            switch (type) {
            case Type::None:
                return "No type";
            case Type::Array:
                return "Array";
            case Type::Boolean:
                return "Boolean";
            case Type::Class:
                return "Class";
            case Type::Environment:
                return "Environment";
            case Type::Function:
                return "Function";
            case Type::Operator:
                return "Operator";
            case Type::Number:
                return "Number";
            case Type::String:
                return "String";
            case Type::Symbol:
                return "Symbol";
            case Type::Void:
                return "Void";
            case Type::Panic:
                return "Panic";
            default:
                return "Unknown type";
            }
        }
    }
}
