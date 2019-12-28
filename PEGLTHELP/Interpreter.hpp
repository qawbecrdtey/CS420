#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <functional>
#include <map>
#include <variant>

#include "Parser/Node.hpp"

namespace interpreter {
	using namespace Parser;
	enum class TYPE { _void, _int, _float, _starint, _starfloat };

	struct int_value {
		int value;
		int_value(int value = 0) : value(value) {}
		~int_value() = default;
	};
	struct float_value {
		float value;
		float_value(float value = 0.0) : value(value) {}
		~float_value() = default;
	};
	struct starint_value {
		std::shared_ptr<int[]> value;
		explicit starint_value(uint64_t len = 0) : value(new int[len]) {}
		starint_value(std::shared_ptr<int[]> const& p) : value(p) {}
		starint_value(std::shared_ptr<int[]>&& p) : value(std::move(p)) {}
		starint_value& operator=(std::shared_ptr<int[]> const& p) { value = p; }
		starint_value& operator=(std::shared_ptr<int[]>&& p) { value = std::move(p); }

		~starint_value() = default;
	};
	struct starfloat_value {
		std::shared_ptr<float[]> value;
		explicit starfloat_value(uint64_t len = 0) : value(new float[len]) {}
		starfloat_value(std::shared_ptr<float[]> const& p) : value(p) {}
		starfloat_value(std::shared_ptr<float[]>&& p) : value(p) {}
		starfloat_value& operator=(std::shared_ptr<float[]> const& p) { value = p; }
		starfloat_value& operator=(std::shared_ptr<float[]>&& p) { value = std::move(p); }

		~starfloat_value() = default;
	};
	using Value = std::variant<int_value, float_value, starint_value, starfloat_value>;

