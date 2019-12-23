#ifndef Parser_node_hpp
#define Parser_node_hpp

#include <iostream>
#include <type_traits>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "Grammar.hpp"

namespace Parser {
	using namespace std::literals::string_view_literals;
    enum class Marker {
        None,
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
        Int_number,
        Float_number,
		Paren,
		Curly,
		Brack
    };
    struct node : tao::pegtl::parse_tree::basic_node<node> {

        Marker marker;

        node(Marker marker = Marker::None) : basic_node(), marker(marker) {};
        /*
        static std::unique_ptr<node> error_node(std::unique_ptr<node> const& n) {
            std::unique_ptr<node> p = std::make_unique<node>(Marker::Error);
            p->children = std::move(n->children);
            p->m_begin = std::move(n->m_begin);
            p->m_end = std::move(n->m_end);
            return std::move(p);
        }*/
        static std::unique_ptr<node> paren_node(std::unique_ptr<node> const& n) {
            if (n->children[0]->marker == Marker::openparen && n->children.back()->marker == Marker::closeparen) {
                std::unique_ptr<node> p = std::make_unique<node>(Marker::Paren);
                p->m_begin = std::move(n->m_begin);
                p->m_end = std::move(n->m_end);
                auto const l = n->children.size();
                std::vector<std::unique_ptr<node>> v;
                for (uint64_t i = 1; i + 1 < l; i++) {
                    v.emplace_back(std::move(n->children[i]));
                }
                p->children = std::move(v);
                return std::move(p);
            }
            else return nullptr;
        }
        static std::unique_ptr<node> curly_node(std::unique_ptr<node> const& n) {
            if (n->children[0]->marker == Marker::opencurly && n->children.back()->marker == Marker::closecurly) {
                std::unique_ptr<node> p = std::make_unique<node>(Marker::Curly);
                p->m_begin = std::move(n->m_begin);
                p->m_end = std::move(n->m_end);
                auto const l = n->children.size();
                std::vector<std::unique_ptr<node>> v;
                for (uint64_t i = 1; i + 1 < l; i++) {
                    v.emplace_back(std::move(n->children[i]));
                }
                p->children = std::move(v);
                return std::move(p);
            }
            else return nullptr;
        }
        static std::unique_ptr<node> brack_node(std::unique_ptr<node> const& n) {
            if (n->children[0]->marker == Marker::openbrack && n->children.back()->marker == Marker::closebrack) {
                std::unique_ptr<node> p = std::make_unique<node>(Marker::Brack);
                p->children = std::move(n->children);
                p->m_begin = std::move(n->m_begin);
                p->m_end = std::move(n->m_end);
                auto const l = n->children.size();
                std::vector<std::unique_ptr<node>> v;
                for (uint64_t i = 1; i + 1 < l; i++) {
                    v.emplace_back(std::move(n->children[i]));
                }
                p->children = std::move(v);
                return std::move(p);
            }
            else return nullptr;
        }


