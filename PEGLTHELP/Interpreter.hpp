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
	// map(identifier -> stack(value, level))
	using Identifier_map_type = std::map<std::string_view, std::stack<std::pair<Value, uint64_t>>>;

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

	Value initialize_value(TYPE t) {
		switch (t) {
		case TYPE::_float:
			return float_value{ 0.0 };
		case TYPE::_int:
			return int_value{ 0 };
		case TYPE::_starfloat:
			return starfloat_value{ nullptr };
		case TYPE::_starint:
			return starint_value{ nullptr };
        default:
            throw std::runtime_error("Not possible.");
		}
	}

	bool is_zero(Value const& v) {
	    switch(v.index()) {
        case 0:
            return std::get<0>(v).value == 0;
        case 1:
            return std::get<1>(v).value == 0.0;
        case 2:
            return std::get<2>(v).value == nullptr;
        case 3:
            return std::get<3>(v).value == nullptr;
        default:
            throw std::runtime_error("Not possible.");
	    }
	}

	Value set_value_float(float val) { return float_value{ val }; }
	Value set_value_int(int val) { return int_value{ val }; }
	Value set_value_starfloat(int len) { return starfloat_value{ new float[len] }; }
	Value set_value_starint(int len) { return starint_value{ new int[len] }; }

	Value run_expression(std::unique_ptr<node> const& root, Identifier_map_type& imt) {
		// int
	    if(root->marker == Marker::Int_number) {
			return int_value{ std::stoi(root->string()) };
	    }
		// float
		else if (root->marker == Marker::Float_number) {
			return float_value{ std::stof(root->string()) };
		}
		// identifier
		else if (root->marker == Marker::identifier) {
			if (imt.find(root->string_view()) == imt.end()) {
				throw std::runtime_error("Identifier not defined.");
			}
			auto&& v = imt[root->string_view()].top().first;
			switch (v.index()) {
			case 0:
				return int_value{ std::get<0>(v).value };
			case 1:
				return float_value{ std::get<1>(v).value };
			case 2:
				return starint_value{ std::get<2>(v).value };
			case 3:
				return starfloat_value{ std::get<3>(v).value };
			default:
				throw std::runtime_error("Not possible.");
			}
		}



		if (root->marker == Marker::equal) {
		    auto&& p = root->children[0];
		    assert(p->marker == Marker::identifier);
		    if (imt.find(p->string_view()) == imt.end()) {

		    }
            if (!p->children.empty()) {

            }
		}
		return int_value{0};
	}

	void run_statement(std::unique_ptr<node> const& root, Identifier_map_type& imt, uint64_t level) {
		// Compound statement
		if (root->marker == Marker::Curly) {
			for (auto&& p : root->children) {
				run_statement(p, imt, level + 1);
			}
		}

		// Declaration
		else if (root->marker == Marker::int_keyword || root->marker == Marker::float_keyword) {
			TYPE t;
			if (root->marker == Marker::int_keyword) t = TYPE::_int;
			else t = TYPE::_float;
			for (auto&& p : root->children) {
				if (p->marker == Marker::equal) {
					auto sv = p->children[0]->string_view();
					if (imt.find(sv) != imt.end() && imt[sv].top().second == level) {
						throw std::runtime_error("Redefinition of identifier.");
					}
					imt[sv].push(std::make_pair(run_expression(p->children[1], imt), level));
				}
				else {
					assert(p->marker == Marker::identifier); // By the assignment specification; there will be no pointer definition.
					if (!p->children.empty()) {
					    auto sv = p->string_view();
                        if (imt.find(sv) != imt.end() && imt[sv].top().second == level) {
                            throw std::runtime_error("Redefinition of identifier.");
                        }
						if (t == TYPE::_int) {
                            imt[sv].push(std::make_pair(set_value_starint(std::stoi(p->children[0]->string())), level));
						}
						else {
                            imt[sv].push(std::make_pair(set_value_starfloat(std::stoi(p->children[0]->string())), level));
						}
					}
					else {
                        auto sv = p->string_view();
                        if (imt.find(sv) != imt.end() && imt[sv].top().second == level) {
                            throw std::runtime_error("Redefinition of identifier.");
                        }
                        imt[sv].push(std::make_pair(initialize_value(t), level));
					}
				}
			}
		}

		// if
		else if (root->marker == Marker::if_keyword) {
		    Value v = run_expression(root->children[0], imt);
            if(!is_zero(v)) {
                run_statement(root->children[1], imt, level);
            }
            if(root->children.size() == 3 && is_zero(v)) {
                run_statement(root->children[2], imt, level);
            }
		}

		// while
		else if (root->marker == Marker::while_keyword) {
		    while(true) {
                if (!is_zero(run_expression(root->children[0], imt))) run_statement(root->children[1], imt, level);
                else break;
            }
		}

		// for
		else if (root->marker == Marker::for_keyword) {
            run_statement(root->children[0], imt, level + 1);
            run_statement(root->children[1], imt, level + 1);
            if(root->children.size() == 3) {
                run_statement(root->children[2], imt, level + 1);
            }
            else if (root->children.size() == 4) {
                run_expression(root->children[2], imt);
                run_statement(root->children[2], imt, level + 1);
            }
            else throw std::runtime_error("Not possible; for statement mismatch.");
		}
	}
}

#endif