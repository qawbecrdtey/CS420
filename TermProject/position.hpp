#ifndef position_hpp
#define position_hpp

#include <cstddef>
#include <string_view>

#include "ASCII.hpp"

struct position {
	std::string_view data;
	std::uint64_t byte;
	std::uint64_t line;
	std::uint64_t col;

	position(std::string_view data, std::uint64_t start, std::uint64_t line, std::uint64_t col)
		: data(data), byte(start), line(line), col(col) {}
	position(position const &);
	position(position &&) noexcept;
	position const &operator=(position const &);
	position const &operator=(position &&) noexcept;
	~position() = default;
};

position operator+(position const &pos, std::uint64_t dist);

#endif