        template<typename Rule, typename Input, typename... States>
        void start(Input const& in, States&&...) {
            set_type<Rule>();
            source = in.source();
            m_begin = tao::pegtl::internal::iterator(in.iterator());

            // TODO : Add information in marker.
            // If needed, add more else if constexpr rules.
            // Check https://github.com/taocpp/PEGTL/blob/master/doc/Parse-Tree.md
            //
            // Information on start:
            // All non-root nodes receive a call to start() when
            // a match is attempted for Rule in a parsing run...
            if constexpr (std::is_same_v<Rule, plusequal>) {
				marker = Marker::plusequal;
            }
            else if constexpr (std::is_same_v<Rule, minusequal>) {
				marker = Marker::minusequal;
            }
            else if constexpr (std::is_same_v<Rule, starequal>) {
				marker = Marker::starequal;
            }
            else if constexpr (std::is_same_v<Rule, slashequal>) {
				marker = Marker::slashequal;
            }
            else if constexpr (std::is_same_v<Rule, percentequal>) {
				marker = Marker::percentequal;
            }
            else if constexpr (std::is_same_v<Rule, equalequal>) {
				marker = Marker::equalequal;
            }
            else if constexpr (std::is_same_v<Rule, notequal>) {
				marker = Marker::notequal;
            }
            else if constexpr (std::is_same_v<Rule, lessequal>) {
				marker = Marker::lessequal;
            }
            else if constexpr (std::is_same_v<Rule, greaterequal>) {
				marker = Marker::greaterequal;
            }
            else if constexpr (std::is_same_v<Rule, plusplus>) {
				marker = Marker::plusplus;
            }
            else if constexpr (std::is_same_v<Rule, minusminus>) {
				marker = Marker::minusminus;
            }
            else if constexpr (std::is_same_v<Rule, andand>) {
				marker = Marker::andand;
            }
            else if constexpr (std::is_same_v<Rule, oror>) {
				marker = Marker::oror;
            }
            else if constexpr (std::is_same_v<Rule, equal>) {
				marker = Marker::equal;
            }
            else if constexpr (std::is_same_v<Rule, plus>) {
				marker = Marker::plus;
            }
            else if constexpr (std::is_same_v<Rule, minus>) {
				marker = Marker::minus;
            }
            else if constexpr (std::is_same_v<Rule, star>) {
				marker = Marker::star;
            }
            else if constexpr (std::is_same_v<Rule, slash>) {
				marker = Marker::slash;
            }
            else if constexpr (std::is_same_v<Rule, percent>) {
				marker = Marker::percent;
            }
            else if constexpr (std::is_same_v<Rule, backslash>) {
				marker = Marker::backslash;
            }
            else if constexpr (std::is_same_v<Rule, exclamation>) {
				marker = Marker::exclamation;
            }
            else if constexpr (std::is_same_v<Rule, question>) {
				marker = Marker::question;
            }
            else if constexpr (std::is_same_v<Rule, colon>) {
				marker = Marker::colon;
            }
            else if constexpr (std::is_same_v<Rule, less>) {
				marker = Marker::less;
            }
            else if constexpr (std::is_same_v<Rule, greater>) {
				marker = Marker::greater;
            }
            else if constexpr (std::is_same_v<Rule, openparen>) {
				marker = Marker::openparen;
            }
            else if constexpr (std::is_same_v<Rule, closeparen>) {
				marker = Marker::closeparen;
            }
            else if constexpr (std::is_same_v<Rule, opencurly>) {
				marker = Marker::opencurly;
            }
            else if constexpr (std::is_same_v<Rule, closecurly>) {
				marker = Marker::closecurly;
            }
            else if constexpr (std::is_same_v<Rule, openbrack>) {
				marker = Marker::openbrack;
            }
            else if constexpr (std::is_same_v<Rule, closebrack>) {
				marker = Marker::closebrack;
            }
            else if constexpr (std::is_same_v<Rule, dot>) {
				marker = Marker::dot;
            }
            else if constexpr (std::is_same_v<Rule, string_literal>) {
				marker = Marker::string_literal;
            }
            else if constexpr (std::is_same_v<Rule, break_keyword>) {
				marker = Marker::break_keyword;
            }
            else if constexpr (std::is_same_v<Rule, continue_keyword>) {
				marker = Marker::continue_keyword;
            }
            else if constexpr (std::is_same_v<Rule, else_keyword>) {
				marker = Marker::else_keyword;
            }
            else if constexpr (std::is_same_v<Rule, float_keyword>) {
				marker = Marker::float_keyword;
            }
            else if constexpr (std::is_same_v<Rule, for_keyword>) {
				marker = Marker::for_keyword;
            }
            else if constexpr (std::is_same_v<Rule, if_keyword>) {
				marker = Marker::if_keyword;
            }
            else if constexpr (std::is_same_v<Rule, int_keyword>) {
				marker = Marker::int_keyword;
            }
            else if constexpr (std::is_same_v<Rule, return_keyword>) {
				marker = Marker::return_keyword;
            }
            else if constexpr (std::is_same_v<Rule, void_keyword>) {
				marker = Marker::void_keyword;
            }
            else if constexpr (std::is_same_v<Rule, while_keyword>) {
				marker = Marker::while_keyword;
            }
            else if constexpr (std::is_same_v<Rule, Int_number>) {
				marker = Marker::Int_number;
            }
            else if constexpr (std::is_same_v<Rule, Float_number>) {
				marker = Marker::Float_number;
            }
			else {
				marker = Marker::None;
			}
        }

		template<typename Rule, typename Input, typename... States>
		void failure(Input const& in, States&&...) noexcept {
            std::cout << "Failed to match:\n" << in.position() << std::endl;
		}

        void dfs() {
            if (this->has_content()) std::cout << this->string_view() << std::endl;
            for (auto&& next : children) {
                next->dfs();
            }
        }

		void statement_dfs() {
			if ((this->marker == Marker::int_keyword || this->marker == Marker::float_keyword) && this->has_content())
				std::cout << this->string_view() << std::endl;
			for (auto&& next : this->children) {
				next->statement_dfs();
			}
		}
    };

