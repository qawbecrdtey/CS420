#ifndef Parser_Grammar_hpp
#define Parser_Grammar_hpp

#include <tao/pegtl.hpp>

namespace Parser {
    namespace {
        struct blank : tao::pegtl::blank {};
        struct blanks : tao::pegtl::plus<blank> {};
        struct digit : tao::pegtl::digit {};
        struct digits : tao::pegtl::plus<digit> {};
        struct number : digits {};
        struct space : tao::pegtl::space {};
        struct spaces : tao::pegtl::plus<space> {};
        struct space_s : tao::pegtl::star<space> {};
        struct newline : tao::pegtl::one<'\n'> {};

        struct plusequal : tao::pegtl::string<'+', '='> {};
        struct minusequal : tao::pegtl::string<'-', '='> {};
        struct starequal : tao::pegtl::string<'*', '='> {};
        struct slashequal : tao::pegtl::string<'/', '='> {};
        struct percentequal : tao::pegtl::string<'%', '='> {};
        struct equalequal : tao::pegtl::string<'=', '='> {};
        struct notequal : tao::pegtl::string<'!', '='> {};
        struct lessequal : tao::pegtl::string<'<', '='> {};
        struct greaterequal : tao::pegtl::string<'>', '='> {};
        struct plusplus : tao::pegtl::string<'+', '+'> {};
        struct minusminus : tao::pegtl::string<'-', '-'> {};
        struct linecomment : tao::pegtl::seq<
            tao::pegtl::string<'/', '/'>,
            tao::pegtl::until<tao::pegtl::one<'\n'>>
        > {};
        struct blockcomment : tao::pegtl::seq<
            tao::pegtl::string<'/', '*'>,
            tao::pegtl::until<tao::pegtl::string<'*', '/'>>
        > {};
        struct andand : tao::pegtl::string<'&', '&'> {};
        struct oror : tao::pegtl::string<'|', '|'> {};
        struct equal : tao::pegtl::one<'='> {};
        struct plus : tao::pegtl::one<'+'> {};
        struct minus : tao::pegtl::one<'-'> {};
        struct star : tao::pegtl::one<'*'> {};
        struct slash : tao::pegtl::one<'/'> {};
        struct percent : tao::pegtl::one<'%'> {};
        struct backslash : tao::pegtl::one<92> {};
        struct exclamation : tao::pegtl::one<'!'> {};
        struct question : tao::pegtl::one<'?'> {};
        struct colon : tao::pegtl::one<':'> {};
        struct less : tao::pegtl::one<'<'> {};
        struct greater : tao::pegtl::one<'>'> {};
        struct openparen : tao::pegtl::one<'('> {};
        struct closeparen : tao::pegtl::one<')'> {};
        struct opencurly : tao::pegtl::one<'{'> {};
        struct closecurly : tao::pegtl::one<'}'> {};
        struct openbrack : tao::pegtl::one<'['> {};
        struct closebrack : tao::pegtl::one<']'> {};
        struct dot : tao::pegtl::one<'.'> {};
        struct comma : tao::pegtl::one<'.'> {};
        struct semicolon : tao::pegtl::one<';'> {};
        struct string_literal : tao::pegtl::seq<
            tao::pegtl::one<34>,
            tao::pegtl::until<
                tao::pegtl::minus<
                    tao::pegtl::one<34>,
                    tao::pegtl::string<92, 34>
                >
            >
        > {};
        
        // keywords
        struct break_keyword : tao::pegtl::string<'b', 'r', 'e', 'a', 'k'> {};
        struct continue_keyword : tao::pegtl::string<'c', 'o', 'n', 't', 'i', 'n', 'u', 'e'> {};
        struct else_keyword : tao::pegtl::string<'e', 'l', 's', 'e'> {};
        struct float_keyword : tao::pegtl::string<'f', 'l', 'o', 'a', 't'> {};
        struct for_keyword : tao::pegtl::string<'f', 'o', 'r'> {};
        struct if_keyword : tao::pegtl::string<'i', 'f'> {};
        struct int_keyword : tao::pegtl::string<'i', 'n', 't'> {};
        struct return_keyword : tao::pegtl::string<'r', 'e', 't', 'u', 'r', 'n'> {};
        struct void_keyword : tao::pegtl::string<'v', 'o', 'i', 'd'> {};
        struct while_keyword : tao::pegtl::string<'w', 'h', 'i', 'l', 'e'> {};
        
        struct keyword : tao::pegtl::sor<
            break_keyword,
            continue_keyword,
            else_keyword,
            float_keyword,
            for_keyword,
            if_keyword,
            int_keyword,
            return_keyword,
            void_keyword,
            while_keyword
        > {};

        // identifier
        struct identifier : tao::pegtl::minus<tao::pegtl::identifier, keyword> {};

        // nonterminal

        struct Type_specifier : tao::pegtl::sor<
            int_keyword,
            float_keyword,
            void_keyword
        > {};

        struct Int_number : digits {};
        
