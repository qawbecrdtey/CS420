#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "ASCII.hpp"
#include "syntax_tree_node.hpp"

inline std::string_view substr(std::string_view base, std::uint64_t pos = 0, std::uint64_t len = std::string_view::npos) { return base.substr(pos, len); }

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
	std::cout << "Length: " << source.length() << std::endl;
	std::vector<std::unique_ptr<syntax_tree_node>> token_vector;
	for (std::uint64_t i = 0; i < source.size();) {
		std::cout << i << ' ' << source[i] << std::endl;
		switch (determine_char(source[i])) {
		case ASCII::Minus: i++;  continue;
		case ASCII::Null: goto A;
		case ASCII::Control: i++; continue;
		case ASCII::Whitespace: while(determine_char(source[i]) == ASCII::Whitespace) i++; continue;
		case ASCII::Printable:
		{
			using P = std::pair<char, char>;
			bool flag = true;
			if (i + 1 < source.size()) {
				P const p = { source[i], source[i + 1] };
				flag = false;
				if (p.second == '=') {
					switch (p.first) {
					case '+':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("+="sv)); i += 2;
						break;
					case '-':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("-="sv)); i += 2;
						break;
					case '*':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("*="sv)); i += 2;
						break;
					case '/':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("/="sv)); i += 2;
						break;
					case '%':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("%="sv)); i += 2;
						break;
					case '=':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("=="sv)); i += 2;
						break;
					case '!':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("!="sv)); i += 2;
						break;
					case '<':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("<="sv)); i += 2;
						break;
					case '>':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(">="sv)); i += 2;
						break;
					default: flag = true;
					}
				}
				else if (p == P{ '+', '+' }) {

					i += 2;
				}
				else if (p == P{ '-', '-' }) {

					i += 2;
				}
				else if (p == P{ '/', '/' }) {

					i += 2;
				}
				else if (p == P{ '&', '&' }) {

					i += 2;
				}
				else if (p == P{ '|', '|' }) {

					i += 2;
				}
				else flag = true;
			}
			if (flag) {
				switch (source[i]) {
				case '=':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("="sv)); i++;
					break;
				case '+':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("+"sv)); i++;
					break;
				case '-':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("-"sv)); i++;
					break;
				case '*':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("*"sv)); i++;
					break;
				case '/':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("/"sv)); i++;
					break;
				case '%':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("%"sv)); i++;
					break;
				case 92: // \ //
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("\\"sv)); i++;
					break;
				case '!':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("!"sv)); i++;
					break;
				case '<':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("<"sv)); i++;
					break;
				case '>':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(">"sv)); i++;
					break;
				case '(':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("("sv)); i++;
					break;
				case ')':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(")"sv)); i++;
					break;
				case '{':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("{"sv)); i++;
					break;
				case '}':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("}"sv)); i++;
					break;
				case '[':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("["sv)); i++;
					break;
				case ']':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("]"sv)); i++;
					break;
				case '.':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("."sv)); i++;
					break;
				case ',':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(","sv)); i++;
					break;
				case ';':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(";"sv)); i++;
					break;
				case (char)34: { // " //
					uint64_t x = 1;
					while (i + x < source.size() && !(source[i + x] == (char)34 && source[i + x - 1] != (char)92)) x++;
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(substr(source, i, x + 1)));
					i += x + 1;
				}
					break;
				default:
					if (is_identifier_first(source[i])) {
						uint64_t x = 1;
						while (is_identifier_middle(source[i + x])) x++;
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(substr(source, i, x)));
						i += x;
					}
					else if (is_number(source[i])) {
						uint64_t x = 1;
						while (is_number(source[i + x])) x++;
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(substr(source, i, x)));
						i += x;
					}
					else {
						std::cout << "Unexpected character " << source[i] << std::endl;
						throw std::exception("Syntax error : Unexpected character.");
					}
				}
			}
		}
			break;
		default:
			std::cout << "Unexpected character " << source[i] << std::endl;
			throw std::exception("Syntax error.");
		}
	}

	A:
	std::cout << "Well done!" << std::endl;
	for (auto&& cur : token_vector) {
		std::cout << cur->data << std::endl;
	}
}