    template<typename Rule>
    struct selector : std::false_type {};
    template<>
    struct selector<plusequal> : std::true_type {};
    template<>
    struct selector<minusequal> : std::true_type {};
    template<>
    struct selector<starequal> : std::true_type {};
    template<>
    struct selector<slashequal> : std::true_type {};
    template<>
    struct selector<percentequal> : std::true_type {};
    template<>
    struct selector<equalequal> : std::true_type {};
    template<>
    struct selector<notequal> : std::true_type {};
    template<>
    struct selector<lessequal> : std::true_type {};
    template<>
    struct selector<greaterequal> : std::true_type {};
    template<>
    struct selector<plusplus> : std::true_type {};
    template<>
    struct selector<minusminus> : std::true_type {};
    template<>
    struct selector<andand> : std::true_type {};
    template<>
    struct selector<oror> : std::true_type {};
    template<>
    struct selector<equal> : std::true_type {};
    template<>
    struct selector<plus> : std::true_type {};
    template<>
    struct selector<minus> : std::true_type {};
    template<>
    struct selector<star> : std::true_type {};
    template<>
    struct selector<slash> : std::true_type {};
    template<>
    struct selector<percent> : std::true_type {};
    template<>
    struct selector<backslash> : std::true_type {};
    template<>
    struct selector<exclamation> : std::true_type {};
    template<>
    struct selector<question> : std::true_type {};
    template<>
    struct selector<colon> : std::true_type {};
    template<>
    struct selector<less> : std::true_type {};
    template<>
    struct selector<greater> : std::true_type {};
    template<>
    struct selector<openparen> : std::true_type {};
    template<>
    struct selector<closeparen> : std::true_type {};
    template<>
    struct selector<opencurly> : std::true_type {};
    template<>
    struct selector<closecurly> : std::true_type {};
    template<>
    struct selector<openbrack> : std::true_type {};
    template<>
    struct selector<closebrack> : std::true_type {};
    template<>
    struct selector<dot> : std::true_type {};
    template<>
    struct selector<string_literal> : std::true_type {};

    template<>
    struct selector<break_keyword> : std::true_type {};
    template<>
    struct selector<continue_keyword> : std::true_type {};
    template<>
    struct selector<else_keyword> : std::true_type {};
    template<>
    struct selector<float_keyword> : std::true_type {};
    template<>
    struct selector<for_keyword> : std::true_type {};
    template<>
    struct selector<if_keyword> : std::true_type {};
    template<>
    struct selector<int_keyword> : std::true_type {};
    template<>
    struct selector<return_keyword> : std::true_type {};
    template<>
    struct selector<void_keyword> : std::true_type {};
    template<>
    struct selector<while_keyword> : std::true_type {};
    template<>
    struct selector<identifier> : std::true_type {};

    template<>
    struct selector<Int_number> : std::true_type {};
    template<>
    struct selector<Float_number> : std::true_type {};

