#ifndef __FILATTI_SYNCHRONOUS_HPP__
#define __FILATTI_SYNCHRONOUS_HPP__

#include <mutex>

namespace filatti {
    class Synchronous {
    private:
        std::mutex _lock;

    protected:
        inline void synchronize(std::function<void()> function) noexcept {
            std::lock_guard<std::mutex> lock_guard(_lock);
            function();
        }

    public:
        virtual ~Synchronous() { }
    };
}

#endif //__FILATTI_SYNCHRONOUS_HPP__