	// map(identifier -> stack(value, level))
	using Identifier_map_type = std::map<std::string_view, std::stack<std::pair<Value, uint64_t>>>;
	using func_param_type = std::map<std::string_view, std::pair<TYPE, std::vector<std::pair<TYPE, std::string_view>>>>;

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
			return float_value(0.0);
		case TYPE::_int:
			return int_value(0);
		case TYPE::_starfloat:
			return starfloat_value(0);
		case TYPE::_starint:
			return starint_value(0);
		default:
			throw std::runtime_error("Not possible.");
		}
	}

	bool is_zero(Value const& v) {
		switch (v.index()) {
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

	Value set_value_float(float val) { return float_value(val); }
	Value set_value_int(int val) { return int_value(val); }
	Value set_value_starfloat(int len) { return starfloat_value(len); }
	Value set_value_starint(int len) { return starint_value(len); }

	template<typename T>
	Value set_value(T val) {
		if constexpr (std::is_same_v<T, int>) return set_value_int(val);
		else if constexpr (std::is_same_v<T, float>) return set_value_float(val);
		else if constexpr (std::is_same_v<T, int*>) return set_value_starint(val);
		else if constexpr (std::is_same_v<T, float*>) return set_value_starfloat(val);
		else throw std::runtime_error("Not possible.");
	}

	Value run_expression(std::unique_ptr<node> const& root, Identifier_map_type& imt, func_param_type const& func_param) {
		// int
		if (root->marker == Marker::Int_number) {
			return int_value(std::stoi(root->string()));
		}
		// float
		else if (root->marker == Marker::Float_number) {
			return float_value(std::stof(root->string()));
		}
		// identifier
		else if (root->marker == Marker::identifier) {
			if ((imt.find(root->string_view()) == imt.end()) && (func_param.find(root->string_view()) == func_param.end())) {
				throw std::runtime_error("Identifier not defined.");
			}
			else if (imt.find(root->string_view()) != imt.end()) {
				auto&& val = imt[root->string_view()].top().first;
				Value v;
				switch (val.index()) {
				case 0:
					v = int_value(std::get<0>(val).value);
					if (root->children.size() == 1) return v;
					else if (root->children.size() == 2) {
						auto&& p = root->children[1];
						if (p->marker == Marker::Brack) {
							throw std::runtime_error("int does not have operator[].");
						}
						else if (p->marker == Marker::Paren) {
							throw std::runtime_error("int does not have operator().");
						}
						else if (p->marker == Marker::plusplus) {
							std::get<0>(val).value++;
							return v;
						}
						else if (p->marker == Marker::minusminus) {
							std::get<0>(val).value--;
							return v;
						}
					}
					else throw std::runtime_error("Not possible.");
					break;
				case 1:
					v = float_value(std::get<1>(val).value);
					if (root->children.size() == 1) return v;
					else if (root->children.size() == 2) {
						throw std::runtime_error("Not possible.");
					}
					else throw std::runtime_error("Not possible.");
					break;
				case 2:
				case 3:
					v = val;
					if (root->children.size() == 1) return v;
					else if (root->children.size() == 2) {
						throw std::runtime_error("Not possible.");
					}
					else throw std::runtime_error("Not possible.");
					break;
				default:
					throw std::runtime_error("Not possible.");
				}
			}
		}

		// plus
		else if (root->marker == Marker::plus && root->children.size() == 2) {
			Value const l = run_expression(root->children[0], imt, func_param);
			Value const r = run_expression(root->children[1], imt, func_param);

			switch (l.index()) {
			case 0:
			{
				auto const x = std::get<0>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x + y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x + y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot add int with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 1:
			{
				auto const x = std::get<1>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x + y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x + y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot add float with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 2:
			case 3:
				throw std::runtime_error("Cannot add pointers with other types.");
			default:
				throw std::bad_variant_access();
			}
		}

		// minus
		else if (root->marker == Marker::minus && root->children.size() == 2) {
			Value const l = run_expression(root->children[0], imt, func_param);
			Value const r = run_expression(root->children[1], imt, func_param);

			switch (l.index()) {
			case 0:
			{
				auto const x = std::get<0>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x - y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x - y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot subtract int with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 1:
			{
				auto const x = std::get<1>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x - y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x - y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot subtract float with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 2:
			case 3:
				throw std::runtime_error("Cannot subtract pointers with other types.");
			default:
				throw std::bad_variant_access();
			}
		}

		// multiply
		else if (root->marker == Marker::star && root->children.size() == 2) {
			Value const l = run_expression(root->children[0], imt, func_param);
			Value const r = run_expression(root->children[1], imt, func_param);

			switch (l.index()) {
			case 0:
			{
				auto const x = std::get<0>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x * y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x * y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot multiply int with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 1:
			{
				auto const x = std::get<1>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x * y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x * y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot multiply float with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 2:
			case 3:
				throw std::runtime_error("Cannot multiply pointers with other types.");
			default:
				throw std::bad_variant_access();
			}
		}

		// divide
		else if (root->marker == Marker::slash && root->children.size() == 2) {
			Value const l = run_expression(root->children[0], imt, func_param);
			Value const r = run_expression(root->children[1], imt, func_param);

			if (is_zero(r)) throw std::runtime_error("Cannot divide with 0.");

			switch (l.index()) {
			case 0:
			{
				auto const x = std::get<0>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x / y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x / y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot divide int with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 1:
			{
				auto const x = std::get<1>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x / y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x / y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot divide float with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 2:
			case 3:
				throw std::runtime_error("Cannot divide pointers with other types.");
			default:
				throw std::bad_variant_access();
			}
		}

		// remainder
		else if (root->marker == Marker::percent && root->children.size() == 2) {
			Value const l = run_expression(root->children[0], imt, func_param);
			Value const r = run_expression(root->children[1], imt, func_param);

			if (is_zero(r)) throw std::runtime_error("Cannot remainder with 0.");

			switch (l.index()) {
			case 0:
			{
				auto const x = std::get<0>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x % y);
				}
				case 1:
					throw std::runtime_error("Cannot remainder with float.");
				case 2:
				case 3:
					throw std::runtime_error("Cannot remainder with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 1:
			{
				auto const x = std::get<1>(l).value;
				switch (r.index()) {
				case 0:
				{
					auto const y = std::get<0>(r).value;
					return set_value(x / y);
				}
				case 1:
				{
					auto const y = std::get<1>(r).value;
					return set_value(x / y);
				}
				case 2:
				case 3:
					throw std::runtime_error("Cannot remainder float with pointers.");
				default:
					throw std::bad_variant_access();
				}
			}
			case 2:
			case 3:
				throw std::runtime_error("Cannot remainder pointers with other types.");
			default:
				throw std::bad_variant_access();
			}
		}


		else if (root->marker == Marker::equal) {
			auto&& p = root->children[0];
			assert(p->marker == Marker::identifier);
			if (imt.find(p->string_view()) == imt.end()) {

			}
			if (!p->children.empty()) {

			}
		}
		return int_value{ 0 };
	}

	void run_statement(std::unique_ptr<node> const& root, Identifier_map_type& imt, func_param_type const& func_param, uint64_t level) {
		// Compound statement
		if (root->marker == Marker::Curly) {
			for (auto&& p : root->children) {
				run_statement(p, imt, func_param, level + 1);
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
					imt[sv].push(std::make_pair(run_expression(p->children[1], imt, func_param), level));
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
			Value v = run_expression(root->children[0], imt, func_param);
			if (!is_zero(v)) {
				run_statement(root->children[1], imt, func_param, level);
			}
			if (root->children.size() == 3 && is_zero(v)) {
				run_statement(root->children[2], imt, func_param, level);
			}
		}

		// while
		else if (root->marker == Marker::while_keyword) {
			while (true) {
				if (!is_zero(run_expression(root->children[0], imt, func_param))) run_statement(root->children[1], imt, func_param, level);
				else break;
			}
		}

		// for
		else if (root->marker == Marker::for_keyword) {
			run_statement(root->children[0], imt, func_param, level + 1);
			run_statement(root->children[1], imt, func_param, level + 1);
			if (root->children.size() == 3) {
				run_statement(root->children[2], imt, func_param, level + 1);
			}
			else if (root->children.size() == 4) {
				run_expression(root->children[2], imt, func_param);
				run_statement(root->children[2], imt, func_param, level + 1);
			}
			else throw std::runtime_error("Not possible; for statement mismatch.");
		}
	}
}

#endif