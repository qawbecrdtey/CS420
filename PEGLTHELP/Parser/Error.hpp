#ifndef Parser_error_hpp
#define Parser_error_hpp

#include <iostream>
#include <type_traits>

#include "Grammar.hpp"

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
			throw tao::petgl::parse_error(error_meg, in);
		}
	};

	template<> const std::string control<semicolon>::error_msg = "expected ;";
	template<> const std::string control<digit>::error_msg = "expected digit";
	template<> const std::string control<closeparen>::error_msg = "expected )";
	template<> const std::string control<closecurly>::error_msg = "expected }";
	template<> const std::string control<closebrack>::error_msg = "expected ]";
	template<> const std::string control<Expression>::error_msg = "expected expression";
	template<> const std::string control<string_literal>::error_msg = "expected string";
	template<> const std::string control<comma>::error_msg = "expected ,";
	template<> const std::string control<Compound_statement>::error_msg = "expected Compound statement";
}

#endif