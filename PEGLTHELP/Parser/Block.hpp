#ifndef Parser_Block_hpp
#define Parser_Block_hpp

#include <array>
#include <stack>
#include <vector>

namespace Parser {
    template<typename T, uint64_t SIZE>
    struct type {
        std::array<T>
    };
}

#endif