        struct Float_number : tao::pegtl::sor<
            tao::pegtl::seq<dot,number>,
            tao::pegtl::seq<number, dot, number>,
            tao::pegtl::seq<number,dot>
        > {};

        struct Constant : tao::pegtl::sor<Int_number, Float_number> {};
        
        struct Expression;
        struct Primary_expression : tao::pegtl::sor<
            identifier,
            Constant,
            tao::pegtl::seq<openparen, Expression, closeparen>
        > {};

        struct Argument_expression_list;
        struct Postfix_expression : tao::pegtl::sor<
            tao::pegtl::seq<Postfix_expression, openbrack, Expression, closebrack>,
            tao::pegtl::seq<Postfix_expression, openparen, Argument_expression_list, closeparen>,
            tao::pegtl::seq<Postfix_expression, openparen, closeparen>,
            tao::pegtl::seq<Postfix_expression, plusplus>,
            tao::pegtl::seq<Postfix_expression, minusminus>,
            Primary_expression
        > {};
        
        struct Assignment_expression;
        struct Argument_expression_list : tao::pegtl::sor<
            tao::pegtl::seq<Argument_expression_list, comma, Assignment_expression>,
            Assignment_expression
        > {};

        struct Unary_operator;
        struct Unary_expression : tao::pegtl::sor<
            Postfix_expression,
            tao::pegtl::seq<plusplus, Unary_expression>,
            tao::pegtl::seq<minusminus, Unary_expression>,
            tao::pegtl::seq<Unary_operator, Unary_expression>
        > {};

        struct Unary_operator : tao::pegtl::sor<star, plus, minus, exclamation> {};

        struct Multiplicative_expression : tao::pegtl::sor<
            tao::pegtl::seq<Multiplicative_expression, star, Unary_expression>,
            tao::pegtl::seq<Multiplicative_expression, slash, Unary_expression>,
            tao::pegtl::seq<Multiplicative_expression, percent, Unary_expression>,
            Unary_expression
        > {};

        struct Additive_expression : tao::pegtl::sor<
            tao::pegtl::seq<Additive_expression, plus, Multiplicative_expression>,
            tao::pegtl::seq<Additive_expression, minus, Multiplicative_expression>,
            Multiplicative_expression
        > {};

        struct Relational_expression : tao::pegtl::sor<
            tao::pegtl::seq<Relational_expression, less, Additive_expression>,
            tao::pegtl::seq<Relational_expression, greater, Additive_expression>,
            tao::pegtl::seq<Relational_expression, lessequal, Additive_expression>,
            tao::pegtl::seq<Relational_expression, greaterequal, Additive_expression>,
            Additive_expression
        > {};

        struct Equality_expression : tao::pegtl::sor<
            tao::pegtl::seq<Equality_expression, equalequal, Relational_expression>,
            tao::pegtl::seq<Equality_expression, notequal, Relational_expression>,
            Relational_expression
        > {};

        struct Logical_AND_expression : tao::pegtl::sor<
            tao::pegtl::seq<Logical_AND_expression, andand, Equality_expression>,
            Equality_expression
        > {};

        struct Logical_OR_expression : tao::pegtl::sor<
            tao::pegtl::seq<Logical_OR_expression, oror, Equality_expression>,
            Logical_AND_expression
        > {};

        struct Conditional_expression : tao::pegtl::sor<
            tao::pegtl::seq<Logical_OR_expression, question, Expression, colon, Conditional_expression>,
            Logical_OR_expression
        > {};

        struct Assignment_operator;
        struct Assignment_expression : tao::pegtl::sor<
            tao::pegtl::seq<Unary_expression, Assignment_operator, Assignment_expression>,
            Conditional_expression
        > {};

        struct Assignment_operator : tao::pegtl::sor<equal, starequal, slashequal, percentequal, plusequal, minusequal> {};
        
        struct Expression : tao::pegtl::sor<
            tao::pegtl::seq<Expression, comma, Assignment_expression>,
            Assignment_expression
        > {};

        struct Constant_expression : Conditional_expression {};

        struct Init_declarator_list;
        struct Declaration : tao::pegtl::sor<
            tao::pegtl::seq<Type_specifier, Init_declarator_list>,
            Type_specifier
        > {};

        struct Init_declarator;
        struct Init_declarator_list : tao::pegtl::sor<
            tao::pegtl::seq<Init_declarator_list, comma, Init_declarator>,
            Init_declarator
        > {};

        struct Declarator;
        struct Init_declarator : tao::pegtl::sor<
            tao::pegtl::seq<Declarator, equal, Assignment_expression>,
            Declarator
        > {};

        struct Pointer;
        struct Direct_declarator;
        struct Declarator : tao::pegtl::sor<
            tao::pegtl::seq<Pointer, Direct_declarator>,
            Direct_declarator
        > {};

