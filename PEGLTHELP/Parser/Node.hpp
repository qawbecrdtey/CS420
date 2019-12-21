#ifndef Parser_node_hpp
#define Parser_node_hpp

#include <iostream>
#include <type_traits>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "Grammar.hpp"

namespace Parser {
	using namespace std::literals::string_view_literals;

    struct node : tao::pegtl::parse_tree::basic_node<node> {

		std::string_view corresponding_string;

		node() {

		}

        void dfs() {
            for (auto&& next : children) {
                next->dfs();
            }
            std::cout << this->type << ' ' << this->source << std::endl;
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
        }
    };
	template<>
	struct selector<Unary_expression> : std::true_type {
		static void transform(std::unique_ptr<node>& n) {
			if (n->children.size() == 1) {
				n = std::move(n->children[0]);
                return;
			}
            n->remove_content();
            auto r = std::move(n->children[1]);
            auto u = std::move(n->children[0]);
            n->children.clear();
            u->children.emplace_back(std::move(r));
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            n->remove_content();
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
            }/*
            n->remove_content();
            auto s = std::move(n->children[0]);
            s->children.emplace_back(std::move(n->children[1]));
            n = std::move(s);*/
        }
    };
    template<>
    struct selector<Direct_declarator> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
                return;
            }
            n->remove_content();

        }
    };
    template<>
    struct selector<Direct_declarator_R> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.back()->children.size()) {
                auto c = std::move(n->children.back()->children);
                n->children.pop_back();
                for (uint64_t i = 0; i < c.size(); i++) {
                    n->children.emplace_back(std::move(c[i]));
                }
            }
        }
    };
    template<>
    struct selector<Parameter_list> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            if (n->children.size() == 1) {
                n = std::move(n->children[0]);
            }
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
            n->remove_content();
            std::vector<std::unique_ptr<node>> v;
            v.reserve(n->children.size() - 2);
            for (uint64_t i = 1; i < n->children.size() - 1; i++) {
                v.emplace_back(std::move(n->children[i]));
            }
            n->children = std::move(v);
        }
    };
    template<>
    struct selector<Expression_statement> : std::true_type {};
    template<>
    struct selector<Selection_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n->remove_content();
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
            n->remove_content();
            auto fst = std::move(n->children[0]);
            fst->children.emplace_back(std::move(n->children[2]));
            fst->children.emplace_back(std::move(n->children[4]));
            n = std::move(fst);
        }
    };
    template<>
    struct selector<For_statement> : std::true_type {
        static void transform(std::unique_ptr<node>& n) {
            n->remove_content();
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