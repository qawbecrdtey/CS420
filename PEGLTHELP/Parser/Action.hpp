#ifndef Parser_Action_hpp
#define Parser_Action_hpp

#include <tao/pegtl.hpp>

#include "Grammar.hpp"

namespace Parser {
	template<typename Rule>
	struct action : tao::pegtl::nothing<Rule> {};
}

#endif