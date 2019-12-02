#ifndef syntax_tree_node_hpp
#define syntax_tree_node_hpp

#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>

#include "string_pos.hpp"

template<typename Type, typename ...Args>
struct construct_vector;
template<typename Type, typename T, typename ...Args>
struct construct_vector<Type, T, Args...> {
	static_assert(std::is_same_v<std::remove_cv<Type>, std::remove_cv<T>>, "T is not of type Type.");

	static std::vector<Type>&& func(T&& arg, Args&&... args) {
		std::vector<Type> v;
		v.reserve(sizeof...(Args) + 1);
		std::stack<Type> s;
		helper(s, arg, args...);
		while (!s.empty()) {
			v.emplace_back(std::move(s.top()));
			s.pop();
		}
		return std::move(v);
	}

private:
	static void helper(std::stack<Type>& s, T&& arg, Args&&... args) {
		construct_vector<...Args>::helper(s, args);
		s.push(std::move(arg));
	}
};
template<typename Type>
struct construct_vector<Type> {
	static std::vector<Type>&& func() { return std::move(std::vector<Type>()); }

private:
	static void helper(std::stack<Type>&) {}
};

struct syntax_tree_node {
	std::vector<std::unique_ptr<syntax_tree_node>> children;

	explicit syntax_tree_node(std::int64_t size) {
		if (size > -1) children.reserve(size);
	}

	/// Args... are type of std::unique_ptr&lt;syntax_tree_node&gt;.
	template<typename ...Args>
	explicit syntax_tree_node(Args&&... args) {
		auto&& v = construct_vector<std::unique_ptr<syntax_tree_node>, Args...>::func(args...);
		std::exchange(children, v);
	}
	syntax_tree_node(syntax_tree_node const&) = delete;
	syntax_tree_node(syntax_tree_node&& stn) noexcept {
		children.reserve(stn.children.size());
		for (auto&& e : stn.children) {
			children.emplace_back(std::move(e));
		}
	}
	syntax_tree_node const& operator=(syntax_tree_node const&) = delete;
	syntax_tree_node const& operator=(syntax_tree_node&&) = delete;
	~syntax_tree_node() = default;

	void add_node(std::unique_ptr<syntax_tree_node>&& node) {
		children.emplace_back(std::move(node));
	}
};

#endif