#ifndef Parser_Block_hpp
#define Parser_Block_hpp

#include <cassert>
#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace Parser {

	template<typename T>
	struct value {
		T val;

		value() = default;
		value(T const& t) : val(t) {};
		value(value const&) = default;
		value(value&&) = default;
		value& operator=(value const&) = default;
		value& operator=(value&&) = default;
		~value() = default;

		T get_value() const { return val; }

		template<typename U>
		void set_value(value<U>& u) {
			if constexpr (std::is_same_v<U, T>) {
				val = u.val;
			}
			else if constexpr (std::is_convertible_v<U, V>) {
				val = static_cast<T>(u.val);
			}
			else {
				throw std::runtime_error("Not convertible.");
			}
		}
	};
	template<>
	struct value<void> {};

	using Value = std::variant<value<void>, value<int>, value<float>, value<int*>, value<float*>>;

	template<typename T>
	void get_value(Value val, T& t) {
		switch (val.index()) {
		case 0:
			t = static_cast<T>(std::get<0>(val).get_value());
			break;
		case 1:
			t = static_cast<T>(std::get<1>(val).get_value());
			break;
		case 2:
			t = static_cast<T>(std::get<2>(val).get_value());
			break;
		case 3:
			t = static_cast<T>(std::get<3>(val).get_value());
			break;
		default:
			throw std::bad_variant_access();
		}
	}
	template<typename T>
	void set_value(Value val, T& t) {
		switch (val.index()) {
		case 1:
			std::get<1>(val).set_value(t);
			break;
		case 2:
			std::get<2>(val).set_value(t);
			break;
		case 3:
			std::get<3>(val).set_value(t);
			break;
		case 4:
			std::get<4>(val).set_value(t);
			break;
		default:
			throw std::bad_variant_access();
		}
	}

	struct Value_level {
		Value val;
		uint64_t level;

		Value_level(Value const& val, uint64_t level) : val(val), level(level) {};
		~Value_level() = default;
	};

	using identifier_map_stack = std::map<std::string_view, std::stack<Value_level>>;

	struct block {
		uint64_t level;
		identifier_map_stack ims;
		std::vector<std::shared_ptr<block>> next;

		explicit block(uint64_t level) : level(level), ims(), next() {}
		block(block const&) = default;
		block(block&&) = default;
		block& operator=(block const&) = default;
		block& operator=(block&&) = default;
		~block() = default;

		template<typename T>
		bool initialize_identifier(uint64_t level, std::string_view id, T t) {
			if (ims.find(id) != ims.end() && ims[id].top().level == level) {
				throw std::runtime_error("There already is a declaration of the identifier in the same level.");
			}
			ims[id].push(Value_level(value<T>(t), level));
		}
		template<typename T>
		bool modify_identifier(std::string id, T t) {
			if (ims.find(id) == ims.end()) {
				throw std::runtime_error("Identifier not defined.");
			}
			set_value(ims[id].top(), t);
		}
		void remove_current_level() {
			assert(level);
			for (auto& p : ims) {
				if (p.second.top().level == level) {
					p.second.pop();
					if (p.second.empty()) {
						ims.erase(p.first);
					}
				}
			}
		}

		// makes a copy of current block.
		void generate_next_block() {
			next.emplace_back(std::make_shared<block>(*this));
		}
		void connect_next_block(std::shared_ptr<block>& b) {
			next.emplace_back(b);
		}
		void reset_next_block() {
			next.clear();
		}
	};

	enum class TYPE : int { _void, _int, _float, _starint, _starfloat };

	template<typename ...Args>
	void function_block_helper(std::vector<std::pair<TYPE, std::string_view>> &v, TYPE t, std::string_view s, Args... args_t) {
		function_block_helper(v, args_t);
		v.emplace_back(std::make_pair(t, std::move(s)));
	}
	template<>
	void function_block_helper(std::vector<std::pair<TYPE, std::string_view>> &v, TYPE t, std::string_view s) {
		v.emplace_back(std::make_pair(t, std::move(s)));
	}

	struct function_block {
		TYPE return_type;
		std::string_view name;
		std::vector<std::pair<TYPE, std::string_view>> parameters;
		std::shared_ptr<block> block_head;

		function_block() = default;
		function_block(function_block const&) = delete;
		function_block(function_block&&) = delete;
		function_block& operator=(function_block const&) = delete;
		function_block& operator=(function_block&&) = delete;
		~function_block() = default;

		template<typename ...Args>
		void init_var(std::string_view name, TYPE return_t, Args... args_t) {
			this->name = name;
			return_type = return_t;
			function_block_helper(args_t...);
		}

		void init(std::string_view name, TYPE return_t, std::vector<std::pair<TYPE, std::string_view>> params) {
			return_type = return_t;
			this->name = name;
			parameters = std::move(params);
		}

		void init_block() {
			block_head = std::make_shared<block>(1);
			for (auto& p : parameters) {
				switch (static_cast<int>(p.first)) {
				case 1:
					block_head->initialize_identifier(1, p.second, static_cast<int>(0));
					break;
				case 2:
					block_head->initialize_identifier(1, p.second, static_cast<float>(0));
					break;
				case 3:
					block_head->initialize_identifier(1, p.second, static_cast<int*>(0));
					break;
				case 4:
					block_head->initialize_identifier(1, p.second, static_cast<float*>(0));
					break;
				default:
					throw std::runtime_error("Not possible");
				}
			}
		}
	};
}

#endif