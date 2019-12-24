#ifndef Parser_error_hpp
#define Parser_error_hpp

#include <iostream>
#include <type_traits>

#include "Grammar.hpp"
#include "tao/pegtl/parse_error.hpp"

namespace Parser{
	using namespace tao::pegtl;

	struct Escaped : seq < one<'\\'>, must < one < 'n', 'r', 't' > > > {};
	struct Content : tao::pegtl::star < sor< Escaped, not_one< '"' > > > {};
	struct Literal : seq < one< '"'>, Content, one< '"' > > {};

	template <typename Rule>
	struct control
		: tao::pegtl::normal< Rule >
	{
		static const std::string error_msg;

		template< typename Input, typename... States >
		static void raise(const Input& in, States&&...)
		{
			throw tao::pegtl::parse_error(error_msg, in);
		}
	};

	template<> const std::string control<semicolon>::error_msg = "expected ;";
	template<> const std::string control<Int_number>::error_msg = "expected int number";
	template<> const std::string control<Float_number>::error_msg = "expected float number";
	template<> const std::string control<closeparen>::error_msg = "expected )";
	template<> const std::string control<closecurly>::error_msg = "expected }";
	template<> const std::string control<closebrack>::error_msg = "expected ]";
	template<> const std::string control<Expression>::error_msg = "expected expression";
	template<> const std::string control<string_literal>::error_msg = "expected string";
	template<> const std::string control<comma>::error_msg = "expected ,";
	template<> const std::string control<Compound_statement>::error_msg = "expected Compound statement";
	template<> const std::string control<linecomment_end>::error_msg = "expected line comment";
	template<> const std::string control<blockcomment_end>::error_msg = "expected block comment ending";
	template<> const std::string control<While_statement_end>::error_msg = "expected While statement ending";
	template<> const std::string control<For_statement_end>::error_msg = "expected For statement ending";
	template<> const std::string control<space>::error_msg = "expected space";
	template<> const std::string control<spaces>::error_msg = "expected spaces";
	template<> const std::string control<space_s>::error_msg = "expected space_s";
	template<> const std::string control<Compound_statement_end>::error_msg = "expected Compound statement ending";
}

#endif