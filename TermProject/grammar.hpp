#ifndef grammar_hpp
#define grammar_hpp

#include <array>
#include <variant>

#include "syntax_tree_node.hpp"

// http://jsmachines.sourceforge.net/machines/lr1.html

/*      GRAMMAR

1   S ::= A B S
2       | A B
3   A ::= A a
4       | a
5   B ::= B b
6       | b

*/

/*                  LR TABLE

    STATE   |     ACTION      |      GOTO       |
    ---------------------------------------------
            |  a  |  b  |  $  |  S  |  A  |  B  |
    =============================================
    0       | s 3 |     |     |  1  |  2  |     |
    1       |     |     | acc |     |     |     |
    2       | s 5 | s 6 |     |     |     |  4  |
    3       | r 4 | r 4 |     |     |     |     |
    4       | s 3 | s 8 | r 2 |  7  |  2  |     |
    5       | r 3 | r 3 |     |     |     |     |
    6       | r 6 | r 6 | r 6 |     |     |     |
    7       |     |     | r 1 |     |     |     |
    8       | r 5 | r 5 | r 5 |     |     |     |
*/

namespace grammar {
    struct shift { std::uint64_t n; };
    struct reduce { std::uint64_t n; };
    struct accept {};
    struct error {};
    
    using action = std::variant<error, accept, shift, reduce>;

    struct LRTable {
        static constexpr std::uint64_t state_cnt = 9;
        static constexpr std::uint64_t token_cnt = 2;
        static constexpr std::uint64_t nonterminal_cnt = 3;
        
        static constexpr std::array<std::array<action, token_cnt + 1>, state_cnt> Action = {
            { shift{3},     error{},    error{}     },
            { error{},      error{},    accept{}    },
            { shift{5},     shift{6},   error{}     },
            { reduce{4},    reduce{4},  error{}     },
            { shift{3},     shift{8},   reduce{2}   },
            { reduce{3},    reduce{3},  error{}     },
            { reduce{6},    reduce{6},  reduce{6}   },
            { error{},      error{},    reduce{1}   },
            { reduce{5},    reduce{5},  reduce{5}   }
        };

        static constexpr std::array<std::array<std::int64_t, nonterminal_cnt>, state_cnt> Goto = {
            {  1,  2, -1 },
            { -1, -1,  4 },
            { -1, -1, -1 },
            {  7,  2, -1 },
            { -1, -1, -1 },
            { -1, -1, -1 },
            { -1, -1, -1 },
            { -1, -1, -1 }
        };
    };
}

#endif