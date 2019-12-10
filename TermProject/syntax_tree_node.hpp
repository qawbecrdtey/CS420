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
	exclamation,
	question,
	colon,
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
    break_keyword,
    continue_keyword,
    else_keyword,
    float_keyword,
    for_keyword,
    if_keyword,
    int_keyword,
	return_keyword,
	void_keyword,
	while_keyword,
    identifier,
	number,

	// TODO : Fill in nonterminals.

	Type_specifier,                 //  ::= float_keyword
	                                //  |   int_keyword
	                                //  |   void_keyword
	Int_number,						//	::= number
	Float_number,					//	::= number . number
									//	|	number .
									//	|	. number
	Constant,						//	::= Int_number
									//	|	Float_number
	Primary_expression,             //  ::= identifier
	                                //  |   Constant
	                                //  |   ( Expression )
	Postfix_expression,             //  ::= Primary_expression
	                                //  |   Postfix_expression [ Expression ]
	                                //  |   Postfix_expression ( Argument_expression_list* )
	                                //  |   Postfix_expression plusplus
	                                //  |   Postfix_expression minusminus
	Argument_expression_list,       //  ::= Assignment_expression
	                                //  |   Argument_expression_list , Assignment_expression
	Unary_expression,               //  ::= Postfix_expression
	                                //  |   plusplus Unary_expression
	                                //  |   minusminus Unary_expression
	                                //  |   Unary_operator Unary_expression
	Unary_operator,                 //  ::= star
	                                //  |   plus
	                                //  |   minus
	                                //  |   exclamation
	Multiplicative_expression,      //  ::= Unary_expression
	                                //  |   Multiplicative_expression star Unary_expression
	                                //  |   Multiplicative_expression slash Unary_expression
	                                //  |   Multiplicative_expression percent Unary_expression
	Additive_expression,            //  ::= Multiplicative_expression
	                                //  |   Additive_expression plus Multiplicative_expression
	                                //  |   Additive_expression minus Multiplicative_expression
	Relational_expression,          //  ::= Additive_expression
	                                //  |   Relational_expression less Additive_expression
	                                //  |   Relational_expression greater Additive_expression
	                                //  |   Relational_expression lessequal Additive_expression
	                                //  |   Relational_expression greaterequal Additive_expression
	Equality_expression,            //  |   Relational_expression
	                                //  |   Equality_expression equalequal Relational_expression
	                                //  |   Equality_expression notequal Relational_expression
    Logical_AND_expression,         //  ::= Equality_expression
                                    //  |   Logical_AND_expression andand Equality_expression
    Logical_OR_expression,          //  ::= Logical_AND_expression
                                    //  |   Logical_OR_expression oror Logical_AND_expression
    Conditional_expression,         //  ::= Logical_OR_expression
                                    //  |   Logical_OR_expression question Expression colon Conditional_expression
	Assignment_expression,          //  ::= Conditional_expression
	                                //  |   Unary_expression Assignment_operator Assignment_expression
    Assignment_operator,            //  ::= equal
                                    //  |   starequal
                                    //  |   slashequal
                                    //  |   percentequal
                                    //  |   plusequal
                                    //  |   minusequal
	Expression,                     //  ::= Assignment_expression
	                                //  |   Expression , Assignment_expression
    Constant_expression,            //  ::= Conditional_expression
    Declaration,                    //  ::= Type_specifier init_declarator_list*
	Init_declarator_list,           //  ::= Init_declarator
	                                //  |   Init_declarator_list , Init_declarator
	Init_declarator,                //  ::= Declarator
	                                //  |   Declarator = Assignment_expression
	Declarator,                     //  ::= identifier
	Statement,                      //  ::= Compound_statement
	                                //  |   Expression_statement
	                                //  |   Selection_statement
	                                //  |   Iteration_statement
	                                //  |   Jump_statement
	Compound_statement,             //  ::= { Block_item_list* }
	Block_item_list,                //  ::= Block_item
	                                //  |   Block_item_list Block_item
	Block_item,                     //  ::= Declaration
	                                //  |   Statement
	Expression_statement,           //  ::= Expression* ;
	Selection_statement,            //  ::= if_keyword ( Expression ) Statement
	                                //  |   if_keyword ( Expression ) Statement else_keyword Statement
	Iteration_statement,            //  ::= while_keyword ( Expression ) Statement
	                                //  |   for_keyword ( Expression* ; Expression* ; Expression* ) Statement
	                                //  |   for_keyword ( declaration Expression* ; Expression* ) Statement
	Jump_statement,                 //  ::= continue_keyword ;
	                                //  |   break_keyword ;
	                                //  |   return Expression* ;
	External_declaration,           //  ::= Type_specifier Identifier Declaration_list* Compound_statement
	Declaration_list,               //  ::= Declaration
	                                //  |   Declaration_list Declaration
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
	case SYMBOL::exclamation: std::cout << "exclamation" << std::endl; break;
    case SYMBOL::question: std::cout << "question" << std::endl; break;
    case SYMBOL::colon: std::cout << "colon" << std::endl; break;
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
	case SYMBOL::break_keyword: std::cout << "break_keyword" << std::endl; break;
	case SYMBOL::continue_keyword: std::cout << "continue_keyword" << std::endl; break;
	case SYMBOL::else_keyword: std::cout << "else_keyword" << std::endl; break;
	case SYMBOL::float_keyword: std::cout << "float_type" << std::endl; break;
	case SYMBOL::for_keyword: std::cout << "for_keyword" << std::endl; break;
	case SYMBOL::if_keyword: std::cout << "if_keyword" << std::endl; break;
	case SYMBOL::int_keyword: std::cout << "int_type" << std::endl; break;
	case SYMBOL::return_keyword: std::cout << "return_keyword" << std::endl; break;
	case SYMBOL::void_keyword: std::cout << "void_keyword" << std::endl; break;
	case SYMBOL::while_keyword: std::cout << "while_keyword" << std::endl; break;
	case SYMBOL::identifier: std::cout << "identifier" << std::endl; break;
	case SYMBOL::number: std::cout << "number" << std::endl; break;

		// TODO : Fill in nonterminals.

	// case SYMBOL::Constant: std::cout << "Constant" << std::endl; break;
	// case SYMBOL::Primary_expression: std::cout << "Primary_expression" << std::endl; break;

	default:
		throw std::exception();
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