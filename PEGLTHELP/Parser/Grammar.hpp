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
        struct spaces;
        struct space_s;
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
        struct linecomment_end : tao::pegtl::until<tao::pegtl::one<'\n', '\0'>> {};
        struct linecomment : tao::pegtl::if_must<
            tao::pegtl::string<'/', '/'>,
            linecomment_end
        > {};
        struct blockcomment_end : tao::pegtl::until<tao::pegtl::string<'*', '/'>> {};
        struct blockcomment : tao::pegtl::if_must<
            tao::pegtl::string<'/', '*'>,
            blockcomment_end
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
        struct comma : tao::pegtl::one<','> {};
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
        struct space : tao::pegtl::sor<
            tao::pegtl::space,
            linecomment,
            blockcomment
        > {};
        struct spaces : tao::pegtl::plus<space> {};
        struct space_s : tao::pegtl::star<space> {};

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
            string_literal,
            tao::pegtl::seq<openparen, space_s, Expression, space_s, closeparen>
        > {};

        struct Argument_expression_list;
        struct Postfix_expression_R;
        struct Postfix_expression : tao::pegtl::seq<
            Primary_expression,
            tao::pegtl::opt<space_s, Postfix_expression_R>
        > {};
        
        struct Array_subscript : tao::pegtl::seq<
            openbrack, space_s,
            Expression, space_s,
            closebrack
        > {};
        struct Function_call : tao::pegtl::seq<
            openparen, space_s,
            tao::pegtl::opt<Argument_expression_list, space_s>,
            closeparen
        > {};
        struct Postfix_expression_R : tao::pegtl::sor<
            Array_subscript,
            Function_call,
            plusplus,
            minusminus
        > {};
        
        struct Assignment_expression;
        struct Argument_expression_list : tao::pegtl::list<
            Assignment_expression,
            tao::pegtl::pad<comma, space>
        > {};

        struct Unary_operator;
        struct Unary_expression : tao::pegtl::sor<
            Postfix_expression,
            tao::pegtl::seq<
                tao::pegtl::sor<plusplus, minusminus, Unary_operator>,
                Unary_expression
            >
        > {};

        struct Unary_operator : tao::pegtl::sor<star, plus, minus, exclamation> {};

        struct Multiplicative_expression : tao::pegtl::list<
            Unary_expression,
			tao::pegtl::pad<tao::pegtl::sor<star, slash, percent>, space>
        > {};

        struct Additive_expression : tao::pegtl::list<
            Multiplicative_expression,
            tao::pegtl::pad<tao::pegtl::sor<plus, minus>, space>
        > {};

        struct Relational_expression : tao::pegtl::list<
            Additive_expression,
            tao::pegtl::pad<tao::pegtl::sor<lessequal, greaterequal, less, greater>, space>
        > {};

        struct Equality_expression : tao::pegtl::list<
            Relational_expression,
            tao::pegtl::pad<tao::pegtl::sor<equalequal, notequal>, space>
        > {};

        struct Logical_AND_expression : tao::pegtl::list<
            Equality_expression,
            tao::pegtl::pad<andand, space>
        > {};

        struct Logical_OR_expression : tao::pegtl::list<
            Logical_AND_expression,
            tao::pegtl::pad<oror, space>
        > {};

        struct Conditional_expression : tao::pegtl::seq<
            Logical_OR_expression,
            tao::pegtl::opt<
                space_s,
                question, space_s,
                Expression, space_s,
                colon, space_s,
                Conditional_expression
            >
        > {};

        struct Assignment_operator;
        struct Assignment_expression : tao::pegtl::sor<
            tao::pegtl::seq<Unary_expression, space_s, Assignment_operator, space_s, Assignment_expression>,
            Conditional_expression
        > {};

        struct Assignment_operator : tao::pegtl::sor<equal, starequal, slashequal, percentequal, plusequal, minusequal> {};
        
        struct Expression : tao::pegtl::list<
            Assignment_expression,
            tao::pegtl::pad<comma, space>
        > {};

        struct Init_declarator_list;
        struct Declaration : tao::pegtl::seq<
            Type_specifier,
            tao::pegtl::opt<spaces, Init_declarator_list>,
            space_s,
            tao::pegtl::must<semicolon>
        > {};

        struct Init_declarator;
        struct Init_declarator_list : tao::pegtl::list<
            Init_declarator,
            tao::pegtl::pad<comma, space>
        > {};

        struct Declarator;
        struct Init_declarator : tao::pegtl::seq<
            Declarator,
            tao::pegtl::opt<space_s, equal, space_s, Assignment_expression>
        > {};

        struct Pointer;
        struct Direct_declarator;
        struct Declarator : tao::pegtl::seq<
            tao::pegtl::opt<Pointer, space_s>,
			Direct_declarator
        > {};

        struct Parameter_list;
        struct Identifier_list;
        struct Direct_declarator_R;
        struct Direct_declarator : tao::pegtl::seq<
            identifier,
            tao::pegtl::opt<Direct_declarator_R>
        > {};

        struct Direct_declarator_R_brack : tao::pegtl::seq<
            openbrack, space_s, Assignment_expression, space_s, closebrack
        > {};
        struct Direct_declarator_R_paren : tao::pegtl::seq<
            openparen, space_s,
            tao::pegtl::opt<tao::pegtl::sor<Parameter_list, Identifier_list>>,
            space_s, closeparen
        > {};
        struct Direct_declarator_R : tao::pegtl::seq<
            space_s,
            tao::pegtl::sor<
                Direct_declarator_R_brack,
                Direct_declarator_R_paren
            >,
            tao::pegtl::opt<Direct_declarator_R>
        > {};

        struct Pointer : tao::pegtl::seq<
            star,
            space_s,
            tao::pegtl::opt<Pointer>
        > {};

        struct Parameter_declaration;
        struct Parameter_list : tao::pegtl::list<
            Parameter_declaration,
            tao::pegtl::pad<comma, space>
        > {};

        //struct Declaration_specifiers;
        struct Parameter_declaration : tao::pegtl::seq<
            Type_specifier,
            tao::pegtl::opt<spaces, Declarator>
        > {};

        struct Identifier_list : tao::pegtl::list<
            identifier,
            tao::pegtl::pad<comma, space>
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
        struct Compound_statement_end : tao::pegtl::seq<
            tao::pegtl::pad_opt<Block_item_list, space>,
            tao::pegtl::must<closecurly>
        > {};
        struct Compound_statement : tao::pegtl::if_must<
            opencurly,
            Compound_statement_end
        > {};

        struct Block_item;
        struct Block_item_list : tao::pegtl::list<Block_item, space_s> {};

        struct Block_item : tao::pegtl::sor<Declaration, Statement> {};

        struct Expression_statement : tao::pegtl::seq<
            tao::pegtl::opt<Expression, space_s>,
            semicolon
        > {};

        struct Selection_statement : tao::pegtl::seq<
            if_keyword, space_s,
            openparen, space_s,
            Expression, space_s,
            closeparen, space_s,
            Statement,
            tao::pegtl::opt<
                space_s,
                else_keyword, space_s,
                Statement
            >
        > {};

        struct While_statement_end : tao::pegtl::seq<
            space_s,
            openparen, space_s,
            Expression, space_s,
            closeparen, space_s,
            Statement
        > {};
        struct While_statement : tao::pegtl::if_must<while_keyword, While_statement_end> {};
        struct For_statement_end : tao::pegtl::seq<
            space_s,
            openparen, space_s,
            tao::pegtl::sor<
                Expression_statement,
                Declaration
            >,
            space_s,
            Expression_statement, space_s,
            tao::pegtl::opt<Expression, space_s>,
            closeparen, space_s,
            Statement
        > {};
        struct For_statement : tao::pegtl::if_must<for_keyword, For_statement_end> {};
        struct Iteration_statement : tao::pegtl::sor<
            While_statement,
            For_statement
        > {};

        struct Jump_statement : tao::pegtl::seq<
            tao::pegtl::seq<
                return_keyword,
                tao::pegtl::opt<spaces, Expression>
            >,
            space_s,
            tao::pegtl::must<semicolon>
        > {};

        struct External_declaration;
        struct External_declaration_list : tao::pegtl::list<
            External_declaration,
            spaces
        > {};

        struct External_declaration : tao::pegtl::seq<
            Type_specifier,
            spaces,
            Declarator,
            space_s,
            Compound_statement
        > {};
    }
    struct grammar : tao::pegtl::seq<space_s, External_declaration_list, space_s, tao::pegtl::eof> {};
}

#endif