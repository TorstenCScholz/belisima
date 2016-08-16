#include "Array.h"
#include "Panic.h"
#include "Void.h"

#include <sstream>
#include <iostream>

namespace bel {
    namespace expr {
        Array::Array(size_t len) : _content(len) {
            for (size_t i = 0; i < len; ++i) {
                _content.at(i) = new Void();
            }
        }

        Array::Array(const Array& that) {
            for (auto it = that._content.begin(); it != that._content.end(); ++it) {
                _content.push_back((*it)->clone());
            }
        }

        Array::~Array() {
            for (auto it = _content.begin(); it != _content.end(); ++it) {
                delete *it;
            }
        }

        Array& Array::operator=(const Array& that) {
            for (auto it = _content.begin(); it != _content.end(); ++it) {
                delete *it;
            }
            _content.clear();

            for (auto it = that._content.begin(); it != that._content.end(); ++it) {
                _content.push_back((*it)->clone());
            }

            return *this;
        }

        Expression* Array::eval(Environment& env) {
            Array* arr = (Array*)clone();

            std::cout << ">>>> In eval: " << arr->length() << std::endl;

            return arr;
        }

        Type::Value Array::type() const {
            return Type::Array;
        }

        std::string Array::toString() const {
            std::stringstream str;

            str << "[";
            if (_content.size() > 0) {
                str << _content[0]->toString();
            }
            if (_content.size() > 1) {
                for (int i = 1; i < _content.size(); ++i) {
                    str << ", " << _content[i]->toString();
                }
            }

            str << "]";

            return str.str();
        }

        Expression* Array::clone() const {
            return new Array(*this);
        }

        Expression* Array::get(const std::vector<size_t>& pos) const {
            if (pos.size() == 1) {
                if (pos[0] >= _content.size()) {
                    throw bel::expr::Panic("ARRAY", "Index out of range.");
                }

                return _content.at(pos[0]);
            }
            
            std::vector<size_t> dec_pos(pos);
            dec_pos.erase(dec_pos.begin());

            Array* arr = dynamic_cast<Array*>(_content.at(pos[0]));
            if (arr == nullptr) {
                throw bel::expr::Panic("ARRAY", "Cannot get element, because element is not an array.");
            }

            return arr->get(dec_pos);
        }

        void Array::set(const std::vector<size_t>& pos, Expression* data) {
            if (pos.size() == 1) {
                if (pos[0] >= _content.size()) {
                    throw bel::expr::Panic("ARRAY", "Index out of range.");
                }

                delete _content.at(pos[0]);
                _content.at(pos[0]) = data;
            }
            else {
                std::vector<size_t> dec_pos(pos);
                dec_pos.erase(dec_pos.begin());

                Array* arr = dynamic_cast<Array*>(_content.at(pos[0]));
                if (arr == nullptr) {
                    throw bel::expr::Panic("ARRAY", "Cannot get element, because element is not an array.");
                }

                arr->set(dec_pos, data);
            }
        }

        size_t Array::length() const {
            return _content.size();
        }
    }
}
