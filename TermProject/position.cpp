#include "position.hpp"

position::position(position const &pos) : data(pos.data), byte(pos.byte), line(pos.line), col(pos.col) {}

position::position(position &&pos) noexcept : data(std::move(pos.data)), byte(pos.byte), line(pos.line), col(pos.col) {}

position const &position::operator=(position const &pos) {
	this->data = pos.data;
	this->byte = pos.byte;
	this->line = pos.line;
	this->col = pos.col;
	return *this;
}

position const &position::operator=(position &&pos) noexcept {
	std::exchange(this->data, pos.data);
	this->byte = pos.byte;
	this->line = pos.line;
	this->col = pos.col;
	return *this;
}

position operator+(position const &pos, std::uint64_t dist) {
	position p(pos);
	for (auto i = p.byte; i < p.byte + dist; i++) {
		if (p.data[i] == '\n') {
			p.line++;
			p.col = 0;
		}
		else p.col++;
	}
	return p;
}