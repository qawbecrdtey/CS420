#ifndef syntax_tree_node_hpp
#define syntax_tree_node_hpp

#include <memory>
#include <vector>

#include "position.hpp"

struct syntax_tree_node {
	position pos;
	std::uint64_t length;
	std::vector<syntax_tree_node> children;

	syntax_tree_node(position const& pos, std::uint64_t length);
	syntax_tree_node(syntax_tree_node const &);
	syntax_tree_node(syntax_tree_node &&) noexcept;
	syntax_tree_node const &operator=(syntax_tree_node const &);
	syntax_tree_node const &operator=(syntax_tree_node &&) noexcept;
	~syntax_tree_node() = default;

	void add_node(std::unique_ptr<syntax_tree_node>&& node);
	std::uint64_t 
};

#endif