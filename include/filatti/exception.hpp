#ifndef __FILATTI_EXCEPTION_HPP__
#define __FILATTI_EXCEPTION_HPP__

#include <exception>
#include <string>

namespace filatti {
    class Exception : public std::exception {
    private:
        std::string _message;

    public:
        explicit Exception(const std::string& message) : _message(message) { };

        virtual const char* what() const throw() {
            return _message.c_str();
        }
    };
}

#endif //__FILATTI_EXCEPTION_HPP__
