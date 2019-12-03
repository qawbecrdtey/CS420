#ifndef syntax_tree_node_hpp
#define syntax_tree_node_hpp

#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>

#include "string_pos.hpp"

struct syntax_tree_node {
	using node_ptr = std::unique_ptr<syntax_tree_node>;
	std::string_view data;
	std::vector<node_ptr> children;

	syntax_tree_node(std::string_view data) : data(data) {}

	void add_node(node_ptr&& node) {
		children.emplace_back(std::move(node));
	}
};

#endif