        struct Type_qualifier_list;
        struct Parameter_list;
        struct Identifier_list;
        struct Direct_declarator : tao::pegtl::sor<
            tao::pegtl::seq<openparen, Declarator, closeparen>,
            tao::pegtl::seq<Direct_declarator, openbrack, Type_qualifier_list, Assignment_expression, closebrack>,
            tao::pegtl::seq<Direct_declarator, openbrack, Assignment_expression, closebrack>,
            tao::pegtl::seq<Direct_declarator, openparen, Parameter_list, closeparen>,
            tao::pegtl::seq<Direct_declarator, openparen, Identifier_list, closeparen>,
            tao::pegtl::seq<Direct_declarator, openparen, closeparen>,
            identifier
        > {};

        struct Pointer : tao::pegtl::sor<
            tao::pegtl::seq<star, Type_qualifier_list, Pointer>,
            tao::pegtl::seq<star, Type_qualifier_list>,
            tao::pegtl::seq<star, Pointer>,
            star
        > {};

        struct Type_pointer;
        struct Type_qualifier_list : tao::pegtl::sor<
            tao::pegtl::seq<Type_qualifier_list, Type_pointer>,
            Type_pointer
        > {};

        struct Parameter_declaration;
        struct Parameter_list : tao::pegtl::sor<
            tao::pegtl::seq<Parameter_list, comma, Parameter_declaration>,
            Parameter_declaration
        > {};

        //struct Declaration_specifiers;
        struct Abstract_declarator;
        struct Parameter_declaration : tao::pegtl::sor<
            tao::pegtl::seq<Type_specifier, Declarator>,
            tao::pegtl::seq<Type_specifier, Abstract_declarator>
        > {};

        struct Identifier_list : tao::pegtl::sor<
            tao::pegtl::seq<Identifier_list, comma, identifier>,
            identifier
        > {};

        struct Direct_abstract_declarator;
        struct Abstract_declarator : tao::pegtl::sor<
            tao::pegtl::seq<Pointer, Direct_abstract_declarator>,
            Direct_abstract_declarator,
            Pointer
        > {};

        struct Compound_statement;
        struct Expression_statement;
        struct Selection_statement;
        struct Iteration_statement;
        struct Jump_statement;
        struct Statement : tao::pegtl::sor<
            Compound_statement,
            Expression_statement,
            Selection_statement,
            Iteration_statement,
            Jump_statement
        > {};

        struct Block_item_list;
        struct Compound_statement : tao::pegtl::sor<
            tao::pegtl::seq<opencurly, Block_item_list, closecurly>,
            tao::pegtl::seq<opencurly, closecurly>
        > {};

        struct Block_item;
        struct Block_item_list : tao::pegtl::sor<
            tao::pegtl::sor<Block_item_list, Block_item>,
            Block_item
        > {};

        struct Block_item : tao::pegtl::sor<Declaration, Statement> {};

        struct Expression_statement : tao::pegtl::sor<
            tao::pegtl::seq<Expression, semicolon>,
            semicolon
        > {};

        struct Selection_statement : tao::pegtl::sor<
            tao::pegtl::seq<
                if_keyword,
                openparen,
                Expression,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                if_keyword,
                openparen,
                Expression,
                closeparen,
                Statement,
                else_keyword,
                Statement
            >
        > {};

        struct Iteration_statement : tao::pegtl::sor<
            tao::pegtl::seq<while_keyword, openparen, Expression, closeparen, Statement>,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                semicolon,
                semicolon,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                Expression,
                openparen,
                semicolon,
                semicolon,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                semicolon,
                Expression,
                semicolon,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                semicolon,
                semicolon,
                Expression,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                Expression,
                semicolon,
                Expression,
                semicolon,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                Expression,
                semicolon,
                semicolon,
                Expression,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                semicolon,
                Expression,
                semicolon,
                Expression,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                openparen,
                Expression,
                semicolon,
                Expression,
                semicolon,
                Expression,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                Declaration,
                semicolon,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                Declaration,
                Expression,
                semicolon,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                Declaration,
                semicolon,
                Expression,
                closeparen,
                Statement
            >,
            tao::pegtl::seq<
                for_keyword,
                Declaration,
                Expression,
                semicolon,
                Expression,
                closeparen,
                Statement
            >
        > {};

        struct Jump_statement : tao::pegtl::sor<
            tao::pegtl::seq<continue_keyword, semicolon>,
            tao::pegtl::seq<break_keyword, semicolon>,
            tao::pegtl::seq<return_keyword, Expression, semicolon>,
            tao::pegtl::seq<return_keyword, semicolon>
        > {};

        struct External_declaration;
        struct External_declaration_list : tao::pegtl::sor<
            tao::pegtl::seq<External_declaration_list, External_declaration>,
            External_declaration
        > {};

        struct Declaration_list;
        struct External_declaration : tao::pegtl::seq<
            Type_specifier,
            identifier,
            tao::pegtl::sor<
                tao::pegtl::seq<Declaration_list, Compound_statement>,
                Compound_statement
            >
        > {};

        struct Declaration_list : tao::pegtl::sor<
            tao::pegtl::sor<Declaration_list, Declaration>,
            Declaration
        > {};

        struct S_aug : External_declaration_list {};
    }
}

#endif