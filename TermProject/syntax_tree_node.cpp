#include "syntax_tree_node.hpp"

syntax_tree_node::syntax_tree_node(position const& pos, std::uint64_t length)
	: pos(pos), length(length) {}

syntax_tree_node::syntax_tree_node(syntax_tree_node const &node)
	: pos(node.pos), length(node.length), children(node.children) {}

syntax_tree_node::syntax_tree_node(syntax_tree_node &&node) noexcept
	: pos(std::move(node.pos)), length(node.length), children(std::move(node.children)) {}

syntax_tree_node const &syntax_tree_node::operator=(syntax_tree_node const &node) {
	this->pos = node.pos;
	this->length = node.length;
	this->children = node.children;
	return *this;
}

syntax_tree_node const &syntax_tree_node::operator=(syntax_tree_node &&node) noexcept {
	std::swap(this->pos, node.pos);
	this->length = node.length;
	std::swap(this->children, node.children);
	return *this;
}

void syntax_tree_node::add_node(std::unique_ptr<syntax_tree_node> &&node) {
	children.emplace_back(std::move(node));
}
