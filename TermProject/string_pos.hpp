#ifndef string_pos_hpp
#define string_pos_hpp

#include <cinttypes>
#include <string_view>

struct string_pos {
	std::string_view base;
	std::string str;
	std::uint64_t byte;

	string_pos(std::string_view base, std::string str, std::uint64_t byte)
		: base(base), str(std::move(str)), byte(byte) {}
	string_pos(string_pos const &sp)
		: base(sp.base), str(sp.str), byte(sp.byte) {}
	string_pos(string_pos &&sp) noexcept
		: base(std::move(sp.base)), str(std::move(sp.str)), byte(sp.byte) {}
	string_pos const &operator=(string_pos const &sp) {
		if (&sp != this) {
			base = sp.base;
			str = sp.str;
			byte = sp.byte;
		}
		return *this;
	}
	string_pos const &operator=(string_pos &&sp) noexcept {
		if (&sp != this) {
			base = std::forward<std::string_view>(sp.base);
			str = std::forward<std::string_view>(sp.str);
			byte = sp.byte;
		}
		return *this;
	}

	std::uint64_t const length() const { return str.length(); }
};

#endif