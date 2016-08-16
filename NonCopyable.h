#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace bel {
    class NonCopyable {
    public:
        NonCopyable() {}
        virtual ~NonCopyable() {}

    private:
        NonCopyable(const NonCopyable& that) {}

        NonCopyable& operator=(const NonCopyable& that) {
            return *this;
        }
    };
}

#endif
