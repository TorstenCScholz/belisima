#ifndef POSITIONGUARD_H
#define POSITIONGUARD_H

#include "InterpreterException.h"

namespace bel {
    namespace interpreter {
        template<class T>
        class GuardedPosition {
        public:
            GuardedPosition(T*& pos, const T* end) : _pos(pos), _end(end) {}
            GuardedPosition(const GuardedPosition& that) : _pos(that.pos), _end(that.end) {}
            ~GuardedPosition() {}

            GuardedPosition& operator=(const GuardedPosition& that) {
                _pos = that._pos;
                _end = that._end;

                return *this;
            }

            T& operator*() {
                if (_pos < _end)
                    return *_pos;
                throw bel::exception::InterpreterException("GuardedPosition::operator*", "Position is invalid.");
            }

            T* operator->() {
                if (_pos < _end)
                    return _pos;
                throw bel::exception::InterpreterException("GuardedPosition::operator->", "Position is invalid.");
            }
            
            T* operator++() {
                T* temp = _pos++;
                return temp;
            }

            T* operator++(int dummy) {
                return ++_pos;
            }

            T* operator--() {
                T* temp = _pos--;
                return temp;
            }

            T* operator--(int dummy) {
                return --_pos;
            }

            T*& get() {
                return _pos;
            }

            bool isEnd() const {
                return !(_pos < _end);
            }

        private:
            T*& _pos;
            const T* _end;
        };
    }
}

#endif
