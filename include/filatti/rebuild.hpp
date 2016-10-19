#ifndef __FILATTI_REBUILD_HPP__
#define __FILATTI_REBUILD_HPP__

#include <filatti/synchronous.hpp>

namespace filatti {
    class Rebuild {
    private:
        bool _does_rebuild;

    protected:
        Rebuild(bool does_rebuild) : _does_rebuild(does_rebuild) { }

        inline void set_rebuild(bool does_rebuild) noexcept {
            _does_rebuild = does_rebuild;
        }

        inline bool does_rebuild() const noexcept {
            return _does_rebuild;
        }

    public:
        virtual ~Rebuild() { }
    };
}

#endif //__FILATTI_REBUILD_HPP__
