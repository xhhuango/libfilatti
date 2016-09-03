#ifndef __FILATTI_DIRTY_HPP__
#define __FILATTI_DIRTY_HPP__

#include <atomic>

namespace filatti {
    class Dirty {
    private:
        std::atomic_bool _dirty;

    protected:
        Dirty(bool dirty) : _dirty(dirty) { }

        inline void make_dirty() noexcept {
            _dirty = true;
        }

        inline bool make_clean_if_dirty() noexcept {
            bool expected = true;
            return _dirty.compare_exchange_strong(expected, false);
        }

    public:
        virtual ~Dirty() { };

        inline bool is_dirty() const noexcept {
            return _dirty;
        };
    };
}

#endif //__FILATTI_DIRTY_HPP__
