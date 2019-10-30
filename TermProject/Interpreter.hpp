#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <array>
#include <type_traits>

#include "type_miscs.hpp"

namespace Interpreter {

/// DEFINITION : token
/// In this program, tokens are characters.
/// All terminals are tokens.

    template<char c>
    struct token {
        static constexpr char character = c;
    };

    template<typename T>
    struct is_whitespace : std::false_type {};
    template<char c>
    struct is_whitespace<token<c>> {
        static constexpr bool value = (c == 127 || c < 33);
    };

    template<typename T>
    struct is_printable : std::false_type {};
    template<char c>
    struct is_printable<token<c>> {
        static constexpr bool value = (c != 127 && c > 32);
    };

/// END DEFINITION : token

/// DEFINITION : nonterminal
/// Should implement all nonterminals.

    struct whitespace {};
    struct whitespaces {};
    

/// END DEFINITION : nonterminal

/// DEFINITION : production
/// Should implement all productions

    template<typename T, typename ...Args>
    struct production {
        
    };

/// END DEFINITION : production
}

#endif