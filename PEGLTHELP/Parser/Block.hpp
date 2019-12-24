#ifndef Parser_Block_hpp
#define Parser_Block_hpp

#include <array>
#include <map>
#include <stack>
#include <variant>
#include <vector>

namespace Parser {

    template<typename T>
    struct type {
        uint64_t size;
        std::unique_ptr<T[]> arr;

        type(T& data, uint64_t size = 1) : size(size), arr(std::make_unique<T[]>(size)) {}
        type(type const& t) : size(size), arr(std::make_unique<T[]>(t.size)) {
            for (uint64_t i = 0; i < size; i++) {
                arr[i] = t.arr[i];
            }
        }
        type(type&& t) : size(std::move(size)), arr(std::move(t.arr)) {};
        type& operator=(type const& t) {
            if (&t != this) {
                size = t.size;
                arr = std::make_unique<T[]>(size);
                for (uint64_t i = 0; i < size; i++) {
                    arr[i] = t.arr[i];
                }
            }
            return *this;
        }
        type& operator=(type&& t) {
            if (&t != this) {
                size = std::move(t.size);
                arr = std::move(t.arr);
            }
            return *this;
        }
        ~type() = default;


    };

    using Type = std::variant<type<int>, type<float>>;

    using identifier_map_stack = std::map<std::string, std::stack<Type>>;
}

#endif