#ifndef __CHIBIENGINE_COMMON_UNIQUE_CLASS_
#define __CHIBIENGINE_COMMON_UNIQUE_CLASS_

namespace game{

    class UniqueClass {
    public:
        UniqueClass(const UniqueClass & ) = delete;
        const UniqueClass & operator=(const UniqueClass & ) = delete;

        UniqueClass(UniqueClass && ) = delete;
        UniqueClass & operator=(UniqueClass && ) = delete;
    protected:
        UniqueClass() = default;
    };

    class Noncopyable {
    public:
        Noncopyable(const Noncopyable & ) = delete;
        const Noncopyable & operator=(const Noncopyable & ) = delete;

        Noncopyable(Noncopyable && ) = default;
        Noncopyable & operator=(Noncopyable && ) = default;
    protected:
        Noncopyable() = default;
    };
}

#endif