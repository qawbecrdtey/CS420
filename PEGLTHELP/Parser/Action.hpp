#ifndef Parser_Action_hpp
#define Parser_Action_hpp

#include <tao/pegtl.hpp>

#include "Grammar.hpp"

namespace Parser {
	template<typename Rule>
	struct action : tao::pegtl::nothing<Rule> {};

	template<>
	struct action<Int_number> {
		template<typename Input>
		static void apply(Input const& in) {
			std::cout << "number is: " << in.string() << std::endl;
		}
	};
	template<>
	struct action<digit> {
	    static void apply0() {
	        std::cout << "digit" << std::endl;
	    }
	};
	template<>
	struct action<grammar>{
	    static void apply0() {
	        std::cout << "grammar" << std::endl;
	    }
	};
	template<>
	struct action<space_s> {
	    static void apply0() {
	        std::cout << "space_s" << std::endl;
	    }
	};
	template<>
	struct action<identifier> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "identifier : " << in.string() << std::endl;
	    }
	};
	template<>
	struct action<Type_specifier> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Type_specifier : " << in.string() << std::endl;
	    }
	};
	template<>
	struct action<openparen>{
	    static void apply0() {
	        std::cout << "openparen" << std::endl;
	    }
	};
	template<>
	struct action<Direct_declarator>{
	    static void apply0(){
	        std::cout << "Direct_declarator" << std::endl;
	    }
	};

	template<>
	struct action<Primary_expression> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Primary_expression : " << in.string() << std::endl;
	    }
	};
	template<>
	struct action<Expression> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Expression : " << in.string() << std::endl;
	    }
	};

	template<>
	struct action<spaces> {
	    static void apply0() {
	        std::cout << "spaces" << std::endl;
	    }
	};
	template<>
	struct action<Declarator> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Declarator : " << in.string() << std::endl;
	    }
	};
	template<>
	struct action<Pointer> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Pointer : " << in.string() << std::endl;
	    }
	};
}

#endif