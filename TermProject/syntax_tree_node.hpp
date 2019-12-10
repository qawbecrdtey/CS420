#ifndef syntax_tree_node_hpp
#define syntax_tree_node_hpp

#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>

#include "string_pos.hpp"

enum class SYMBOL {
	none,
	plusequal,
	minusequal,
	starequal,
	slashequal,
	percentequal,
	equalequal,
	notequal,
	lessequal,
	greaterequal,
	plusplus,
	minusminus,
	slashslash,
	andand,
	oror,
	equal,
	plus,
	minus,
	star,
	slash,
	percent,
	backslash,
	not,
	less,
	greater,
	openparen,
	closeparen,
	opencurly,
	closecurly,
	openbrack,
	closebrack,
	dot,
	comma,
	semicolon,
	string_literal,
	identifier, type,
	number,

	// TODO : Fill in nonterminals.

	Int_number,						//	::= number
	Float_number,					//	::= number . number
									//	|	number . 
									//	|	. number
	Constant,						//	::= Int_number
									//	|	Float_number
	Primary_expression,
	Postfix_expression,
	Argument_expression_list,
	Argument_expression,
	Unary_expression,
	Unary_operator,
	Multiplicative_expression,
	Additive_expression,
	Shift_expression,
	Relational_expression,
	Equality_expression,
	Assignment_expression, // conditional_expression <- Equality_expression
	Declaration,
	Init_declarator_list,
	Init_declarator,
	Declarator, // declarator ::= identifier, initializer <- Assignment_expression
	Statement,
	Compound_statement,
	Block_item_list,
	Block_item,
	Expression_statement,
	Selection_statement,
	Iteration_statement,
	Jump_statement,
	External_declaration,
	Function_definition,
	Declaration_list,

};

void print_SYMBOL(SYMBOL symbol) {
	switch (symbol) {
	case SYMBOL::none: std::cout << "none" << std::endl; break;
	case SYMBOL::plusequal: std::cout << "plusequal" << std::endl; break;
	case SYMBOL::minusequal: std::cout << "minusequal" << std::endl; break;
	case SYMBOL::starequal: std::cout << "starequal" << std::endl; break;
	case SYMBOL::slashequal: std::cout << "slashequal" << std::endl; break;
	case SYMBOL::percentequal: std::cout << "percentequal" << std::endl; break;
	case SYMBOL::equalequal: std::cout << "equalequal" << std::endl; break;
	case SYMBOL::notequal: std::cout << "notequal" << std::endl; break;
	case SYMBOL::lessequal: std::cout << "lessequal" << std::endl; break;
	case SYMBOL::greaterequal: std::cout << "greaterequal" << std::endl; break;
	case SYMBOL::plusplus: std::cout << "plusplus" << std::endl; break;
	case SYMBOL::minusminus: std::cout << "minusminus" << std::endl; break;
	case SYMBOL::slashslash: std::cout << "slashslash" << std::endl; break;
	case SYMBOL::andand: std::cout << "andand" << std::endl; break;
	case SYMBOL::oror: std::cout << "oror" << std::endl; break;
	case SYMBOL::equal: std::cout << "equal" << std::endl; break;
	case SYMBOL::plus: std::cout << "plus" << std::endl; break;
	case SYMBOL::minus: std::cout << "minus" << std::endl; break;
	case SYMBOL::star: std::cout << "star" << std::endl; break;
	case SYMBOL::slash: std::cout << "slash" << std::endl; break;
	case SYMBOL::percent: std::cout << "percent" << std::endl; break;
	case SYMBOL::backslash: std::cout << "backslash" << std::endl; break;
	case SYMBOL::not: std::cout << "not" << std::endl; break;
	case SYMBOL::less: std::cout << "less" << std::endl; break;
	case SYMBOL::greater: std::cout << "greater" << std::endl; break;
	case SYMBOL::openparen: std::cout << "openparen" << std::endl; break;
	case SYMBOL::closeparen: std::cout << "closeparen" << std::endl; break;
	case SYMBOL::opencurly: std::cout << "opencurly" << std::endl; break;
	case SYMBOL::closecurly: std::cout << "closecurly" << std::endl; break;
	case SYMBOL::openbrack: std::cout << "openbrack" << std::endl; break;
	case SYMBOL::closebrack: std::cout << "closebrack" << std::endl; break;
	case SYMBOL::dot: std::cout << "dot" << std::endl; break;
	case SYMBOL::comma: std::cout << "comma" << std::endl; break;
	case SYMBOL::semicolon: std::cout << "semicolon" << std::endl; break;
	case SYMBOL::string_literal: std::cout << "string" << std::endl; break;
	case SYMBOL::identifier: std::cout << "identifier" << std::endl; break;
	case SYMBOL::type: std::cout << "type" << std::endl; break;
	case SYMBOL::number: std::cout << "number" << std::endl; break;

		// TODO : Fill in nonterminals.

	case SYMBOL::Constant: std::cout << "Constant" << std::endl; break;
	case SYMBOL::Primary_expression: std::cout << "Primary_expression" << std::endl; break;

	default:
		throw std::exception("Unexpected enum value.");
	}
}

struct syntax_tree_node {
	using node_ptr = std::unique_ptr<syntax_tree_node>;
	std::string_view data;
	SYMBOL symbol;
	std::vector<node_ptr> children;

	syntax_tree_node(std::string_view data, SYMBOL symbol) : data(data), symbol(symbol) {}

	void add_node(node_ptr&& node) {
		children.emplace_back(std::move(node));
	}
};

#endif