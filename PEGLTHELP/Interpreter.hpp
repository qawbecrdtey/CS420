#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <map>

#include "Parser/Node.hpp"

namespace interpreter {
	using namespace Parser;
	enum class TYPE { _int, _float, _starint, _starfloat };
	void get_function_return_type_and_identifier_from_root(std::unique_ptr<node> const& root, std::map<std::string_view, std::vector<std::pair<TYPE, std::string_view>>>& func_param) {
		for (auto&& p : root->children) {
			assert(p->marker == Marker::int_keyword || p->marker == Marker::float_keyword || p->marker == Marker::void_keyword);
			TYPE t;
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
				func_param[id] = std::move(v);
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

	void run_statement() {

	}
};

#endif