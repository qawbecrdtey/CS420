#ifndef Parser_Block_hpp
#define Parser_Block_hpp


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

	using Value = std::variant<value<int>, value<float>, value<int*>, value<float*>>;

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
		case 0:
			std::get<0>(val).set_value(t);
			break;
		case 1:
			std::get<1>(val).set_value(t);
			break;
		case 2:
			std::get<2>(val).set_value(t);
			break;
		case 3:
			std::get<3>(val).set_value(t);
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

	using identifier_map_stack = std::map<std::string, std::stack<Value_level>>;

	struct block {
		uint64_t level;
		identifier_map_stack ims;

		explicit block(uint64_t level) : level(level), ims() {}
		block(block const&) = default;
		block(block&&) = default;
		block& operator=(block const&) = default;
		block& operator=(block&&) = default;

		template<typename T>
		void initialize_identifier(uint64_t level, std::string id, T& t) {
			if (ims.find(id) != ims.end() && ims[id].top().level == level) {
				throw std::runtime_error("There already is a declaration of the identifier in the same level.");
			}
			ims[id].push(Value_level(value<T>, level));
		}
		template<typename T>
		void modify_identifier(std::string id, T t) {
			if (ims.find(id) == ims.end()) {
				throw std::runtime_error("Identifier not defined.");
			}
			set_value(ims[id].top(), t);
		}
	};
}

#endif