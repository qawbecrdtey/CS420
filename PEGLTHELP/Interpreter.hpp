#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <map>
#include <variant>

#include "Parser/Node.hpp"

namespace interpreter {
	using namespace Parser;
	enum class TYPE { _void, _int, _float, _starint, _starfloat };
	
	struct int_value { int value; };
	struct float_value { float value; };
	struct starint_value { int* value; };
	struct starfloat_value { float* value; };
	using Value = std::variant<int_value, float_value, starint_value, starfloat_value>;
	using Identifier_map_type = std::map<std::string_view, std::stack<Value>>;

	void get_function_return_type_and_identifier_from_root(std::unique_ptr<node> const& root, std::map<std::string_view, std::pair<TYPE, std::vector<std::pair<TYPE, std::string_view>>>>& func_param) {
		for (auto&& p : root->children) {
			assert(p->marker == Marker::int_keyword || p->marker == Marker::float_keyword || p->marker == Marker::void_keyword);
			TYPE t;
			switch (p->marker) {
			case Marker::int_keyword:
				t = TYPE::_int;
				break;
			case Marker::float_keyword:
				t = TYPE::_float;
				break;
			case Marker::void_keyword:
				t = TYPE::_void;
				break;
			default:
				throw std::runtime_error("Not possible.");
			}
			auto&& q = p->children[0];
			auto id = q->children[0]->string_view();
			auto&& r = q->children[1];
			std::vector<std::pair<TYPE, std::string_view>> v;
			for (auto&& s : r->children) {
				if (s->marker == Marker::int_keyword) {
					if (s->children[0]->marker == Marker::star) {
						if (s->children[0]->children.empty()) {
							v.emplace_back(std::make_pair(TYPE::_starint, ""sv));
						}
						else {
							assert(s->children[0]->children[0]->marker == Marker::identifier);
							v.emplace_back(std::make_pair(TYPE::_starint, s->children[0]->children[0]->string_view()));
						}
					}
					else {
						if (r->children[0]->children.empty()) {
							v.emplace_back(std::make_pair(TYPE::_int, ""sv));
						}
						else {
							assert(s->children[0]->marker == Marker::identifier);
							v.emplace_back(std::make_pair(TYPE::_int, s->children[0]->string_view()));
						}
					}
				}
				else if (s->marker == Marker::float_keyword) {
					if (s->children[0]->marker == Marker::star) {
						if (s->children[0]->children.empty()) {
							v.emplace_back(std::make_pair(TYPE::_starfloat, ""sv));
						}
						else {
							assert(s->children[0]->children[0]->marker == Marker::identifier);
							v.emplace_back(std::make_pair(TYPE::_starfloat, s->children[0]->children[0]->string_view()));
						}
					}
					else {
						if (s->children[0]->children.empty()) {
							v.emplace_back(std::make_pair(TYPE::_float, ""sv));
						}
						else {
							assert(s->children[0]->marker == Marker::identifier);
							v.emplace_back(std::make_pair(TYPE::_float, s->children[0]->string_view()));
						}
					}
				}
				else if (s->marker == Marker::void_keyword) {
					assert(r->children.size() == 1);
					continue;
				}
				else {
					std::cout << "##############################" << std::endl;
					throw std::runtime_error(r->string_view().data());
				}
			}
			if (func_param.find(id) == func_param.end()) {
				func_param[id].first = t;
				func_param[id].second = std::move(v);
			}
			else throw std::runtime_error("Multiple parameter with same name in declaration.");
		}
	}

	bool find_function_get_statement(std::string_view name, std::unique_ptr<node> const& root, uint64_t& idx) {
		for (uint64_t i = 0; i < root->children.size(); i++) {
			if (name == root->children[i]->children[0]->children[0]->string_view()) {
				idx = i;
				return true;
			}
		}
		idx = UINTMAX_MAX;
		return false;
	}

	Value run_expression(std::unique_ptr<node> const& root, Identifier_map_type& imt) {
		return int_value{ 42 };
	}

	void run_statement(std::unique_ptr<node> const& root, Identifier_map_type& imt, uint64_t level) {
		if (root->marker == Marker::Curly) {
			for (auto&& p : root->children) {
				run_statement(root, imt, level + 1);
			}
		}
		if (root->marker == Marker::int_keyword || root->marker == Marker::float_keyword) {
			TYPE t;
			if (root->marker == Marker::int_keyword) t = TYPE::_int;
			else t = TYPE::_float;
			for (auto&& p : root->children) {
				if (p->marker == Marker::equal) {
					Value v = run_expression(p->children[1], imt);
					imt[p->children[0]->string_view()].push(v);
				}
				else {
					assert(p->marker == Marker::identifier);
					
				}
			}
		}
	}
};

#endif