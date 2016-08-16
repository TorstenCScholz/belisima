#ifndef BACKTRACKINGGUARD_H
#define BACKTRACKINGGUARD_H

namespace bel {
    namespace interpreter {
	    template<class T>
	    class BacktrackingGuard {
	    public:
		    BacktrackingGuard(T*& pos);
		    ~BacktrackingGuard();

		    void noBacktrack() { _backtrack = false; }

	    private:
		    T*& _pos;
		    T* _start;

		    bool _backtrack;
	    };
	
	    template<class T>
	    BacktrackingGuard<T>::BacktrackingGuard(T*& start):
	    _pos(start),
	    _start(start),
	    _backtrack(true)
	    {
	    }

	    template<class T>
	    BacktrackingGuard<T>::~BacktrackingGuard() {
		    if (_backtrack)
			    _pos = _start;
	    }
    }
}

#endif
