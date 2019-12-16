#ifndef Parser_Action_hpp
#define Parser_Action_hpp

#include <tao/pegtl.hpp>

#include "Grammar.hpp"

namespace Parser {
	template<typename Rule>
	struct action : tao::pegtl::nothing<Rule> {};

	template<>
	struct action<grammar> {
		static void apply0() {
			std::cout << "grammarly correct!" << std::endl;
		}
	};
	/*
	template<>
	struct action<Int_number> {
		template<typename Input>
		static void apply(Input const &in) {
			std::cout << in.string() << std::endl;
		}
	};
	*/
	template<>
	struct action<Int_number> {
		template<typename Input>
		static void apply(Input const& in) {
			std::cout << in.string() << std::endl;
			std::cout << in.position() << std::endl;
		}
	};
	template<>
	struct action<space_s> {
		template<typename Input>
		static void apply(Input const& in) {
			std::cout << "space_s : " << in.string() << std::endl;
			std::cout << in.position() << std::endl;
		}
	};
	template<>
	struct action<External_declaration_list> {
		template<typename Input>
		static void apply(Input const& in) {
			std::cout << "External_declaration_list" << std::endl;
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<External_declaration> {
		template<typename Input>
		static void apply(Input const& in) {
			std::cout << "External_declaration" << std::endl;
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<tao::pegtl::eof> {
		static void apply0() {
			std::cout << "EOF" << std::endl;
		}
	};
	template<>
	struct action<Compound_statement> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Compound_statement" << std::endl;
	        std::cout << in.string() << std::endl;
	    }
	};
	template<>
	struct action<Init_declarator_list> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Init_declarator_list" << std::endl;
	        std::cout << in.string() << std::endl;
	    }
	};
	template<>
	struct action<openparen> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "openparen" << std::endl;
	        std::cout << in.position() << std::endl;
	    }
	};
	template<>
	struct action<closeparen> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "closeparen" << std::endl;
	        std::cout << in.position() << std::endl;
	    }
	};
	template<>
	struct action<for_keyword> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "for_keyword" << std::endl;
	        std::cout << in.position() << std::endl;
	    }
	};
	template<>
	struct action<Declaration_list> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "Declaration_list" << std::endl;
	        std::cout << in.position() << std::endl;
	    }
	};
	template<>
	struct action<int_keyword> {
	    template<typename Input>
	    static void apply(Input const &in) {
	        std::cout << "int_keyword" << std::endl;
	        std::cout << in.position() << std::endl;
	    }
	};
    template<>
    struct action<float_keyword> {
        template<typename Input>
        static void apply(Input const &in) {
            std::cout << "int_keyword" << std::endl;
            std::cout << in.position() << std::endl;
        }
    };
    template<>
    struct action<Pointer> {
        template<typename Input>
        static void apply(Input const &in) {
            std::cout << "Pointer" << std::endl;
            std::cout << in.position() << std::endl;
        }
    };
}

#endif