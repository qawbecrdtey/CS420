#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "ASCII.hpp"
#include "syntax_tree_node.hpp"

int main(int argc, char* argv[]) {

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
	std::vector<std::unique_ptr<syntax_tree_node>> token_vector;
	for (std::uint64_t i = 0; i < source.size();) {
		switch (determine_char(source[i])) {
		case ASCII::Minus: throw std::exception("No negative char should be in the code.");
		case ASCII::Null:
			throw std::exception("Check : The file has null symbol.");
		case ASCII::Control:
			throw std::exception("Not expected control.");
		case ASCII::Whitespace:
			i++;
			continue;
		case ASCII::Printable: {
			using P = std::pair<char, char>;
			bool flag = true;
			if (i + 1 < source.size()) {
				P const p = { source[i], source[i + 1] };
				flag = false;
				if (p.second == '=') {
					switch (p.first) {
					case '+':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("+="));
						break;
					case '-':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("-="));
						break;
					case '*':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("*="));
						break;
					case '/':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("/="));
						break;
					case '%':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("%="));
						break;
					case '=':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("=="));
						break;
					case '!':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("!="));
						break;
					case '<':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("<="));
						break;
					case '>':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(">="));
						break;
					default: flag = true;
					}
				}
				else if (p == P{ '+', '+' }) {

				}
				else if (p == P{ '-', '-' }) {

				}
				else if (p == P{ '/', '/' }) {

				}
				else if (p == P{ '&', '&' }) {

				}
				else if (p == P{ '|', '|' }) {

				}
				else flag = true;
			}
			if (flag) {
				switch (source[i]) {
				case '=':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("="));
					break;
				case '+':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("+"));
					break;
				case '-':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("-"));
					break;
				case '*':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("*"));
					break;
				case '/':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("/"));
					break;
				case '%':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("%"));
					break;
				case '\\':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("\\"));
					break;
				case '!':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("!"));
					break;
				case '<':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("<"));
					break;
				case '>':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(">"));
					break;
				case '(':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("("));
					break;
				case ')':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(")"));
					break;
				case '{':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("{"));
					break;
				case '}':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("}"));
					break;
				case '[':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("["));
					break;
				case ']':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("]"));
					break;
				case '\"': {
					int x = i + 1;
					while (x < source.size() && !(source[x] == '\"' && source[x-1] != '\\')) x++;
				}
				default:
					if (is_identifier_first(source[i])) {
						int x = i + 1;
						while (is_identifier_middle(source[x])) x++;
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(source.substr()));
					}
					else throw std::exception("Syntax error.");
				}
			}
		}
		}
	}
}