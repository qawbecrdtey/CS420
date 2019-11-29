#ifndef type_miscs_hpp
#define type_miscs_hpp

#include <cstddef>

template<typename ...Args>
struct type_array;
template<typename T, typename ...Args>
struct type_array<T, Args...> {
    static constexpr std::size_t size = 1 + sizeof...(Args);

    template<std::size_t idx>
    using get_type = typename type_array<Args...>::template get_type<idx - 1>;
    template<>
    using get_type<0> = T;
};
template<>
struct type_array<> {
    static constexpr std::size_t size = 0;
};

template<typename T>
struct is_type_array : std::false_type {};
template<typename ...Args>
struct is_type_array<type_array<Args...>> : std::true_type {};

#endif