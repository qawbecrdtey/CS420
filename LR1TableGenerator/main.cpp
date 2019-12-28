#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
/*

input format given as:

nonterminal ::= symbol ... symbol | symbol ... symbol | ... | symbol ... symbol >.<
...
nonterminal ::= ... >.<

start_symbol >.<

WARNING : The input file should be LF, not CRLF.

*/

struct product {
private:
	static bool vector_string_le(std::vector<std::string> const& v1, std::vector<std::string> const& v2) {
		auto const size = v1.size() > v2.size() ? v2.size() : v1.size();
		for (uint64_t i = 0; i < size; i++) {
			if (v1[i] <= v2[i]) return true;
		}
		return false;
	}
public:
	std::string from;
	std::vector<std::string> to;
	
	product(std::string from, std::vector<std::string> to = {}) : from(std::move(from)), to(std::move(to)) {}
	product(product const& prod) : from(prod.from), to(prod.to) {}
	product(product&& prod) noexcept : from(std::move(prod.from)), to(std::move(prod.to)) {}

	friend bool operator<(product const& prod1, product const& prod2) {
		return (prod1.from < prod2.from) || (prod1.from == prod2.from && vector_string_le(prod1.to, prod2.to));
	}
	friend bool operator<=(product const& prod1, product const& prod2) {
		return !(prod2 < prod1);
	}
	friend bool operator>(product const& prod1, product const& prod2) {
		return prod2 < prod1;
	}
	friend bool operator>=(product const& prod1, product const& prod2) {
		return !(prod1 < prod2);
	}
};

struct item : public product {
	uint64_t dot;
	std::string lookahead;

	item(std::string from, std::vector<std::string> to = {}, std::string lookahead = "", uint64_t dot = 0)
		: product(std::move(from), std::move(to)), lookahead(std::move(lookahead)), dot(dot) { assert(dot <= to.size()); }
	item(product prod, std::string lookahead = "", uint64_t dot = 0)
		: product(std::move(prod)), lookahead(std::move(lookahead)), dot(dot) { assert(dot <= to.size()); }

	item next() const { return item(from, to, lookahead, dot + 1); }
};

struct grammar {
	std::vector<std::string> token;
	std::vector<std::string> nonterminal;
	std::vector<product> products;
	std::string start_symbol;

	std::map<std::string, std::vector<std::string>> first_set;

	void sort_all() {
		std::sort(token.begin(), token.end());
		std::sort(nonterminal.begin(), nonterminal.end());
		std::sort(products.begin(), products.end());
	}

	bool is_nonterminal(std::string const& s) const {
		return std::binary_search(nonterminal.begin(), nonterminal.end(), s);
	}

	void find_first(std::string const& s, std::string const& cur) { // DFS
		if (s == cur) return;
		if (!is_nonterminal(s)) {
			first_set[s].emplace_back(cur);
			return;
		}

		// products is sorted, so we may use binary search to find s in from of products.

		uint64_t l = 0, r = products.size();
		while (l < r) {
			auto const m = (l + r) / 2;
			if (products[m].from < s) {
				l = m + 1;
			}
			else {
				r = m;
			}
		}
		auto const lo = l; // lower bound of s
		l = 0, r = products.size();
		while (l < r) {
			auto const m = (l + r) / 2;
			if (products[m].from <= s) {
				l = m + 1;
			}
			else {
				r = m;
			}
		}
		auto const hi = l; // upper bound of s

		// 

		for (auto i = lo; i < hi; i++) {
			for (uint64_t j = 0; j < products[i].to.size(); j++) {

			}
		}
	}

	/// https://www.geeksforgeeks.org/program-calculate-first-follow-sets-given-grammar/
	grammar(std::vector<std::string> token, std::vector<std::string> nonterminal, std::vector<product> products, std::string start_symbol)
		: token(std::move(token)), nonterminal(std::move(nonterminal)), products(std::move(products)), start_symbol(std::move(start_symbol)) {
		sort_all();
		auto const count = products.size();

		for (uint64_t i = 0; i < count; i++) {
			auto const s = nonterminal[i];
			uint64_t point2 = 0;
		}
	}
	
	
};

int main(int argc, char* argv[]) {
	using namespace std::literals;
	/* FILE INPUT */

	std::string input_file_name = "../input.txt";
	std::string source;
	// The first element denotes the input file.
	// If no first element exists, then the default is "input.txt".
	if (argc < 2) {
		std::cout << "Using input file ../input.txt as default." << std::endl;
	}
	else {
		std::cout << "Using input file " << (input_file_name = argv[1]) << '.' << std::endl;
	}

	std::ifstream in(input_file_name);
	// If there is no input file with such name, exit the program.
	if (!in) {
		std::cerr << "Couldn't find file " << input_file_name << "!" << std::endl;
		return 1;
	}
	// Saves the input into source.
	in.seekg(0, std::ios::end);
	source.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&source[0], source.size());
	in.close();

	/* FILE INPUT END */

	std::cout << source << std::endl;

	std::stringstream ss;
	ss << source;
	std::string start_symbol;
	std::vector<product> product_vector;
    while(true) {
		std::string nonterm, sym;
		ss >> nonterm >> sym;
		std::cout << nonterm << ' ' << sym << std::endl;
		if (sym == ">.<") {
			start_symbol = nonterm;
			break;
		}
		else if (sym != "::=") {
			return 1;
		}
		std::string const from = nonterm;
		std::vector<std::string> v;
		while (true) {
			ss >> nonterm;
			if (nonterm == "|") {
				product prod(from);
				std::swap(prod.to, v);
				product_vector.emplace_back(prod);
				continue;
			}
			else if (nonterm == ">.<") {
				product prod(from);
				std::swap(prod.to, v);
				product_vector.emplace_back(prod);
				break;
			}
			v.emplace_back(nonterm);
		}
    }

	std::cout << "Number of productions : " << product_vector.size() << std::endl;
	for (auto&& now : product_vector) {
		std::cout << now.from << " ::= ";
		for (auto& cur : now.to) {
			std::cout << cur << ' ';
		}
		std::cout << std::endl;
	}
}