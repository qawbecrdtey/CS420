#ifndef Parser_Action_hpp
#define Parser_Action_hpp

#include <map>
#include <stack>
#include <string>
#include <variant>
#include <vector>

#include <tao/pegtl.hpp>

#include "Grammar.hpp"

namespace Parser {
	struct int_T {
		int val;
	};
	struct float_T {
		float val;
	};
	struct string_T {
		std::string val;
	};
	struct int_pointer_T {
		int *val;
	};
	struct float_pointer_T {
		float *val;
	};
	struct string_pointer_T {
		std::string *val;
	};

	using Value = std::variant<int_T, float_T, string_T, int_pointer_T, float_pointer_T, string_pointer_T>;
	
	using Identifier_map_stack = std::map<std::string, std::stack<Value>>;
	using Identifier_storage_vector = std::vector<std::string>;
	Identifier_storage_vector primary_isv;

	template<typename Rule>
	struct action : tao::pegtl::nothing<Rule> {};

	template<>
	struct action<grammar> {
		static void apply0(Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << "grammarly correct!" << std::endl;
		}
	};
	template<>
	struct action<Int_number> {
		template<typename Input>
		static void apply(Input const& in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
		    std::cout << "Int_number" << std::endl;
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<Assignment_expression> {
		template<typename Input>
		static void apply(Input const& in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
            std::cout << "Float_number" << std::endl;
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<External_declaration_list> {
		template<typename Input>
		static void apply(Input const& in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
            std::cout << "External_declaration_list" << std::endl;
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<External_declaration> {
		template<typename Input>
		static void apply(Input const& in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
            std::cout << "External_declaration" << std::endl;
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<tao::pegtl::eof> {
		static void apply0(Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << "EOF" << std::endl;
		}
	};
	template<>
	struct action<Init_declarator_list> {
	    template<typename Input>
	    static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
            std::cout << "Init_declarator_list" << std::endl;
			std::cout << in.string() << std::endl;
	    }
	};
	template<>
	struct action<int_keyword> {
	    template<typename Input>
	    static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
            std::cout << "int_keyword" << std::endl;
			std::cout << in.string() << std::endl;
	    }
	};
    template<>
    struct action<float_keyword> {
        template<typename Input>
        static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
            std::cout << "float_keyword" << std::endl;
			std::cout << in.string() << std::endl;
        }
    };
	template<>
	struct action<Parameter_declaration> {
		template<typename Input>
		static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<Init_declarator> {
		template<typename Input>
		static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<identifier> {
		template<typename Input>
		static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << in.string() << std::endl;
			isv.push_back(in.string());
		}
	};
	template<>
	struct action<string_literal> {
		template<typename Input>
		static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << in.string() << std::endl;
		}
	};
	template<>
	struct action<semicolon> {
		template<typename Input>
		static void apply(Input const &in, Identifier_map_stack &ims, Identifier_storage_vector &isv) {
			std::cout << in.string() << std::endl;
		}
	};
}

#endif