    template<>
    struct selector<Primary_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
			if (n->children.size() == 1) {
                n = std::move(n->children[0]);
            }
            else if (n->children.size() == 3) {
                n = std::move(n->children[1]);
            }
            else throw std::exception();
        }
    };
    template<>
    struct selector<Postfix_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
			if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            n->children[0]->children.emplace_back(std::move(n->children[1]));
            n = std::move(n->children[0]);
        }
    };
    template<>
    struct selector<Array_subscript> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n = node::brack_node(n);
        }
    };
    template<>
    struct selector<Function_call> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n = node::paren_node(n);
        }
    };
	template<>
	struct selector<Unary_expression> : std::true_type {
		static void transform(std::unique_ptr<node>& n) {
			if (n->children.size() == 1) {
				n = std::move(n->children[0]);
                return;
			}
            auto u = std::move(n->children[0]);
            u->children.emplace_back(std::move(n->children[1]));
            n = std::move(u);
		}
	};
	template<>
	struct selector<Multiplicative_expression> : std::true_type {
		static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
			auto r = std::move(n->children.back());
			n->children.pop_back();
			auto o = std::move(n->children.back());
			n->children.pop_back();
			o->children.emplace_back(std::move(n));
			o->children.emplace_back(std::move(r));
			n = std::move(o);
            transform(n->children[0]);
		}
	};
    template<>
    struct selector<Additive_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto r = std::move(n->children.back());
            n->children.pop_back();
            auto o = std::move(n->children.back());
            n->children.pop_back();
            o->children.emplace_back(std::move(n));
            o->children.emplace_back(std::move(r));
            n = std::move(o);
            transform(n->children[0]);
        }
    };
    template<>
    struct selector<Relational_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto r = std::move(n->children.back());
            n->children.pop_back();
            auto o = std::move(n->children.back());
            n->children.pop_back();
            o->children.emplace_back(std::move(n));
            o->children.emplace_back(std::move(r));
            n = std::move(o);
            transform(n->children[0]);
        }
    };
    template<>
    struct selector<Equality_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto r = std::move(n->children.back());
            n->children.pop_back();
            auto o = std::move(n->children.back());
            n->children.pop_back();
            o->children.emplace_back(std::move(n));
            o->children.emplace_back(std::move(r));
            n = std::move(o);
            transform(n->children[0]);
        }
    };
    template<>
    struct selector<Logical_AND_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto r = std::move(n->children.back());
            n->children.pop_back();
            auto o = std::move(n->children.back());
            n->children.pop_back();
            o->children.emplace_back(std::move(n));
            o->children.emplace_back(std::move(r));
            n = std::move(o);
            transform(n->children[0]);
        }
    };
    template<>
    struct selector<Logical_OR_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto r = std::move(n->children.back());
            n->children.pop_back();
            auto o = std::move(n->children.back());
            n->children.pop_back();
            o->children.emplace_back(std::move(n));
            o->children.emplace_back(std::move(r));
            n = std::move(o);
            transform(n->children[0]);
        }
    };
    template<>
    struct selector<Conditional_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            n->children[1] = std::move(n->children[2]);
            n->children[2] = std::move(n->children[4]);
            n->children.resize(3);
        }
    };
    template<>
    struct selector<Assignment_expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto o = std::move(n->children[1]);
            o->children.emplace_back(std::move(n->children[0]));
            o->children.emplace_back(std::move(n->children[2]));
            n = std::move(o);
        }
    };
    template<>
    struct selector<Expression> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
            }
        }
    };
    
    template<>
    struct selector<Declaration> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            auto t = std::move(n->children[0]);
            auto l = std::move(n->children[1]);
            n = std::move(t);
            n->children = std::move(l->children);
        }
    };
    template<>
    struct selector<Init_declarator_list> : std::true_type {};
    template<>
    struct selector<Init_declarator> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto r = std::move(n->children[2]);
            auto o = std::move(n->children[1]);
            o->children.emplace_back(std::move(n->children[0]));
            o->children.emplace_back(std::move(r));
            n = std::move(o);
        }
    };
    template<>
    struct selector<Declarator> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            n->children[n->children.size() - 2]->children.emplace_back(std::move(n->children.back()));
            n->children.pop_back();
            transform(n);
        }
    };
    template<>
    struct selector<Direct_declarator> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }

        }
    };
    template<>
    struct selector<Direct_declarator_R_brack> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n = node::brack_node(n);
        }
    };
    template<>
    struct selector<Direct_declarator_R_paren> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n = node::paren_node(n);
        }
    };
    template<>
    struct selector<Parameter_declaration> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            auto l = std::move(n->children[0]);
            l->children.emplace_back(std::move(n->children[1]));
            n = std::move(l);
        }
    };
    template<>
    struct selector<Identifier_list> : std::true_type {};
    
    template<>
    struct selector<Compound_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n = node::curly_node(n);
        }
    };
    template<>
    struct selector<Expression_statement> : std::true_type {};
    template<>
    struct selector<Selection_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            auto fst = std::move(n->children[0]);
            auto trd = std::move(n->children[2]);
            auto fth = std::move(n->children[4]);
            if (n->children.size() == 5) {
                fst->children.emplace_back(std::move(trd));
                fst->children.emplace_back(std::move(fth));
            }
            else if (n->children.size() == 7) {
                auto sth = std::move(n->children[6]);
                fst->children.emplace_back(std::move(trd));
                fst->children.emplace_back(std::move(fth));
                fst->children.emplace_back(std::move(sth));
            }
            else throw std::exception();
            n = std::move(fst);
        }
    };
    template<>
    struct selector<While_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            auto fst = std::move(n->children[0]);
            fst->children.emplace_back(std::move(n->children[2]));
            fst->children.emplace_back(std::move(n->children[4]));
            n = std::move(fst);
        }
    };
    template<>
    struct selector<For_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            auto fst = std::move(n->children[0]);
            fst->children.emplace_back(std::move(n->children[2]));
            fst->children.emplace_back(std::move(n->children[3]));
            fst->children.emplace_back(std::move(n->children[4]));
            fst->children.emplace_back(std::move(n->children[6]));
            n = std::move(fst);
        }
    };
    template<>
    struct selector<Jump_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            auto fst = std::move(n->children[0]);
            if (n->children.size() == 3) {
                fst->children.emplace_back(std::move(n->children[1]));
            }
            n = std::move(fst);
        }
    };
    template<>
    struct selector<External_declaration> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            auto fst = std::move(n->children[0]);
            fst->children.emplace_back(std::move(n->children[1]));
            fst->children.emplace_back(std::move(n->children[2]));
            n = std::move(fst);
        }
    };
}

#endif