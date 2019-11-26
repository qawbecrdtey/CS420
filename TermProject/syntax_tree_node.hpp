#ifndef syntax_tree_node_hpp
#define syntax_tree_node_hpp

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "string_pos.hpp"

struct syntax_tree_node {
	std::vector<std::unique_ptr<syntax_tree_node>> children;

	explicit syntax_tree_node(std::int64_t size) {
		if (size > -1) children.reserve(size);
	}
	syntax_tree_node(syntax_tree_node const &stn) {
		children = stn.children;
	}
	syntax_tree_node(syntax_tree_node &&stn) noexcept {
		std::exchange(children, stn.children);
	}
	syntax_tree_node const &operator=(syntax_tree_node const &stn) {
		if (&stn != this) children = stn.children;
		return *this;
	}
	syntax_tree_node const &operator=(syntax_tree_node &&stn) noexcept {
		if (&stn != this) std::exchange(children, stn.children);
		return *this;
	}
	~syntax_tree_node() = default;

	void add_node(std::unique_ptr<syntax_tree_node> &&node);
	virtual void parse(std::vector<std::string> const &) = 0;
};

#endif