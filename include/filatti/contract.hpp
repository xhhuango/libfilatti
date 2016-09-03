#ifndef __FILATTI_PRECONDITION_HPP__
#define __FILATTI_PRECONDITION_HPP__

#include <filatti/exception.hpp>

namespace filatti {
#define PRECONDITION(condition, message)        \
    if (!(condition))                           \
        throw Exception(message)
}

#endif //__FILATTI_PRECONDITION_HPP__
