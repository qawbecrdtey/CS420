#ifndef grammar_hpp
#define grammar_hpp

#include <array>
#include <map>
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
	using node_ptr = std::unique_ptr<syntax_tree_node>;

	struct shift { std::uint64_t n; };
	struct reduce { std::uint64_t n; };
	struct accept {};
	struct error {};

	using action = std::variant<error, accept, shift, reduce>;

	struct LRTable {
		static constexpr std::uint64_t state_cnt = 9;
		static constexpr std::uint64_t token_cnt = 2;
		static constexpr std::uint64_t nonterminal_cnt = 3;
		static constexpr std::uint64_t rule_cnt = 6;

		static constexpr std::array<std::array<action, token_cnt + 1>, state_cnt> ACTION{
			error{},	shift{3},   error{},
			accept{},	error{},    error{},
			error{},	shift{5},   shift{6},
			error{},	reduce{4},  reduce{4},
			reduce{2},	shift{3},   shift{8},
			error{},	reduce{3},	reduce{3},
			reduce{6},  reduce{6},	reduce{6},
			reduce{1},	error{},    error{},
			reduce{5},	reduce{5},  reduce{5}
		};

		static constexpr std::array<std::array<std::int64_t, nonterminal_cnt>, state_cnt> GOTO{
			1, 2, -1,
			-1, -1, 4,
			-1, -1, -1,
			7, 2, -1,
			-1, -1, -1,
			-1, -1, -1,
			-1, -1, -1,
			-1, -1, -1
		};
	};

	class ParseError : public std::exception {
		string_pos pos;

	public:
		ParseError(string_pos pos, char const* info = "") : std::exception(info), pos(std::move(pos)) {}
		ParseError(string_pos pos, std::string info = "") : std::exception(info.c_str()), pos(std::move(pos)) {}
	};

	std::map<std::string, std::uint64_t> token_map = {
		{ "a", 1 },
		{ "b", 2 }
	};

	std::array<std::uint64_t, LRTable::rule_cnt + 1> rule_size{ 1, 3, 2, 2, 1, 2, 1 };
	auto const get_nonterminal = [](std::uint64_t n) {
		switch (n) {
		case 1:
		case 2:
			return 1;
			break;
		case 3:
		case 4:
			return 2;
			break;
		case 5:
		case 6:
			return 3;
			break;
		default: throw std::exception("Cannot get such nonterminal.");
		}
	};

	void parse(std::vector<string_pos> const& data) {
		auto&& it = data.begin();
		std::stack<node_ptr> stack;
		std::stack<std::int64_t> state_stack;
		state_stack.push(0);

		while (true) {
			if (it != data.end()) {
				switch (LRTable::ACTION[state_stack.top()][token_map[it->str]].index) {
				case 0: // error
				case 1: // accept
					throw ParseError(it->str.c_str, "Wrong syntax.");

				case 2: // shift
					stack.push(std::make_unique<syntax_tree_node>(0, it->base, it->byte));
					state_stack.push(std::get<2>(LRTable::ACTION[state_stack.top()][token_map[it->str]]).n);
					break;

				case 3: // reduce
				{
					auto const n = std::get<3>(LRTable::ACTION[state_stack.top()][token_map[it->str]]).n;
					auto&& p = std::make_unique<syntax_tree_node>(n, )
					for (std::uint64_t i = 0; i < n; i++) {
						stack.pop();
						state_stack.pop();
					}
				}
					break;

				default: throw std::bad_variant_access(); // program fault
				}
			}
			else {
				switch (LRTable::ACTION[s][0].index) {
				case 0: // error
				case 1: // accept
				case 2: // shift
				case 3: // reduce
				default: throw std::bad_variant_access(); // program fault
				}
			}
		}
	}
}

#endif